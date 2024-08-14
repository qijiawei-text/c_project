'''
Created on Oct 11, 2017

@author: qxiong06
'''

import multiprocessing
import multiprocessing.queues
import threading
import logging
import time
import os
import serial
from pyuart import PyUart
from util import *
from OPLogManager import OPMultiprocessLogger
class BootMonitor(object):
    '''
    classdocs
    '''
    def __init__(self, uart_name,baudrate,sync_queue,res_queue,logger=None):
        '''
        Constructor
        '''
        try:
            #the event used to inform the main process
            self.uart_name = uart_name
            manager = multiprocessing.Manager()
            self.mon_ready_event = manager.Event()
            self.ctl_ready_event = manager.Event()
            self.res_queue = res_queue     #The queue used to send back data to main process
            self.sync_queue = sync_queue
            self.sync_queue.put(self.mon_ready_event)
            self.sync_queue.put(self.ctl_ready_event)
            if not logger:
                self.logger = logging.getLogger("BOOT")
            else:
                self.logger = logger #OPMultiprocessLogger.getLogger(logger_queue,"BOOTLOG",logging.DEBUG)
            self.uart = PyUart(uart_name,baudrate,timeout=1,logger=self.logger)
            self.test_rounds=0
            self.boot_log_file = None
            self.reader_loop_alive = False
            self.control_loop_alive = False
        except:
            raise

    def set_run_cfg(self,boot_timeout=3*60,uart_read_timeout=10,log_path=None):
        self.boot_timeout = boot_timeout
        self.uart_read_timeout = uart_read_timeout
        self.log_path = log_path

    def set_check_info(self,prompt,powered_on_check_func=None,kernel_version_check_func=None):    
        self.expect_prompt = prompt
        self.kernel_version_check = kernel_version_check_func
        self.powered_on_check = powered_on_check_func
    def get_config_queue(self):
        cfg = self.sync_queue.get()
        
    def prepare(self):
        if not self.log_path or not os.path.isdir(self.log_path):            
            self.log_path = os.path.join(os.getcwd(),'boot_log')
            os.makedirs(self.log_path,exist_ok=True)
            self.logger.warning("Didn't give correct log path, use the current directory:%s"%(self.log_path))

        log_file_name = str(self.test_rounds)+'-'+time.strftime("(%Y-%m-%d-%H-%M-%S)")+self.uart_name+'-boot.log'
        log_file = os.path.join(self.log_path,log_file_name)
        self.logger.info("current linux boot log file:"+log_file)
        self.boot_log_file = open(log_file,mode='wb')
        if not self.res_queue.empty():
            self.logger.error("The queue is not empty!")


    def stop_control_loop(self):
        self.control_loop_alive = False

    def check_board_really_powerdown(self):
        con = self.uart.read_print()
        if con:
            return False
        if self.check_poweron_result():
            return False
        return True

    def get_sync_info(self):
        if not self.sync_queue.empty():
            sync_info = self.sync_queue.get_nowait()
            return sync_info 
        else:
            return False
    def check_control_loop_end(self):
        sync_info = self.get_sync_info()
        if sync_info and "STOP" in sync_info and sync_info["STOP"]:
            return True
        return False
    def control_loop(self):
        '''
        The main loop function.
        '''
        while self.control_loop_alive:
            self.prepare()
            #Tell the main process we are ready
            self.logger.info("Monitor thread ready. Set ready_event...")
            self.mon_ready_event.set()
            #Wait main process tell us to begin.
            self.ctl_ready_event.wait()
            self.ctl_ready_event.clear()
            if self.check_control_loop_end():
                self.stop_control_loop()
                continue
            ##start

            self.logger.info("Round %d monitor thread start..."%(self.test_rounds))
            self.start_reader_loop()
            self.wait_reader_thread()
            poweron_res = {}
            if self.check_poweron_result():
                poweron_res["POWERON"] = StatusCode.SUCCESS
            else:
                poweron_res["POWERON"] = StatusCode.FAIL
            self.res_queue.put(poweron_res)
            self.logger.info("Round %d monitor thread end..."%(self.test_rounds))
            self.test_rounds += 1

    def check_prompt(self,received_str):
        if len(received_str)>=len(self.expect_prompt):
            received_str = received_str[-len(self.expect_prompt):]
        else:
            return False
        if received_str == self.expect_prompt:
            return True
        else:
            return False


    def run_command_and_check(self,cmd,expect_response):
        try:
            res = self.uart.send_data(cmd)
            self.logger.info("get:%s,expect:%s"%(res,expect_response))
            if len(res)<5:
                for ele in res:
                    if ele.decode().find(expect_response):
                        return True
            return False
        except BaseException as err:
            self.logger.error(str(err))
            return False

    def run_bps_command_and_check(self,cmd,expect_response):
        try:
            res = self.uart.send_data(cmd)
            self.logger.info("received res:%s expect_response:%s"%(res,expect_response))
            if 3 == len(res):
                if expect_response in res[1].decode():
                    res = self.uart.send_data("\r\n")
                    res = self.uart.send_data("\r\n")
                    return True
            res = self.uart.send_data("\r\n")
            res = self.uart.send_data("\r\n")
            return False
        except BaseException as err:
            self.logger.error(str(err))
            return False

    def check_poweron_result(self):
        if hasattr(self, "powered_on_check") and self.powered_on_check:
            print("Use cumstomer check")
            res = self.powered_on_check()
            return res
        else:
            res = self.run_command_and_check("\n",self.expect_prompt)
            if res:
                self.logger.info("POWERON SUCCESS")
            else:
                self.logger.error("POWERON FAIL")
            return res


    def start_reader_loop(self):
        """Start reader thread"""
        self.reader_loop_alive = True
        # start serial->console thread
        self.receiver_thread = threading.Thread(target=self.receive_loop, name='receive')
        #self.receiver_thread.daemon = True
        self.receiver_thread.start()

    def stop_reader_loop(self):
        """Stop reader thread only, wait for clean exit of thread"""
        self.reader_loop_alive = False
        if hasattr(self.uart.serial, 'cancel_read'):
            self.uart.serial.cancel_read()
        self.receiver_thread.join()


    def wait_reader_thread(self):
        """wait for worker threads to terminate"""
        self.receiver_thread.join(self.boot_timeout)
        self.stop_reader_loop()
        self.boot_log_file.close()
        self.logger.info("Read loop ended")


    def receive_loop(self):
        """read data"""
        whole_time = time.time()
        last_received=""
        timeout_check = False
        self.logger.info("enter receive_loop")
        self.logger.info("uart %s "%(self.uart.serial.name))
        while self.reader_loop_alive:
            try:
                #get the number of bytes in buffer.
                read_bytes = self.uart.serial.inWaiting()
                if 0 == read_bytes:
                    self.uart.serial.timeout = self.uart_read_timeout
                    timeout_check = True
                    read_bytes = 1
                else:
                    self.uart.serial.timeout = 2

                #data = self.uart.serial.read(read_bytes).decode()
                data = self.uart.serial.read(read_bytes)
                if data:
                    #self.logger.info(data)
                    self.boot_log_file.write(data)
                    self.boot_log_file.flush()
                    
                else:#no data, mean time out
                    if timeout_check:
                        self.logger.info("timeout_check")
                        timeout_check = False
                        
                        if self.check_poweron_result():
                            self.reader_loop_alive = False

            except BaseException as err:
                self.logger.error(str(err))
                #self.reader_loop_alive = False
            else:
                if whole_time+self.boot_timeout < time.time():
                    self.logger.error("Booting wait timeout!")
                    self.reader_loop_alive = False
        self.boot_log_file.flush()
        self.logger.info("This round time:%d"%(time.time()-whole_time))
    def __del__(self):
        print("BootMonitor end")


def boot_monitor_main(uart_name,baudrate,sync_queue, res_queue, prompt,logger_queue=None,log_path=None, thread_timeout=2.5*60,read_timeout=10):
    #Must config the logger at the process bigning.
    OPMultiprocessLogger.worker_configurer(logger_queue,logging.DEBUG)
    #logger.info("Start BOOT process")
    boot_monitor = BootMonitor(uart_name,baudrate,sync_queue,res_queue)
    boot_monitor.set_run_cfg(thread_timeout,read_timeout,log_path)
    boot_monitor.set_check_info(prompt)
    boot_monitor.control_loop_alive = True
    boot_monitor.control_loop()
    


def start_boot_monitor(uart_name,baudrate,sync_queue, res_queue,prompt, logger_manager=None,log_path=None, thread_timeout=3*60,read_timeout=10):
    #logger = logger_manager.getLogger("Create")
    #logger.info("Create monitor process")
    ctl_pr = multiprocessing.Process(target=boot_monitor_main,args=(uart_name,baudrate,sync_queue, res_queue,prompt,logger_manager.get_queue(),log_path,thread_timeout,read_timeout))
    ctl_pr.start()
    return ctl_pr

