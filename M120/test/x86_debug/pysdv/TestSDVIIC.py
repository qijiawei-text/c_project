
from ctypes import *
from ctypes import util
import random
import os
import logging
import time
import math
import unittest
import multiprocessing
import threading
from sdv import *
from TestSDV import *
from OPLogManager import *
from util import *
from MultiProcessTest import *
from TestSDVCPLD import *

class TestSDVIIC(TestSDV):
    power_info = ""
    def __init__(self, logger=None):
        if not logger:
            self.logger = logging.getLogger("TestSDVIIC")
        else:
            self.logger = logger
        super(TestSDVIIC,self).__init__(logger=self.logger)
        self.sdviic = SDVIIC(self.logger)
        self.testsdvcpld = TestSDVCPLD(self.logger)
        
    def Test_SusiI2CGetFrequency(self,iic_id=SUSI_ID_I2C_EXTERNAL):
        res = self.sdviic.SusiI2CGetFrequency(iic_id)
        self.logger.debug(res)
        return res
    
    def Test_SusiI2CSetFrequency(self,freq,iic_id=SUSI_ID_I2C_EXTERNAL):
        res = self.sdviic.SusiI2CSetFrequency(freq,iic_id)
        if StatusCode.SUCCESS == res:
            res = self.Test_SusiI2CGetFrequency(iic_id)
            if res == str(freq):
                return StatusCode.SUCCESS
            else:
                return StatusCode.FAIL
        else:
            return StatusCode.FAIL
        
    

    def Test_SDV_ReadBoardTemp(self):
        temp = self.sdviic.SDV_ReadBoardTemp()
        if StatusCode.FAIL == temp:
            return temp
        else:
            return "Temp:%s"%(temp)

    def Test_SDV_ReadBoardTempLoop(self,log_freq=10, max_rounds=2000,max_time=60*60*4):
        res = self.test_rounds_loop(max_rounds,max_time)(self.Test_SDV_ReadBoardTemp)
        #self.logger.info("Test_SDV_ReadBoardTempLoop finished one round.")
        return res


    def Test_SDV_PowerInfoRead(self,log_freq,idx=1):
        # In Spec. the max address is 0xF0(240)
        pre_log_time = time.time()
        for addr in range(0, 512):
            for len in range(0,512):
                info = self.sdviic.SDV_PowerInfoRead(addr, len,idx)
                if time.time() - pre_log_time > log_freq:
                    self.logger.info("Test_SDV_PowerInfoRead addr:%d, power info:%s power idx:%d" % ( addr, str(info), idx))
                    pre_log_time = time.time()
        return "One address length rounds finished."

    def Test_SDV_PowerInfoReadLoop(self,log_freq=10, max_rounds=2000,max_time=60*60*4,power_idx=0):
        self.test_rounds_loop(max_rounds,max_time)(self.Test_SDV_PowerInfoRead,log_freq,power_idx)


    def Test_SDV_PowerInfoReadSingle(self):
        addr = 0
        leng = 128
        res = []
        for idx in range(2):
            try:
                info = self.sdviic.SDV_PowerInfoRead(addr, leng, idx)
            except Exception as error:
                self.logger.error("Test_SDV_PowerInfoRead FAIL. Error:%s",str(error))

            if info == StatusCode.FAIL:
                self.logger.error("Test_SDV_PowerInfoRead FAIL! addr:%d, length:%d",addr,leng)
            else:
                self.logger.debug("Test_SDV_PowerInfoRead addr:%d, length:%d, power info:%s power idx:%d" % (addr, leng,str(info), idx))
                info = StatusCode.SUCCESS
            res.append(info)   
        return res


    def Test_SDV_EEpromCfgReadWrite(self,addr,length):
        return self.rw_test(self.sdviic.SDV_EEpromCfgWrite,self.sdviic.SDV_EEpromCfgRead,(addr,length))
            
    def Test_SDV_EEpromCfgReadWriteRound(self,log_freq=10):
        max_scope = int(EEPROM_128KB_SIZE/2)
        res = self.test_addr_len_loop(max_scope,0,256)(self.Test_SDV_EEpromCfgReadWrite)
        return res

    def Test_SDV_EEpromCfgReadWriteLoop(self,log_freq=10,max_rounds = 1000, max_time=60*60*8):
        self.test_rounds_loop(max_rounds,max_time)(self.Test_SDV_EEpromCfgReadWriteRound,log_freq)


    def Test_SDV_EEpromIDReadWrite(self,addr,len_i):
        return self.rw_test(self.sdviic.SDV_EEpromIDWrite, self.sdviic.SDV_EEpromIDRead,(addr,len_i))
        
    def Test_SDV_EEpromIDReadWriteRound(self,log_freq=10):
        max_scope = EEPROM_2Kb_SIZE
        res = self.test_addr_len_loop(max_scope,0,1)(self.Test_SDV_EEpromIDReadWrite)
        return res

    def Test_SDV_EEpromIDReadWriteLoop(self,log_freq=10,max_rounds = 1000, max_time=60*60*8):
        self.test_rounds_loop(max_rounds,max_time)(self.Test_SDV_EEpromIDReadWriteRound,log_freq)

    def Test_SDV_EEpromFanReadWrite(self,addr,len_i,fan_i):
        online = self.testsdvcpld.Test_CPLDFanGetOnline()
        if StatusCode.FAIL == online:
            return online
        elif online[fan_i] == 0: 
            return self.rw_test(self.sdviic.SDV_EEpromFanWrite,self.sdviic.SDV_EEpromFanRead,(addr, len_i, fan_i))
        else:
            return StatusCode.SUCCESS
           
    def Test_SDV_EEpromFanReadWriteRound(self,fan_idx, log_freq=10):
        max_scop = EEPROM_128KB_SIZE
        res = self.test_addr_len_loop(max_scop,0,256)(self.Test_SDV_EEpromFanReadWrite,fan_idx)
        return res
        
    def Test_SDV_EEpromFanReadWriteLoop(self,fan_idx, log_freq=10, max_rounds = 1000, max_time=60*60*8):
        self.test_rounds_loop(max_rounds, max_time)(self.Test_SDV_EEpromFanReadWriteRound,fan_idx,log_freq)    
        
    def Test_SDV_EEpromTDCMReadWrite(self, addr, len_i):
        res = self.rw_test(self.sdviic.SDV_EEpromTDCMWrite, self.sdviic.SDV_EEpromTDCMRead, (addr, len_i))
        return res
    
    def Test_SDV_EEpromTDCMReadWriteRound(self,log_freq=10):
        max_scop = 512 
        res = self.test_addr_len_loop(max_scop,0,256)(self.Test_SDV_EEpromTDCMReadWrite)
        return res
    
    def Test_SDV_EEpromTDCMReadWriteLoop(self,log_freq=10,max_rounds = 1000, max_time = 60*60*8):
        self.test_rounds_loop(max_rounds, max_time)(self.Test_SDV_EEpromTDCMReadWriteRound,log_freq)
     
    
 

    def Test_SDV_EEpromPAM4ReadWrite(self, addr, len_i):
        res = self.rw_test(self.sdviic.SDV_EEpromPAM4Write, self.sdviic.SDV_EEpromPAM4Read,(addr,len_i))
        return res

    def Test_SDV_EEpromPAM4ReadWriteRound(self, log_freq=10):
        max_scop = 512
        res = self.test_addr_len_loop(max_scop,0,256)(self.Test_SDV_EEpromPAM4ReadWrite)
        return res
 
    def Test_SDV_EEpromPAM4ReadWriteLoop(self,log_freq=10,max_rounds = 1000, max_time = 60*60*8):
        self.test_rounds_loop(max_rounds, max_time)(self.Test_SDV_EEpromPAM4ReadWriteRound, log_freq)

#unittest.skip("Only manual test")        
class UnitTestSDVIIC(unittest.TestCase):
    
    def __init__(self,methodName='runTest',frequency=100):
        super(UnitTestSDVIIC,self).__init__(methodName)
        self.iic_freq = frequency

    @classmethod
    def setUpClass(cls):
        pass
       
    def setUp(self):
        self.logger = logging.getLogger("IIC")
        self.logger.setLevel(logging.INFO)
        self.logger.debug("Start...")
        self.testsdviic = TestSDVIIC(self.logger)
 
        #self.logger = UnitTestSDVIIC.logger
        #self.logger.setLevel(self.log_level)
        #self.testsdviic = UnitTestSDVIIC.testsdviic
        res = self.testsdviic.Test_SusiI2CSetFrequency(self.iic_freq)
        self.assertEqual(res, StatusCode.SUCCESS)
    
    def test_Test_SDV_ReadBoardTemp(self):
        res = self.testsdviic.Test_SDV_ReadBoardTemp()
        self.assertNotEqual(res,StatusCode.FAIL)

        
    @unittest.skip("Skip")    
    def test_Test_SDV_PowerInfoReadSingle(self):
        res = self.testsdviic.Test_SDV_PowerInfoReadSingle()
        self.assertEqual(res,[StatusCode.SUCCESS,StatusCode.SUCCESS])
        
    #@unittest.skip("Skip")
    def test_Test_SDV_EEpromIDReadWrite(self):
        res = self.testsdviic.Test_SDV_EEpromIDReadWrite(0,16)
        self.assertEqual(res, StatusCode.SUCCESS)

    #@unittest.skip("Skip")    
    def test_Test_SDV_EEpromCfgReadWrite(self):
        res = self.testsdviic.Test_SDV_EEpromCfgReadWrite(0,512)
        self.assertEqual(res,StatusCode.SUCCESS)

    #@unittest.skip("Skip")    
    def test_Test_SDV_EEpromFanReadWrite(self):
        for fan_i in range(FAN3_IDX+1):
            res = self.testsdviic.Test_SDV_EEpromFanReadWrite(0, 128,fan_i)
            self.assertEqual(res, StatusCode.SUCCESS)

    @unittest.skip("Skip")    
    def test_Test_SDV_EEpromTDCMReadWrite(self):
        # length bigger than 256
        res = self.testsdviic.Test_SDV_EEpromTDCMReadWrite(0,512)
        self.assertEqual(res, StatusCode.SUCCESS)
    
    @unittest.skip("Skip")    
    def test_Test_SDV_EEpromPAM4ReadWrite(self):
        # length bigger than 256
        res = self.testsdviic.Test_SDV_EEpromPAM4ReadWrite(0,512)
        self.assertEqual(res, StatusCode.SUCCESS)

def edvt_sdv_iic_suite():
    '''
    Choose some of the unit test case to do the EDVT test.
    '''
    edvt_iic_freq = 400
    #iic_test = UnitTestSDVIIC(frequency=edvt_iic_freq)
    #UnitTestSDVIIC.setUpClass()
    #iic_test.setUp()
    #iic_test.test_Test_SDV_ReadBoardTemp()
    #iic_test.test_Test_SDV_EEpromIDReadWrite()
    #iic_test.test_Test_SDV_EEpromCfgReadWrite()
    suit = unittest.TestSuite()
    suit.addTest(UnitTestSDVIIC('test_Test_SDV_ReadBoardTemp',edvt_iic_freq))
    suit.addTest(UnitTestSDVIIC('test_Test_SDV_EEpromIDReadWrite',edvt_iic_freq))
    suit.addTest(UnitTestSDVIIC('test_Test_SDV_EEpromCfgReadWrite',edvt_iic_freq))
    #suit.addTest(UnitTestSDVIIC('test_Test_SDV_EEpromFanReadWrite',edvt_iic_freq))
    #suit.addTest(UnitTestSDVIIC('test_Test_SDV_EEpromTDCMReadWrite',edvt_iic_freq))
    #suit.addTest(UnitTestSDVIIC('test_Test_SDV_EEpromPAM4ReadWrite',edvt_iic_freq))
    return suit

class MultiTestSDVIIC(unittest.TestCase):
    def __init__(self,methodName='runTest',max_rounds=0,max_time=100*3600):
        super(MultiTestSDVIIC,self).__init__(methodName)
        self.log_level = logging.INFO
        self.max_time = max_time
        self.max_rounds = max_rounds
        self.mask_round = 0
        self.one_round = 1
        self.iic_freqs = (100,400)

    @classmethod
    def setUpClass(cls):
        cls.logger = logging.getLogger("IIC")
        cls.logger.setLevel(logging.INFO)
        cls.testsdviic = TestSDVIIC(cls.logger)
        
    def setUp(self):
        pass
        self.logger = MultiTestSDVIIC.logger
        self.logger.setLevel(self.log_level)
        self.testsdviic = MultiTestSDVIIC.testsdviic
        
    def instance_obj(self,func, *args, **kwargs):
        while True:
            res = func(*args, **kwargs)
            cur_th_name = threading.current_thread().name
            cur_pr_name = multiprocessing.current_process().name
            if 'MainThread' == cur_th_name:
                cur_name = cur_pr_name
            else:
                cur_name = cur_th_name
                
            if cur_name in self.all_rounds:
                self.all_rounds[cur_name]+=1
            else:
                self.all_rounds[cur_name]=1
            
            #self.logger.info(self.all_rounds)
            if len(self.all_rounds) == len(self.all_instance):
                return res

    def Test_SDV_ReadBoardTempLoop(self):
        return self.instance_obj(self.testsdviic.Test_SDV_ReadBoardTempLoop,self.log_freq, 100,0)

        
    def Test_SDV_EEpromCfgReadWriteLoop(self):
        return self.instance_obj(self.testsdviic.Test_SDV_EEpromCfgReadWriteLoop,self.log_freq,1,0)
    def Test_SDV_EEpromIDReadWriteLoop(self):
        return self.instance_obj(self.testsdviic.Test_SDV_EEpromIDReadWriteLoop,self.log_freq,1,0)
    def get_all_loop_funcs(self,ins_type=None): 
        '''
        Need to change IIC clock, so run them one round then terminate to change clock.
        '''
        mask_round = 1
        self.run_methods = [
                            {"Func":"Test_SDV_ReadBoardTempLoop", "args":()},
                            {"Func":"Test_SDV_EEpromCfgReadWriteLoop", "args":()},
                            {"Func":"Test_SDV_EEpromIDReadWriteLoop", "args":()}
                        ]
        self.powerinfo_test_func=[{"Func":"Test_SDV_EEpromPAM4ReadWriteLoop", "args":(self.log_freq,mask_round,self.max_time,0)},
                                  {"Func":"Test_SDV_EEpromTDCMReadWriteLoop", "args":(self.log_freq,mask_round,self.max_time,0)},
                                  ]
        self.powerinfo_test_func=[{"Func":"Test_SDV_PowerInfoReadLoop", "args":(self.log_freq,mask_round,self.max_time,0)},
                                  {"Func":"Test_SDV_PowerInfoReadLoop", "args":(self.log_freq,mask_round,self.max_time,1)}]
        self.fan_test_func=[
                        {"Func":"Test_SDV_EEpromFanReadWriteLoop", "args":(0,self.log_freq,mask_round,self.max_time)},
                        {"Func":"Test_SDV_EEpromFanReadWriteLoop", "args":(1,self.log_freq,mask_round,self.max_time)},
                        {"Func":"Test_SDV_EEpromFanReadWriteLoop", "args":(2,self.log_freq,mask_round,self.max_time)},
                        {"Func":"Test_SDV_EEpromFanReadWriteLoop", "args":(3,self.log_freq,mask_round,self.max_time)}
                        ]
        test_funcs = self.run_methods
        #test_funcs.extend(self.fan_test_func)
        return test_funcs


    def test_multi(self): 
        self.testsdviic = MultiTestSDVIIC.testsdviic
        self.log_freq = 10
        run_methods = self.get_all_loop_funcs()
        start_time = time.time()
        end_time   = start_time + self.max_time
        rounds = 0
        while time.time()<end_time:
            if self.max_rounds !=0 and rounds>self.max_rounds:
                break
            rounds += 1
            for freq in self.iic_freqs:
                res = self.testsdviic.Test_SusiI2CSetFrequency(freq)
                self.logger.info("IIC freqency:%d KHz"%(freq))
                self.sdv_iic_threads = MultiThread(self,run_methods,self.logger)
                self.sdv_iic_processes = MultiProcess(self,self.run_methods,self.logger)
                prs = self.sdv_iic_processes.create_processes()
                self.all_instance = prs
                self.all_rounds= multiprocessing.Manager().dict()
                self.sdv_iic_processes.run_processes()
                #ths = self.sdv_iic_threads.create_threads()
                #self.all_rounds= multiprocessing.Manager().dict()
                #self.all_instance = ths
                #self.sdv_iic_threads.run_threads()
                   

def sst_iic_run(max_rounds,max_time):
    iic_test = MultiTestSDVIIC(max_rounds=max_rounds,max_time=max_time)
    MultiTestSDVIIC.setUpClass()
    iic_test.setUp()
    pr = multiprocessing.Process(target=iic_test.test_multi,args=())
    pr.start()

if __name__ == "__main__":
    #UnitTestSDVIIC.main() 
    create_oplogger()
    edvt_iic_suit = edvt_sdv_iic_suite()
    #sst_iic_suit = sst_sdv_iic_suite(3600)
    unittest.TextTestRunner(verbosity=2).run(edvt_iic_suit)
    #unittest.main()


