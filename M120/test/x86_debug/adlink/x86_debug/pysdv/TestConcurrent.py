#!/usr/bin/python
# -*- coding: utf-8 -*-
import struct
import os
import time
import socket
import threading
import multiprocessing
import hashlib
from MultiProcessTest import *
from TestSDVIIC import *
from TestSDVCPLD import *
from TestSDVUART import *
from OPLogManager import *
from ReplayServerClient import *


def i2c_multithreads():
    test_functions=[{"Func":"Test_SDV_ReadBoardTempLoop",     "args":(10,0,60*60*12)},
                    {"Func":"Test_SDV_PowerInfoReadLoop", "args":(10,0,60*60*12)},
                    {"Func":"Test_SDV_EEpromCfgReadWriteLoop","args":(10,0,60*60*12)},
                    {"Func":"Test_SDV_EEpromIDReadWriteLoop", "args":(10,0,60*60*12)}
                    ]
    sdv_iic = TestSDVIIC()
    sdv_iic_th = Multithread(sdv_iic,test_functions)
    sdv_iic_th.create_threads()
    return sdv_iic_th.Threads



def uart_multithreads():
        data1 = [0x55]*1024
        data2 = [0xAA]*1024
        data3 = [0xFF]*1024
        data4 = [0x5A]*1024
        data5 = [0xA5]*1024
        test_functions=[{"Func":"SDV_UartWriteRead", "args":(SDVUART.UARTCHOOSE.CPLD_UART_EDFA1,data1)},
                        {"Func":"SDV_UartWriteRead", "args":(SDVUART.UARTCHOOSE.CPLD_UART_EDFA2,data2)},
                        {"Func":"SDV_UartWriteRead", "args":(SDVUART.UARTCHOOSE.CPLD_UART_EXTEND,data3)},
                        {"Func":"SDV_UartWriteRead", "args":(SDVUART.UARTCHOOSE.CPLD_UART_OCM,data4)},
                        {"Func":"SDV_UartWriteRead", "args":(SDVUART.UARTCHOOSE.CPLD_UART_WSS,data5)},
                        ]
        sdv_uart = TestSDVUART()
        sdv_uart_th = Multithread(sdv_uart,test_functions)
        sdv_uart_th.create_threads()
        return sdv_uart_th.Threads

linux_log_dir = "/home/oplink/bootdisk/"
if __name__ == '__main__':
    logger = create_oplogger("MAIN",linux_log_dir,isFilelog=False)
    threads = []
    threads = i2c_multithreads()
    threads += cpld_multithreads()
    threads += uart_multithreads()
    threads += start_linux_server_client()
    print(threads)
    for th in threads:
        th.start()
    for th in threads:
        th.join()


    #test_ins = Test_SDVIIC(logger)
    #run_test_processes(test_ins)

