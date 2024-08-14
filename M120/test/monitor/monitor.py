from datetime import datetime
from importlib import reload
from time import sleep
from logging.handlers import SysLogHandler
import logging
import settings
from ncConn import NCConnection
from xmlUtil import *

class Monitor(NCConnection):
    """ Monitors whether target-gain aligns with the target-output configured. """
    def __init__(self):
        super(Monitor, self).__init__()
        self.ns = {"nc": "urn:ietf:params:xml:ns:netconf:base:1.0", 
            "dev": "http://org/openroadm/device",
            "opt": "http://com/oplink/optical",
            "opt-amp": "http://openconfig.net/yang/optical-amplfier",
            "aps": "http://openconfig.net/yang/optical-transport-line-protection",
            "notif": "urn:ietf:params:xml:ns:netconf:notification:1.0",
            "alarm": "http://org/openroadm/alarm",
            "syslog": "http://org/openroadm/syslog"
        }
        self.syslog_format = '''{} {} {} {} [{}]: "srvafct": "{}", "severity":"{}", "description":"{}"'''
        self.DEBUG = settings.DEBUG
        self.node_name = self.get_node_name()
        self.app_name = "MonitorScript"
        self.level = "alarm"

    def get_node_name(self):
        reply = self.nc.get(filter=ncfilter_open+filter_get_node_type+ncfilter_close)
        node_name_ele = reply.data.xpath(xpath_get_node_type, namespaces=self.ns)
        return node_name_ele[0].text

    def get_active_path(self):
        reply = self.nc.get(filter=ncfilter_open+filter_get_active_path+ncfilter_close)
        active_path_ele = reply.data.xpath(xpath_get_active_path, namespaces=self.ns)
        active_path = active_path_ele[0].text
        if (active_path == "PRIMARY"):
            return 3
        elif (active_path == "SECONDARY"):
            return 5
        else:
            exit(1)

    def form_syslog(self, notif):
        notif_ele = notif.notification_ele
        time = notif_ele.xpath(xpath_notif_time_stamp, namespaces=self.ns)[0].text
        type = notif_ele.xpath(xpath_notif_alarm_type, namespaces=self.ns)[0].text
        severity = notif_ele.xpath(xpath_notif_alarm_severity, namespaces=self.ns)[0].text
        msgid = notif_ele.xpath(xpath_notif_alarm_msgid, namespaces=self.ns)[0].text
        return self.syslog_format.format(time, self.node_name, settings.HOST, self.app_name, self.level, type, severity, msgid)

    def test_monitor_power(self):
        # setup logger
        logger = logging.getLogger()
        reply = self.nc.get(filter=ncfilter_open+filter_get_syslog_server+ncfilter_close)
        syslog_server_addr_ele = reply.data.xpath(xpath_get_syslog_server_addr, namespaces=self.ns)
        syslog_server_port_ele = reply.data.xpath(xpath_get_syslog_server_port, namespaces=self.ns)
        for i in range(len(syslog_server_addr_ele)):
            server = (syslog_server_addr_ele[i].text, int(syslog_server_port_ele[i].text))
            logger.addHandler(SysLogHandler(server))
        logger.addHandler(logging.FileHandler("/app/monitor.log"))
        if self.DEBUG:
            logger.setLevel(logging.DEBUG)
        else:
            logger.setLevel(logging.WARN)

        TARGET_POWER = {}
        TYPE = settings.TYPE # ? there may be a better way to determine the device type
        # change this block of codes if device type is different
        if (TYPE == "AMP"):
            port_num = 1
        elif (TYPE == "OPS"):
            port_num = self.get_active_path()
        else:
            exit(1)
        while True:
            try:
                # reload settings, so that it is in sync with any update
                reload(settings)
                TARGET_POWER["PA"] = settings.PA_TARGET_POWER
                TARGET_POWER["BA"] = settings.BA_TARGET_POWER
                monitor_interval = settings.monitor_interval
                diff_threshold = settings.diff_threshold
                
                # BA is not configurable on the testing device available for me, 
                # if it is, can use the for loop beneath
                selection = "PA"
                # for selection in ["PA", "BA"]:

                # get target gain
                reply = self.nc.get_config(source="running", filter=ncfilter_open+filter_get_target_gain.format(selection)+ncfilter_close)
                target_gain_ele = reply.data.xpath(xpath_get_target_gain.format(selection), namespaces=self.ns)
                target_gain_text = target_gain_ele[0].text
                target_gain = float(target_gain_text)

                # get current input power
                reply = self.nc.get(filter=ncfilter_open+filter_get_current_power.format(port_num)+ncfilter_close)
                current_power_ele = reply.data.xpath(xpath_get_current_power.format(port_num), namespaces=self.ns)
                current_power_text = current_power_ele[0].text
                current_power = float(current_power_text)

                target_power = current_power + target_gain

                # send syslog if diff > threshold
                if (abs(target_power - TARGET_POWER[selection]) > diff_threshold):
                    time = datetime.utcnow().isoformat()[:-7] + 'Z'
                    type = "qualityOfService"
                    severity = "critical"
                    msgid = "UPDATE_TargetGain"
                    logger.warning(self.syslog_format.format(time, self.node_name, settings.HOST, self.app_name, self.level, type, severity, msgid))
                    # uncomment the lines below if the script should be responsible to do the correction/update
                    # new_target_gain = round(TARGET_POWER[selection] - current_power, 2)
                    # reply = self.nc.edit_config(target="running", config=ncconfig_open+filter_set_target_gain.format(selection, new_target_gain)+ncconfig_close)
                
                while True:
                    notif = self.nc.take_notification(block=False)
                    if notif is None:
                        break
                    logger.warning(self.form_syslog(notif))
                sleep(monitor_interval)
            except KeyboardInterrupt:
                self.tear_down()
                exit(0)

if __name__ == "__main__":
    monitor = Monitor()
    monitor.test_monitor_power()
