import unittest
import time
from OPLogManager import *
from TestSDVIIC import *
from TestSDVCPLD import *
from TestSDVUART import *
max_test_time = 3600*100
def test_loop():
    sst_cpld_run(0,max_test_time)
    sst_iic_run(0,max_test_time)
    sst_uart_run(0,max_test_time)
    #time.sleep(30)


def test_edvt_loop():
    suits = []
    cpld_suit = edvt_cpld_suite()
    suits.append(cpld_suit)
    uart_suit = edvt_uart_suite()
    suits.append(uart_suit)
    iic_suit = edvt_sdv_iic_suite()
    suits.append(iic_suit)

    for suit in suits:
        unittest.TextTestRunner(verbosity=2).run(suit)
    

log_manager = create_oplogger(isFilelog=True)
if __name__ == '__main__':
    #test_loop()
    test_edvt_loop()