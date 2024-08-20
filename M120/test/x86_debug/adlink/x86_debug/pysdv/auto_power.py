import time
import os
import logging
import configparser
from OPLogManager import *
from bootmonitor import *
import multiprocessing
from util import *
from powerctl import*

class BootMonitorProcess(object):
    def __init__(self,uart_name,uart_baud,sync_queue, res_queue,expect_prompt,log_manager,logger,log_path=None,boot_timeout=100, read_timeout=15):
        self.uart_name = uart_name
        self.process_ins = start_boot_monitor(uart_name,uart_baud,sync_queue, res_queue,expect_prompt,log_manager,log_path,boot_timeout,read_timeout)
        self.sync_queue = sync_queue
        self.res_queue = res_queue
        self.expect_prompt = expect_prompt
        self.log_manager = log_manager
        self.logger = logger#logging.getLogger("MonitorPr")
        self.log_path  = log_path
        self.monitor_ready_ev = self.sync_queue.get(1)
        self.ctl_ready_ev = self.sync_queue.get(1)
        self.logger.info("uart:%s,monitor_ev:%s,ctl_ev:%s"%(self.uart_name,str(self.monitor_ready_ev),str(self.ctl_ready_ev)))
        self.success_rounds = 0
        self.fail_rounds = 0
        self.fail_rounds_arr = []
        self.total_rounds = 0
        
    def wait_check_result(self):
        try:
            res = self.res_queue.get()
            self.logger.info("board %s, round %d ,result:%s"%(self.uart_name,self.total_rounds,str(res)))
            self.total_rounds += 1
            if StatusCode.SUCCESS == res["POWERON"]:
                self.success_rounds += 1
            elif StatusCode.FAIL == res["POWERON"]:
                self.fail_rounds_arr.append(self.total_rounds)
                self.fail_rounds += 1
            elif res == "NOT POWERDOWN":
                self.logger.info("Fake rounds")
            self.print_result()
        except Exception as error:
            self.logger.error(str(error))
            
    def print_result(self):    
        self.logger.info("board:%s Total tested rounds:%d, success rounds:%d, fail rounds:%d"%(self.uart_name,self.total_rounds, self.success_rounds, self.fail_rounds) )
        self.logger.info("Fail rounds:"+str(self.fail_rounds_arr))


def poweronoff_round(log_manager,power_conf,max_rounds=10,max_time=60*8):
    logger = log_manager.getLogger("Control")
    expect_prompt = "root@localhost:"
    manager = multiprocessing.Manager()
    board_seq = get_oc03_seq(power_conf)
    logger.info(board_seq)
    monitors = []
    for ele in board_seq:
        sync_queue = manager.Queue()
        res_queue = manager.Queue()
        monitor = BootMonitorProcess(ele['port'],ele['baudrate'],sync_queue,res_queue,expect_prompt,log_manager,logger)
        monitors.append(monitor)
    #logger.stop_logger()
    uart_conf = get_psw_uart_conf(power_conf)
    power_ctl = PowerCtl(uart_conf,logger)
    power_seq = power_ctl.get_auto_power_seq(power_conf)
    logger.info("power sequence:%s"%(str(power_seq)))
    rounds = 0
    start_time = time.time()

    this_temp_start_time=0#time.time()
    this_temp_end_time=0
    for ele in power_seq:
        logger.info("This rounds info %s"%(ele))
        this_round = ele["loop"]
        power_ctl.set_power(ele,False)
        if power_ctl.get_start_time_flag(ele):
            this_temp_start_time=time.time()
            this_temp_end_time=0
        res = power_ctl.get_end_time_flag(ele)
        if res:
            this_temp_end_time = this_temp_start_time + res*60
            logger.info("Start time:%d, end time:%d"%(this_temp_start_time,this_temp_end_time))
        while this_round > 0:
            try:
                #wait for monitor thread ready
                this_round_start_time = time.time()
                logger.info("Wait for monitor process ready")
                logger.info("Rest rounds for this config:%d"%(this_round))
                for mon in monitors:
                    mon.monitor_ready_ev.wait()
                    mon.monitor_ready_ev.clear()
                logger.info("Turn on test board power")
                power_ctl.enable_power()
                for mon in monitors:
                    mon.ctl_ready_ev.set()
                #res_queue.clear()
                for mon in monitors:
                    res = mon.wait_check_result()
                this_round_cost_time = time.time()-this_round_start_time
                logger.info("Round %d  cost time:%f S"%(rounds,this_round_cost_time))
                logger.info("Turn off test board power")
                power_ctl.disable_power()
                time.sleep(ele['sleep']*60)
                this_round -= 1
            except Exception as error:
                logger.exception(str(error))
        logger.info("Finished this rounds!")
        while this_temp_end_time > time.time():
            time.sleep(10)
            logger.info("Waiting for temperature time end time:%d, current time:%d"%(this_temp_end_time,time.time()))

def create_monitor_process(test_board_uart_name,test_board_baudrate,
                           monitor_ready_ev,monitor_start_ev,powerdown_ev,res_queue,log_manager=None):
    monitor_process = multiprocessing.Process(target=boot_monitor_main,
                                              args=(test_board_uart_name,test_board_baudrate,
                                              monitor_ready_ev,monitor_start_ev,powerdown_ev,res_queue,
                                               "3.10.62-ltsi-JUNIPER\r\n","root@localhost:~#",log_manager,os.getcwd()))
    #logger.info("Created monitor process")
    return monitor_process





def get_config(conf_path):
    config = configparser.ConfigParser()
    config.read(conf_path)
    return config
 

def main(conf = "auto_power_config.ini"):
    import argparse
    parser = argparse.ArgumentParser(
        description="auto power on/off test.")

    parser.add_argument(
        "--conf",
        nargs='?',
        type=str,
        help="Config file path.",
        default=os.path.join(ROOT_PATH,conf))

    args = parser.parse_args()
    conf = get_config(args.conf)
    log_manager = create_oplogger(isFilelog=True,is_multiprocess=True)
    #logger = create_oplogger(isFilelog=True,is_multiprocess=True,queue=queue)
    #test_flow_for_one_voltage(logger)
    #logger=logging.getLogger("Control")
    poweronoff_round(log_manager,conf)
    #logger = logging.getLogger("TEST")
    
    #log_manager.stop_logger()
    print("end")
    time.sleep(10)


ROOT_PATH = os.getcwd()

if __name__ == '__main__':

    main()


    #psw = PyPsw("172.16.166.57")

