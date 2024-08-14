import os
import configparser
import time
import logging
from util import *
from pypsw import *
from OPLogManager import *

ROOT_PATH = os.getcwd()
CURRENT_NAME='current'
VOLTAGE_NAME='voltage'
PERIOD_NAME='period'
POWER_SEQ_NAME='POWER_SEQ_'
AUTO_POWER_SEQ_NAME='AUTO_POWER_SEQ_'
SLEEP_NAME='sleep'
LOOP_NAME='loop'
PSW_NAME='PSW'
PSW_PORT_NAME='psw_Port'
PSW_BAUD_NAME='psw_baudrate'
BOARD_NAME='OC03_'
CHECK_TIME_START_NAME='check_time_start'
CHECK_TIME_END_NAME='check_time_end'
#logger = None#logging.getLogger("PSW")
def get_config(conf_path):
    config = configparser.ConfigParser()
    config.read(conf_path)
    return config
 
def get_psw_uart_conf(config):   
    port = config['PSW']['psw_port']
    baud = config['PSW'].getint('psw_baudrate')
    uart_cfg={}
    uart_cfg[PSW_PORT_NAME]=port
    uart_cfg[PSW_BAUD_NAME]=baud
    return uart_cfg
def get_oc03_seq(config):   
    seq_idx=0
    board_seq =[]
    while True:
        sec_name = BOARD_NAME+"%d"%(seq_idx)
        seq_idx +=1
        board_cfg={}
        if config.has_section(sec_name):
            board_cfg['port']=config[sec_name]['port']
            board_cfg['baudrate']=config[sec_name].getint('baudrate')
            board_seq.append(board_cfg)
        else:
            break
    #logger.info("Power sequence:%s"%(str(board_seq)))
    return board_seq

class PowerCtl(object):
    def __init__(self,uart_conf,logger=None):
        if not logger:
            self.logger = logging.getLogger("PowerCtl")
        else:
            self.logger = logger
        self.uart_conf = uart_conf
            
    
    def get_auto_power_seq(self,config):
        seq_idx=0
        power_seq=[]
        while True:
            sec_name = AUTO_POWER_SEQ_NAME+"%d"%(seq_idx)
            seq_idx +=1
            power_cfg={}
            if config.has_section(sec_name):
                power_cfg[CURRENT_NAME]=config[sec_name].getfloat(CURRENT_NAME)
                power_cfg[VOLTAGE_NAME]=config[sec_name].getfloat(VOLTAGE_NAME)
                power_cfg[SLEEP_NAME]=config[sec_name].getfloat(SLEEP_NAME)
                power_cfg[LOOP_NAME]=config[sec_name].getfloat(LOOP_NAME)
                if config.has_option(sec_name,CHECK_TIME_START_NAME):
                    power_cfg[CHECK_TIME_START_NAME]=config[sec_name].getfloat(CHECK_TIME_START_NAME)
                if config.has_option(sec_name,CHECK_TIME_END_NAME):
                    power_cfg[CHECK_TIME_END_NAME]=config[sec_name].getfloat(CHECK_TIME_END_NAME)

                power_seq.append(power_cfg)
            else:
                break
        self.logger.info("Power sequence:%s"%(str(power_seq)))
        return power_seq
    
    def get_start_time_flag(self,power_conf):
        if CHECK_TIME_START_NAME in power_conf:
            self.logger.info("Start one temperature")
            return True
        else:
            return False
    def get_end_time_flag(self,power_conf): 
        if CHECK_TIME_END_NAME in power_conf:
            self.logger.info("Last one temperature")
            return power_conf[CHECK_TIME_END_NAME]
        else:
            return False
    def get_power_seq(self,config):
        seq_idx=0
        power_seq=[]
        while True:
            sec_name = POWER_SEQ_NAME+"%d"%(seq_idx)
            seq_idx +=1
            power_cfg={}
            if config.has_section(sec_name):
                power_cfg[CURRENT_NAME]=config[sec_name].getfloat(CURRENT_NAME)
                power_cfg[VOLTAGE_NAME]=config[sec_name].getfloat(VOLTAGE_NAME)
                power_cfg[PERIOD_NAME]=config[sec_name].getfloat(PERIOD_NAME)
                power_seq.append(power_cfg)
            else:
                break
        self.logger.info("Power sequence:%s"%(str(power_seq)))
        return power_seq
    
    def get_power_uart_instance(self):
        uart_cfg=self.uart_conf
        try_times =5
        while try_times>0:
            try:
                psw = PyPsw(uart_cfg[PSW_PORT_NAME],uart_cfg[PSW_BAUD_NAME],logger=self.logger)
                return psw
            except Exception as error:
                self.logger.exception("Fail with:%s"%(error))
            try_times-=1
            time.sleep(1)
        return StatusCode.FAIL 
    
    def set_power(self,power_cfg,enable=True): 
        try_times =5
        psw = self.get_power_uart_instance()
        if StatusCode.FAIL == psw:
            return psw
        while try_times>0:
            try_times-=1
            try:
                psw.query_error()
                res = psw.SetPSWVoltageSlewRate(0.1,0.1) 
                if res == StatusCode.FAIL:
                    continue
                res = psw.SetPSWVoltageCurrent(power_cfg[VOLTAGE_NAME], power_cfg[CURRENT_NAME],enable) 
                if res == StatusCode.FAIL:
                    continue
                return res
            except Exception as error:
                self.logger.exception("Fail with:%s"%(error))
        self.logger.error("Set power FAIL!")
        return StatusCode.FAIL
    
    def disable_power(self):
        try_times =5
        psw = self.get_power_uart_instance()
        if StatusCode.FAIL == psw:
            return psw
        while try_times>0:
            try_times-=1
            try:
                psw.query_error()
                psw.disable_output()
                psw.measure_power()
                return 
            except Exception as error:
                self.logger.error("Fail with:%s"%(error))
    
    def enable_power(self):
        try_times = 5
        psw = self.get_power_uart_instance()
        if StatusCode.FAIL == psw:
            return psw
        while try_times>0:
            try_times-=1
            try:
                psw.query_error()
                psw.enable_output()
                psw.measure_power()
                return 
            except Exception as error:
                self.logger.error("Fail with:%s"%(error))
    
       
     
def power_round(config):
    logger = logging.getLogger("POWER") 
    uart_cfg = get_psw_uart_conf(config)
    logger.info("psw uart:%s"%(str(uart_cfg))) 
    pwc = PowerCtl(uart_cfg,logger)
    power_seq = pwc.get_power_seq(config)
    for power_cfg in power_seq:
        pwc.set_power(power_cfg)
        sleep_sec = power_cfg[PERIOD_NAME]*60
        time.sleep(sleep_sec)
    time.sleep(5)
    pwc.disable_power() 

def main(conf = "config.ini"):
    import argparse
    parser = argparse.ArgumentParser(
        description="auto download test.")

    parser.add_argument(
        "--conf",
        nargs='?',
        type=str,
        help="Config file path.",
        default=os.path.join(ROOT_PATH,conf))

    args = parser.parse_args()
    conf = get_config(args.conf)
    power_round(conf)

if __name__ == '__main__':
    create_oplogger()
    main()