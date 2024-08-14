# the XML filters according to the device YANG model

ncconfig_open = """<nc:config xmlns:nc="urn:ietf:params:xml:ns:netconf:base:1.0">"""
ncconfig_close = """</nc:config>"""
ncfilter_open = """<nc:filter xmlns:nc="urn:ietf:params:xml:ns:netconf:base:1.0">"""
ncfilter_close = """</nc:filter>"""
notif_open = """<filter xmlns="urn:ietf:params:xml:ns:netconf:notification:1.0">"""
notif_close = """</filter>"""

rpc_reply = "/nc:rpc-reply/nc:data/"

filter_get_current_power = """
    <org-openroadm-device xmlns="http://org/openroadm/device">
        <circuit-packs>
            <circuit-pack-name>cp-main-board</circuit-pack-name>
            <ports>
                <port-id>{}</port-id>
                <port>
                    <port-current-power />
                </port>
            </ports>
        </circuit-packs>
    </org-openroadm-device>
"""

xpath_get_current_power = rpc_reply+"dev:org-openroadm-device/dev:circuit-packs/dev:ports/dev:port/dev:port-current-power"

filter_get_target_gain = """
    <optical-control xmlns="http://com/oplink/optical">
        <work-mode>auto</work-mode>
        <optical-modules>
            <optical-amplifier xmlns="http://openconfig.net/yang/optical-amplfier">
                <amplifiers>
                    <amplifier>
                        <name>{}</name>
                        <config>
                            <target-gain />
                        </config>
                    </amplifier>
                </amplifiers>
            </optical-amplifier>
        </optical-modules>
    </optical-control>
"""

xpath_get_target_gain = rpc_reply+"opt:optical-control/opt:optical-modules/opt-amp:optical-amplifier/opt-amp:amplifiers/opt-amp:amplifier/opt-amp:config/opt-amp:target-gain"

filter_set_target_gain = """
    <optical-control xmlns="http://com/oplink/optical">
        <work-mode>auto</work-mode>
        <optical-modules>
            <optical-amplifier xmlns="http://openconfig.net/yang/optical-amplfier">
                <amplifiers>
                    <amplifier>
                        <name>{}</name>
                        <config>
                            <target-gain>{}</target-gain>
                        </config>
                    </amplifier>
                </amplifiers>
            </optical-amplifier>
        </optical-modules>
    </optical-control>
"""

xpath_get_active_path = rpc_reply+"opt:optical-control/opt:optical-modules/aps:aps/aps:aps-modules/aps:aps-module/aps:state/aps:active-path"

filter_get_active_path = """
    <optical-control xmlns="http://com/oplink/optical">
        <optical-modules>
            <aps xmlns="http://openconfig.net/yang/optical-transport-line-protection">
                <aps-modules>
                    <aps-module>
                        <state>
                            <active-path />
                        </state>
                    </aps-module>
                </aps-modules>
            </aps>
        </optical-modules>
    </optical-control>
"""

filter_get_syslog_server = """
    <syslog xmlns="http://org/openroadm/syslog">
        <log-actions>
            <remote />
        </log-actions>
    </syslog>
"""

xpath_get_syslog_server_addr = rpc_reply+"/syslog:syslog/syslog:log-actions/syslog:remote/syslog:destination/syslog:udp/syslog:address"

xpath_get_syslog_server_port = rpc_reply+"/syslog:syslog/syslog:log-actions/syslog:remote/syslog:destination/syslog:udp/syslog:port"



notification_filter = """
    <alarm-notification xmlns="http://org/openroadm/alarm"/>
"""

xpath_notif_time_stamp = "/notif:notification/notif:eventTime"

xpath_notif_alarm_type = "/notif:notification/alarm:alarm-notification/alarm:type"

xpath_notif_alarm_severity = "/notif:notification/alarm:alarm-notification/alarm:severity"

xpath_notif_alarm_msgid = "/notif:notification/alarm:alarm-notification/alarm:id"


filter_get_node_type = """
    <org-openroadm-device xmlns="http://org/openroadm/device">
        <info>
            <node-type />
        </info>
    </org-openroadm-device>
"""

xpath_get_node_type = rpc_reply+"dev:org-openroadm-device/dev:info/dev:node-type"
