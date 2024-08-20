import struct
import time
import unittest
from sdv import *
from TestSDV import *
from OPLogManager import *
from MultiProcessTest import *
from util import *

class TestSDVUART(TestSDV):
    #test to get this max length value
    #This is not a very accurate value
    max_loopback_len = 1024
    def __init__(self, logger=None):
        super(TestSDVUART,self).__init__(logger)
        self.sdvuart = SDVUART(self.logger)
        self.index = 0

    def Test_UartWriteRead(self,uart_device,length):
        res = self.rw_test(self.sdvuart.SDV_UartWrite,self.sdvuart.SDV_UartRead,(uart_device,length))
        return res
    
    def Test_UartBaudrateLoop(self,func):
        def UartBaudrateLoop(*args,**kwargs):
            is_fail = False
            for baud in SDVUART.UART_BAUDRATES:
                res = self.sdvuart.SDV_UartSetBaud(baud)
                if StatusCode.SUCCESS != res:
                    is_fail = True
                    continue
                else:
                    res = func(*args,**kwargs)
                    if StatusCode.FAIL == res:
                        self.logger.error("%s FAIL at baudrate:%d"%(func,baud))
                        is_fail = True
            if is_fail:
                return StatusCode.FAIL
            else:
                return StatusCode.SUCCESS
        return UartBaudrateLoop

    def Test_UartBaudrateLoopWriteRead(self,uart_device,length):
        return self.Test_UartBaudrateLoop(self.Test_UartWriteRead)(uart_device,length)
        
    def Test_UartStopbitLoop(self,func):
        def UartStopbitLoop(*args,**kwargs):
            is_fail = False
            for stopbit in SDVUART.UART_STOPBITS:
                res = self.sdvuart.SDV_UartSetStopBit(stopbit)
                if StatusCode.SUCCESS != res:
                    is_fail = True
                    continue
                else:
                    res = func(*args,**kwargs)
                    if StatusCode.FAIL == res:
                        self.logger.error("%s FAIL at stop bits:%d"%(func, stopbit))
                        is_fail = True
            if is_fail:
                return StatusCode.FAIL
            else:
                return StatusCode.SUCCESS
        return UartStopbitLoop

    def Test_UartStopbitLoopWriteRead(self,uart_device,length):
        return self.Test_UartStopbitLoop(self.Test_UartWriteRead)(uart_device, length)

    def Test_UartFlowctlLoop(self,func):
        def UartFlowctlLoop(*args, **kwargs):
            is_fail = False
            for flowctl in SDVUART.UART_FLOWCTLS:
                res = self.sdvuart.SDV_UartSetFlowCtl(flowctl)
                if StatusCode.SUCCESS != res:
                    is_fail = True
                    continue
                else:
                    res = func(*args,**kwargs)
                    if StatusCode.FAIL == res:
                        self.logger.error("%s FAIL at flow contrl:%d"%(func, flowctl))
                        is_fail = True
            if is_fail:
                return StatusCode.FAIL
            else:
                return StatusCode.SUCCESS
        return UartFlowctlLoop
    
    def Test_UartFlowctlLoopWriteRead(self,uart_device, length):
        return self.Test_UartFlowctlLoop(self.Test_UartWriteRead)(uart_device, length)
     
    def Test_UartSettingLoopWriteRead(self, uart_device, length):
        self.sdvuart.SDV_UartLock()
        res = self.Test_UartStopbitLoop(self.Test_UartBaudrateLoopWriteRead)(uart_device,length)
        self.sdvuart.SDV_UartUnLock()
        return res
    
    def Test_UartSettingLoopWriteReadRound(self,uart_device,max_len): 
        return self.test_addr_len_loop(max_len,uart_device,0)(self.Test_UartSettingLoopWriteRead)

    def Test_UartSettingLoopWriteReadLoop(self,uart_device,max_rounds=1000, max_time=60*60*8): 
        res = self.test_rounds_loop(max_rounds,max_time)(self.Test_UartSettingLoopWriteReadRound, uart_device,self.max_loopback_len)
        return res
    
    def Test_UartWriteReadRound(self,uart_device,max_len):
        return self.test_addr_len_loop(max_len,uart_device,0)(self.Test_UartWriteRead)
    
    def Test_UartWriteReadLoop(self,uart_device, max_rounds = 1000, max_time=60*60*8):
        return self.test_rounds_loop(max_rounds,max_time)(self.Test_UartWriteReadRound, uart_device,self.max_loopback_len)

    def Test_Edfa1UartWriteReadLoop(self,log_freq=10,max_rounds = 1000, max_time=60*60*8):
        return self.Test_UartWriteReadLoop(SDVUART.UARTCHOOSE.CPLD_UART_EDFA1, max_rounds, max_time)
    def Test_Edfa2UartWriteReadLoop(self,log_freq=10,max_rounds = 1000, max_time=60*60*8):
        return self.Test_UartWriteReadLoop(SDVUART.UARTCHOOSE.CPLD_UART_EDFA2, max_rounds, max_time)
    def Test_WssUartWriteReadLoop(self,log_freq=10,max_rounds = 1000, max_time=60*60*8):
        return self.Test_UartWriteReadLoop(SDVUART.UARTCHOOSE.CPLD_UART_WSS, max_rounds, max_time)
    def Test_OCMUartWriteReadLoop(self,log_freq=10,max_rounds = 1000, max_time=60*60*8):
        return self.Test_UartWriteReadLoop(SDVUART.UARTCHOOSE.CPLD_UART_OCM, max_rounds, max_time)
    def Test_ExtendUartWriteReadLoop(self,log_freq=10,max_rounds = 1000, max_time=60*60*8):
        return self.Test_UartWriteReadLoop(SDVUART.UARTCHOOSE.CPLD_UART_EXTEND, max_rounds, max_time)






   

class UnittestSDVUART(unittest.TestCase):
    def __init__(self,methodName='runTest',max_rounds=1,max_time=8*60*60):
        super(UnittestSDVUART,self).__init__(methodName)
        self.max_time   = max_time
        self.max_rounds = max_rounds

    @classmethod
    def setUpClass(cls):
        cls.logger = logging.getLogger("UART")
        cls.logger.setLevel(logging.INFO)
        cls.logger.debug("Start...")
        cls.test_sdvuart = TestSDVUART(cls.logger)

    def setUp(self):
        self.test_sdvuart = UnittestSDVUART.test_sdvuart
        self.logger       = UnittestSDVUART.logger
        self.test_functions=[
                        {"Func":"Test_UartSettingLoopWriteReadLoop", "args":(SDVUART.UARTCHOOSE.CPLD_UART_EDFA1,self.max_rounds,self.max_time)},
                        {"Func":"Test_UartSettingLoopWriteReadLoop", "args":(SDVUART.UARTCHOOSE.CPLD_UART_EDFA2,self.max_rounds,self.max_time)},
                        {"Func":"Test_UartSettingLoopWriteReadLoop", "args":(SDVUART.UARTCHOOSE.CPLD_UART_EXTEND,self.max_rounds,self.max_time)},
                        {"Func":"Test_UartSettingLoopWriteReadLoop", "args":(SDVUART.UARTCHOOSE.CPLD_UART_OCM,self.max_rounds,self.max_time)},
                        {"Func":"Test_UartSettingLoopWriteReadLoop", "args":(SDVUART.UARTCHOOSE.CPLD_UART_WSS,self.max_rounds,self.max_time)}
                        ]



    #@unittest.skip("Skip")
    def test_Test_Edfa1UartWriteReadLoop(self):
        res = self.test_sdvuart.Test_Edfa1UartWriteReadLoop(max_rounds=1)
        self.assertEqual(res, StatusCode.SUCCESS)

    #@unittest.skip("Skip")
    def test_Test_Edfa2UartWriteReadLoop(self):
        res = self.test_sdvuart.Test_Edfa2UartWriteReadLoop(max_rounds=1)
        self.assertEqual(res, StatusCode.SUCCESS)

    #@unittest.skip("Skip")
    def test_Test_WssUartWriteReadLoop(self):
        res = self.test_sdvuart.Test_WssUartWriteReadLoop(max_rounds=1)
        self.assertEqual(res, StatusCode.SUCCESS)

    #@unittest.skip("Skip")
    def test_Test_OCMUartWriteReadLoop(self):
        res = self.test_sdvuart.Test_OCMUartWriteReadLoop(max_rounds=1)
        self.assertEqual(res, StatusCode.SUCCESS)

    #@unittest.skip("Skip")
    def test_Test_ExtendUartWriteReadLoop(self):
        res = self.test_sdvuart.Test_ExtendUartWriteReadLoop(max_rounds=1)
        self.assertEqual(res, StatusCode.SUCCESS)

    @unittest.skip("Skip")
    def test_Test_UartBaudrateLoopWriteRead(self):
        res = self.test_sdvuart.Test_UartBaudrateLoopWriteRead(SDVUART.UARTCHOOSE.CPLD_UART_EDFA1,1024)
        self.assertEqual(res, StatusCode.SUCCESS)
    
    @unittest.skip("Skip")
    def test_Test_UartStopbitLoopWriteRead(self):   
        res = self.test_sdvuart.Test_UartStopbitLoopWriteRead(SDVUART.UARTCHOOSE.CPLD_UART_EDFA1,1024)
        self.assertEqual(res, StatusCode.SUCCESS)
    @unittest.skip("Skip")
    def test_Test_UartFlowctlLoopWriteRead(self):
        res = self.test_sdvuart.Test_UartFlowctlLoopWriteRead(SDVUART.UARTCHOOSE.CPLD_UART_EDFA1,1024)
        self.assertEqual(res, StatusCode.SUCCESS)
    @unittest.skip("Skip")
    def test_UartSettingsWriteReadLoop(self):
        res = self.test_sdvuart.Test_UartSettingLoopWriteReadLoop(SDVUART.UARTCHOOSE.CPLD_UART_EDFA1,max_rounds=1)
        self.assertEqual(res, StatusCode.SUCCESS)
    
    @unittest.skip("Skip")
    def test_UartWriteReadLoop(self):
        res = self.test_sdvuart.Test_UartWriteReadLoop(SDVUART.UARTCHOOSE.CPLD_UART_EDFA1,1)
        self.assertEqual(res, StatusCode.SUCCESS)

        
    #@unittest.skip("Skip")
    def test_multi(self):
        sdv_uart_pr = MultiProcess(self.test_sdvuart,self.test_functions,self.logger)
        sdv_uart_pr.create_processes()
        sdv_uart_th = MultiThread(self.test_sdvuart,self.test_functions,self.logger)
        sdv_uart_th.create_threads()
        instances = sdv_uart_pr.Processes
        instances + sdv_uart_th.Threads
        for ins in instances:
            ins.start()
        for ins in instances:
            ins.join()
    
def sst_uart_run(max_rounds,max_time):
    uart_test = UnittestSDVUART(max_rounds=max_rounds,max_time=max_time)
    UnittestSDVUART.setUpClass()
    uart_test.setUp()
    pr = multiprocessing.Process(target=uart_test.test_multi,args=())
    pr.start()

def edvt_uart_suite():
    '''
    Choose some of the unit test case to do the EDVT test.
    '''
    suit = unittest.TestSuite()
    suit.addTest(UnittestSDVUART('test_Test_Edfa1UartWriteReadLoop'))
    suit.addTest(UnittestSDVUART('test_Test_Edfa2UartWriteReadLoop'))
    suit.addTest(UnittestSDVUART('test_Test_WssUartWriteReadLoop'))
    suit.addTest(UnittestSDVUART('test_Test_OCMUartWriteReadLoop'))
    suit.addTest(UnittestSDVUART('test_Test_ExtendUartWriteReadLoop'))
    return suit



if __name__ == '__main__':
    create_oplogger()
    unittest.main()