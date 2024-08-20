
import struct
import time
import unittest
import random
import multiprocessing
from OPLogManager import *
from sdv import *
from util import *
from TestSDV import *
from MultiProcessTest import *



class TestSDVCPLD(TestSDV):
    patterns = [0xAA,0xFF,0x55,0xAA,0x5A,0xA5,0x05,0x50,0xA0,0x0A,0x0F,0xF0,0x0]
    CPLD0Start = 0
    CPLD0Len   = 0x8000
    CPLD1Start = 0x8000
    CPLD1Len   = 0x8000
    CPLD_Len   = 0x10000
    CPLD0Interface = [{"offset":0x0,   "length":0x1,   "Reset":0xC3, "mask":0xFF,   "Description":""},
                      {"offset":0x1,   "length":0x1,   "Reset":0x0,  "mask":0xFF,  "Description":""},
                      {"offset":0x2,   "length":0x1,   "Reset":0x0,  "mask":0x0,  "Description":""},
                      {"offset":0x3,   "length":0x1,   "Reset":0x1,  "mask":0xF,  "Description":""},
                      {"offset":0x4,   "length":0x1,   "Reset":0x0,  "mask":0xFF,  "Description":""},
                      {"offset":0x5,   "length":0x1,   "Reset":0x0,  "mask":0xFF,  "Description":""},
                      {"offset":0x20,   "length":0x1,   "Reset":0x26,"mask":0x37,    "Description":""},
                      {"offset":0x28,   "length":0x1,   "Reset":0x3, "mask":0x1F,   "Description":""},
                      {"offset":0x29,   "length":0x1,   "Reset":0x0, "mask":0x7F,   "Description":""},
                      {"offset":0x2C,   "length":0x1,   "Reset":0x3, "mask":0xF,   "Description":""},
                      {"offset":0x30,   "length":0x1,   "Reset":0x0, "mask":0x11,   "Description":""},
                      {"offset":0x31,   "length":0x1,   "Reset":0x0, "mask":0x41,   "Description":""},
                      {"offset":0x34,   "length":0x1,   "Reset":0x8, "mask":0x3E,   "Description":""},
                      {"offset":0x35,   "length":0x1,   "Reset":0xF, "mask":0xF,   "Description":""},
                      {"offset":0x38,   "length":0x1,   "Reset":0x5, "mask":0x7,   "Description":""},
                      {"offset":0x3D,   "length":0x1,   "Reset":0x6, "mask":0xFF,   "Description":""},
                      {"offset":0x3E,   "length":0x1,   "Reset":0x2, "mask":0xFF,   "Description":""},
                      {"offset":0x3F,   "length":0x1,   "Reset":0x0, "mask":0xFF,   "Description":""},
                      {"offset":0x40,   "length":0x1,   "Reset":0x0, "mask":0x0,   "Description":""},
                      {"offset":0x48,   "length":0x1,   "Reset":0xC0, "mask":0xC0,   "Description":""}]

    CPLD1Interface = [{"offset":0x0,   "length":0x1,   "Reset":0xC3, "mask":0xFF,  "Description":"CHIP ID"},
                      {"offset":0x1,   "length":0x1,   "Reset":0x1,  "mask":0xFF,  "Description":"CHIP ID"},
                      {"offset":0x2,   "length":0x1,   "Reset":0x0,  "mask":0x0,  "Description":""},
                      {"offset":0x3,   "length":0x1,   "Reset":0x1,  "mask":0xFF,  "Description":""},
                      {"offset":0x4,   "length":0x1,   "Reset":0x0,  "mask":0xFF,  "Description":""},
                      {"offset":0x5,   "length":0x1,   "Reset":0x0,  "mask":0xFF,  "Description":""},
                      {"offset":0x10,  "length":0x1,   "Reset":0x0,  "mask":0x1,  "Description":""},
                      {"offset":0x11,  "length":0x1,   "Reset":0x0,  "mask":0x1,  "Description":""},
                      {"offset":0x12,  "length":0x1,   "Reset":0x0,  "mask":0xFF,  "Description":""},
                      {"offset":0x20,  "length":0x1,   "Reset":0x26, "mask":0x37,  "Description":""},
                      {"offset":0x28,  "length":0x1,    "Reset":0x6, "mask":0xF,  "Description":""},
                      {"offset":0x29,  "length":0x1,   "Reset":0x1F, "mask":0x1F,  "Description":""},
                      {"offset":0x30,  "length":0x1,   "Reset":0x0,  "mask":0x11,  "Description":""},
                      {"offset":0x34,  "length":0x1,   "Reset":0x0,  "mask":0xC1,  "Description":""},
                      {"offset":0x36,  "length":0x1,   "Reset":0x0,  "mask":0x0,  "Description":""},
                      {"offset":0x38,  "length":0x1,   "Reset":0x7,  "mask":0x87,  "Description":""},
                      {"offset":0x39,  "length":0x1,   "Reset":0x7,  "mask":0x7,  "Description":""},
                      {"offset":0x40,  "length":0x1,   "Reset":0x81, "mask":0x81,  "Description":""},
                      {"offset":0x42,  "length":0x1,   "Reset":0x1,  "mask":0x33,  "Description":""},
                      {"offset":0x44,   "length":0x1,   "Reset":0x0, "mask":0x0,   "Description":""},
                      {"offset":0x48,   "length":0x1,   "Reset":0x0, "mask":0x3,   "Description":""},
                      {"offset":0x80,   "length":0x1,   "Reset":0x1, "mask":0xFF,   "Description":""},
                      {"offset":0x81,   "length":0x1,   "Reset":0x0, "mask":0x3,   "Description":""},
                      {"offset":0x82,   "length":0x1,   "Reset":0x0, "mask":0xF,   "Description":""},
                      {"offset":0x83,   "length":0x1,   "Reset":0x0, "mask":0xF,   "Description":""},
                      {"offset":0x84,   "length":0x1,   "Reset":0x0, "mask":0xFF,   "Description":""},
                      {"offset":0x85,   "length":0x1,   "Reset":0x0, "mask":0xFF,   "Description":""},
                      {"offset":0x86,   "length":0x1,   "Reset":0x0, "mask":0xFF,   "Description":""},
                      {"offset":0x87,   "length":0x1,   "Reset":0x0, "mask":0xFF,   "Description":""},
                      {"offset":0x90,   "length":0x1,   "Reset":0x1, "mask":0xFF,   "Description":""},
                      {"offset":0x92,   "length":0x1,   "Reset":0x30,"mask":0xFF,    "Description":""},
                      {"offset":0x94,   "length":0x1,   "Reset":0x0, "mask":0x0,   "Description":""},
                      {"offset":0x96,   "length":0x1,   "Reset":0x0, "mask":0xFF,   "Description":""},
                      {"offset":0x97,   "length":0x1,   "Reset":0x0, "mask":0x0,   "Description":""}]

    CPLDFanInterface = [{"offset":0x0,   "length":0x1,   "Reset":0xC3, "mask":0xFF,  "Description":"CHIP ID"},
                        {"offset":0x1,   "length":0x1,   "Reset":0x2, "mask":0xFF,   "Description":""},
                        {"offset":0x2,   "length":0x1,   "Reset":0x0, "mask":0x0,   "Description":""},
                        {"offset":0x3,   "length":0x1,   "Reset":0x1, "mask":0xFF,   "Description":""},
                        {"offset":0x4,   "length":0x1,   "Reset":0x0, "mask":0x0,   "Description":""},
                        {"offset":0x5,   "length":0x1,   "Reset":0x0, "mask":0x0,   "Description":""},
                        {"offset":0x7,   "length":0x1,   "Reset":0x0, "mask":0xFF,   "Description":""},
                        {"offset":0xF,   "length":0x1,   "Reset":0x0, "mask":0x0,   "Description":""},
                        {"offset":0x10,   "length":0x1,   "Reset":0x50, "mask":0x0,   "Description":""},
                        {"offset":0x11,   "length":0x1,   "Reset":0x50, "mask":0x0,   "Description":""},
                        {"offset":0x12,   "length":0x1,   "Reset":0x50, "mask":0x0,   "Description":""},
                        {"offset":0x13,   "length":0x1,   "Reset":0x50, "mask":0x0,   "Description":""},
                        {"offset":0x14,   "length":0x1,   "Reset":0x0, "mask":0x0,   "Description":""},
                        {"offset":0x15,   "length":0x1,   "Reset":0x0, "mask":0x0,   "Description":""},
                        {"offset":0x16,   "length":0x1,   "Reset":0x0, "mask":0x0,   "Description":""},
                        {"offset":0x17,   "length":0x1,   "Reset":0x0, "mask":0x0,   "Description":""}]


    def __init__(self, logger=None):
        super(TestSDVCPLD,self).__init__(logger)
        self.libsdv = SDVCPLD(self.logger)
        self.fan_num=4 
     
    def Test_cpld_get_version(self):
        cpld0_ver_addr = [0x0, 0x1,0x2,0x3]
        cpld1_ver_addr = [TestSDVCPLD.CPLD1Start+ele for ele in cpld0_ver_addr]
        cpld0_ver = []
        cpld1_ver = []
        for ele in cpld0_ver_addr:
            read_value = self.libsdv.SDV_GetCpldValue(ele, 1)
            cpld0_ver.append(read_value)
        self.logger.debug("CPLD0 version:%s"%(str(cpld0_ver)))
        for ele in cpld1_ver_addr:
            read_value = self.libsdv.SDV_GetCpldValue(ele, 1)
            cpld1_ver.append(read_value)
        self.logger.debug("CPLD1 version:%s"%(str(cpld1_ver)))
        return (cpld0_ver,cpld1_ver)
         
    def Test_cpld_get_fan_version(self): 
        ver_addr = [0x0,0x1,0x2,0x3]
        ver = []
        for ele in ver_addr:
            read_value = self.libsdv.SDV_FanCpldRead(ele)
            ver.append(read_value)
        self.logger.debug("CPLD0 version:%s"%(str(ver)))
        return ver

    def Test_cpld_fan_default_value(self):
        isFail = False
        for idx, ele in enumerate(TestSDVCPLD.CPLDFanInterface):
            addr = ele["offset"]
            reset_value = ele["Reset"]
            mask = ele["mask"]
            read_value = self.libsdv.SDV_FanCpldRead(addr)
            if StatusCode.FAIL == read_value:
                self.logger.error("FAIL")
                continue
            if 0x0 == mask:
                continue
            if reset_value != (mask & read_value):
                self.logger.error("The read data is not match the default value! At addr: 0x%x, Expect: 0x%x, Read:0x%x, Masked read: 0x%x"%(addr,reset_value, read_value, read_value & mask))
                isFail = True
            else:
                self.logger.debug("Read data: 0x%x match the default at address: 0x%x"%(mask & read_value, addr))
        if isFail:
            return StatusCode.FAIL
        else:
            return StatusCode.SUCCESS
        
    
    
    def Test_cpld_default_value(self,base_addr,cpld_interface,log_freq=10):
        isFail = False
        pre_log_time = time.time()
        for idx, ele in enumerate(cpld_interface):
            addr = base_addr + ele["offset"]
            reset_value = ele["Reset"]
            mask = ele["mask"]
            read_value = self.libsdv.SDV_GetCpldValue(addr, 1)
            if StatusCode.FAIL != read_value:
                read_value = struct.unpack("B", read_value)[0]
            else:
                self.logger.error("FAIL")
                continue
            if reset_value !=( mask & read_value):
                self.logger.error("The read data is not match the default value! At addr: 0x%x, Expect: 0x%x, Read:0x%x, Masked read: 0x%x"%(addr,reset_value, read_value, read_value & mask))
                isFail = True
            else:
                if time.time() - pre_log_time>log_freq:
                    self.logger.debug("Read data: 0x%x match the default at address: 0x%x"%(mask & read_value, addr))
                    pre_log_time = time.time()
        if isFail:
            return StatusCode.FAIL
        else:
            return StatusCode.SUCCESS

    def Test_read_cpld_default(self,cpld_idx):
        if 0==cpld_idx:
            base_addr = TestSDVCPLD.CPLD0Start
            cpld_int = TestSDVCPLD.CPLD0Interface
        elif 1 == cpld_idx:
            base_addr = TestSDVCPLD.CPLD1Start
            cpld_int = TestSDVCPLD.CPLD1Interface
        status = self.Test_cpld_default_value(base_addr, cpld_int, 0)
        return status


    def Test_CPLDRAMReadWrite(self,addr,length):
        return self.rw_test(self.libsdv.SDV_SetCpldValue,self.libsdv.SDV_GetCpldValue,(addr,length))
    
    def Test_CPLDRAMReadWriteRound(self,start_addr, length, log_freq=10):
        return self.test_addr_len_loop(length,start_addr,1)(self.Test_CPLDRAMReadWrite)
        
    def Test_CPLDRAMReadWriteLoop(self,start_addr, length, log_freq=10,max_rounds = 1000, max_time = 60*60*8):    
        return self.test_rounds_loop(max_rounds, max_time)(self.Test_CPLDRAMReadWriteRound, start_addr, length,log_freq)
    
    def Test_TurnOffCPLDFans(self):
        fan_pwm_base = 0x10
        isFail = False
        for idx in range(self.fan_num):
            res = self.libsdv.SDV_FanCpldWrite(fan_pwm_base+idx,0)
            if StatusCode.SUCCESS != res:
                isFail = True
        if isFail:
            return StatusCode.FAIL
        else:
            return StatusCode.SUCCESS

    def Test_CPLDFanGetOnline(self):
        res = self.libsdv.SDV_FanCpldRead(0xF)
        if StatusCode.FAIL == res:
            self.logger.error("FAIL")
            return StatusCode.FAIL
        else:
            online = (res & 0x1, (res & 0x2)>>1, (res & 0x4)>>2, (res & 0x8)>>3)
            self.logger.debug(online)
            return online
        
    def Test_CPLDFanGetSpeed(self):
        speeds = []
        fan_speed_base = 0x14
        for i in range(self.fan_num):
            
            res = self.libsdv.SDV_FanCpldRead(fan_speed_base+i)
            if StatusCode.FAIL == res:
                self.logger.error("FAIL! fan id:%d"%(i))
                return StatusCode.FAIL
            else:
                speeds.append(res)
        self.logger.debug(speeds)
        return speeds
            
    def Test_FanCpldDutyReadTry(self,fan_id,try_loop=3):
        fan_pwm_base = 0x10
        prev_res = 0
        for idx in range(try_loop):
            res = self.libsdv.SDV_FanCpldRead(fan_pwm_base+fan_id)
            if StatusCode.FAIL == res:
                self.logger.error("FAIL")
                #return StatusCode.READ_FAIL
            elif idx == 0:
                prev_res = res
                continue
            elif res == prev_res:
                return res
        return res
             
            

    def Test_CPLDFanPWMCfg(self,fan_id):
        fan_pwm_base = 0x10
        fan_speed_base = 0x14
     
        duty = random.choice(range(100))
        res = self.libsdv.SDV_FanCpldWrite(fan_pwm_base+fan_id, duty)
        if StatusCode.SUCCESS != res:
            self.logger.error("FAIL")
            return StatusCode.WRITE_FAIL
        else:
            res = self.Test_FanCpldDutyReadTry(fan_id)
            if StatusCode.FAIL == res:
                self.logger.error("FAIL")
                return StatusCode.READ_FAIL
            #res = self.libsdv.SDV_FanCpldRead(fan_pwm_base+fan_id)
            else:
                if res != duty:
                    self.logger.error("FAIL! CPLD Fan PWM read write NOT match! FAN id:%d, duty:%d, read result:%s"%(fan_id, duty, str(res)))
                    return StatusCode.RW_NOT_MATCH
                else:
                    self.logger.debug("SUCCESS! CPLD Fan PWM read write match! FAN id:%d, duty:%d, read duty:%d"%(fan_id, duty, res))
                    onlines = self.Test_CPLDFanGetOnline()
                    if StatusCode.FAIL == onlines:
                        return StatusCode.FAIL
                    time.sleep(1)
                    speeds  = self.Test_CPLDFanGetSpeed()
                    if StatusCode.FAIL == speeds:
                        return StatusCode.FAIL
                    if not onlines[fan_id]:
                        if duty>0 and speeds[fan_id]>0 or duty==0:
                            self.logger.debug("SUCCESS! CPLD Fan PWM setting take effect.FAN id:%d, duty:%d, speed:%d"%(fan_id, duty,speeds[fan_id]))
                            return StatusCode.SUCCESS 
                        else:
                            self.logger.error("FAIL! CPLD Fan PWM setting not take effect.FAN id:%d, duty:%d, speed:%d"%(fan_id, duty,speeds[fan_id]))
                            return StatusCode.FAIL
                    return StatusCode.SUCCESS


    def Test_CPLDFanPWMCfgLoop(self,fan_id, log_freq=10,max_rounds=0, max_time=60*60*8):
        return self.test_rounds_loop(max_rounds, max_time)(self.Test_CPLDFanPWMCfg, fan_id)
    
    def Test_CPLDFanPWMCfgSeq(self): 
        is_fail = False
        for i in range(4):
            res = self.Test_CPLDFanPWMCfg(i)
            if res != StatusCode.SUCCESS:
                is_fail = True 
        if is_fail:
            return StatusCode.FAIL
        else:
            return StatusCode.SUCCESS
        

    def Test_CPLDFanPWMCfgSeqLoop(self,log_freq=10,max_rounds=0, max_time=60*60*8):
        return self.test_rounds_loop(max_rounds, max_time)(self.Test_CPLDFanPWMCfgSeq)
            

class UnittestSDVCPLD(unittest.TestCase):
    def __init__(self,methodName='runTest',max_rounds=1,max_time=3600):
        super(UnittestSDVCPLD,self).__init__(methodName)
        self.log_level = logging.INFO
        self.max_time = max_time
        self.max_rounds = max_rounds

    @classmethod
    def setUpClass(cls):
        cls.logger = logging.getLogger("CPLD")
        cls.logger.debug("Start...")
        cls.testsdvcpld = TestSDVCPLD(cls.logger)
     
    def setUp(self):
        self.logger = UnittestSDVCPLD.logger
        self.logger.setLevel(self.log_level)
        self.test_sdvcpld = UnittestSDVCPLD.testsdvcpld
        self.log_freq = 10
        self.cpld_ram_rw_process_funcs = [{"Func":"Test_CPLDRAMReadWriteLoop","args":(0xC000,0x1000,self.log_freq,self.max_rounds,self.max_time)},
                     {"Func":"Test_CPLDRAMReadWriteLoop","args":(0xD000,0x1000,self.log_freq,self.max_rounds,self.max_time)}]
        self.cpld_ram_rw_thread_funcs = [
                     {"Func":"Test_CPLDRAMReadWriteLoop","args":(0xE000,0x1000,self.log_freq,self.max_rounds,self.max_time)},
                     {"Func":"Test_CPLDRAMReadWriteLoop","args":(0xF000,0xFFF ,self.log_freq,self.max_rounds,self.max_time)}]

        self.cpld_fan_func=[{"Func":"Test_CPLDFanPWMCfgSeqLoop","args":(self.log_freq,self.max_rounds,self.max_time)}] 
        self.cpld_fan_pwm_funcs = [{"Func":"Test_CPLDFanPWMCfgLoop","args":(0,self.log_freq,self.max_rounds,self.max_time)},
                              {"Func":"Test_CPLDFanPWMCfgLoop","args":(1,self.log_freq,self.max_rounds,self.max_time)},
                              {"Func":"Test_CPLDFanPWMCfgLoop","args":(2,self.log_freq,self.max_rounds,self.max_time)},
                              {"Func":"Test_CPLDFanPWMCfgLoop","args":(3,self.log_freq,self.max_rounds,self.max_time)}]

    def get_all_loop_funcs(self, ins_type): 
        if ins_type == 'thread':
            cpld_funcs = self.cpld_ram_rw_thread_funcs
        elif ins_type == 'process':
            cpld_funcs = self.cpld_ram_rw_process_funcs
        #cpld_funcs.extend(self.cpld_fan_func)
        #self.logger.debug(cpld_funcs)
        return cpld_funcs

    def test_cpld_get_version(self):
        res = self.test_sdvcpld.Test_cpld_get_version()
        self.logger.info(res)
        
    def test_cpld_get_fan_version(self):
        res = self.test_sdvcpld.Test_cpld_get_fan_version()
        self.logger.info(res)

    #@unittest.skip("Skip")
    def test_read_cpld_default(self):
        res = self.test_sdvcpld.Test_read_cpld_default(0)
        self.assertEqual(res, StatusCode.SUCCESS)
        res = self.test_sdvcpld.Test_read_cpld_default(1)
        self.assertEqual(res, StatusCode.SUCCESS)
        
    @unittest.skip("Skip")    
    def test_Test_TurnOffCPLDFans(self):
        res = self.test_sdvcpld.Test_TurnOffCPLDFans()
        self.assertEqual(res, StatusCode.SUCCESS)

    @unittest.skip("Skip")    
    def test_cpld_fan_default(self):
        res = self.test_sdvcpld.Test_cpld_fan_default_value()
        self.assertEqual(res,StatusCode.SUCCESS)

    @unittest.skip("Skip")    
    def test_Test_CPLDFanGetOnline(self):
        res = self.test_sdvcpld.Test_CPLDFanGetOnline()
        self.assertNotEqual(res, StatusCode.FAIL)

    @unittest.skip("Skip")    
    def test_Test_CPLDFanPWMCfgSeqLoop(self):    
        res = self.test_sdvcpld.Test_CPLDFanPWMCfgSeqLoop()
        self.assertEqual(res, StatusCode.SUCCESS)

    @unittest.skip("Skip")    
    def test_Test_CPLDFanPWMCfg(self):
        res = self.test_sdvcpld.Test_CPLDFanPWMCfgSeq()
        self.assertEqual(res, StatusCode.SUCCESS)

    @unittest.skip("Skip")    
    def test_Test_CPLDFanGetSpeed(self):
        res= self.test_sdvcpld.Test_CPLDFanGetSpeed()
        self.assertNotEqual(res, StatusCode.FAIL)

    #@unittest.skip("Skip")
    def test_Test_CPLDRAMReadWrite(self):
        res = self.test_sdvcpld.Test_CPLDRAMReadWrite(0xC000, 0x3FFF)
        self.assertEqual(res, StatusCode.SUCCESS)

    @unittest.skip("Skip")
    def test_Test_CPLDRAMReadWriteRound(self):
        res = self.test_sdvcpld.Test_CPLDRAMReadWriteRound(0xc000,0x1000,0)
        self.assertEqual(res, StatusCode.SUCCESS)
        
    @unittest.skip("Skip")
    def test_cpld0_default_loop(self):
        self.test_sdvcpld.Test_cpld0_default_loop(self.log_freq,0,60*60*12)

    @unittest.skip("Skip")
    def test_cpld1_default_loop(self):
        self.test_sdvcpld.Test_cpld1_default_loop(self.log_freq,0,60*60*12)

    @unittest.skip("Skip")
    def test_cpld_default_multiprocess(self):
        cpld_funcs = [{"Func":"Test_cpld1_default_loop","args":(10,0,60*60*8)},
                      {"Func":"Test_cpld0_default_loop","args":(10,0,60*60*8)},
                      {"Func":"Test_cpld1_default_loop","args":(10,0,60*60*8)},
                      {"Func":"Test_cpld0_default_loop","args":(10,0,60*60*8)},
                      {"Func":"Test_cpld1_default_loop","args":(10,0,60*60*8)},
                      {"Func":"Test_cpld0_default_loop","args":(10,0,60*60*8)}]
        sdv_cpld_th = MultiProcess(self.test_sdvcpld,cpld_funcs)
        sdv_cpld_th.create_processes()
        sdv_cpld_th.run_processes()

    @unittest.skip("Skip")
    def test_cpld_ram_rw_multiprocess(self):
        cpld_funcs = self.get_all_loop_funcs()
        sdv_cpld_pr = MultiProcess(self.test_sdvcpld,cpld_funcs)
        sdv_cpld_pr.create_processes()
        sdv_cpld_pr.run_processes()

    @unittest.skip("Skip")
    def test_cpld_ram_rw_multithread(self):
        cpld_funcs = self.get_all_loop_funcs()
        sdv_cpld_th = MultiThread(self.test_sdvcpld,cpld_funcs)
        sdv_cpld_th.create_threads()
        sdv_cpld_th.run_threads()
    
    #@unittest.skip("Skip")
    def test_multi(self):
        cpld_pr_funcs = self.get_all_loop_funcs("process")
        cpld_th_funcs = self.get_all_loop_funcs("thread")
        start_time = time.time()
        end_time   = start_time + self.max_time
        while time.time()<end_time:
            sdv_cpld_pr = MultiProcess(self.test_sdvcpld,cpld_pr_funcs)
            sdv_cpld_th = MultiThread(self.test_sdvcpld,cpld_th_funcs)
            sdv_cpld_pr.create_processes()
            sdv_cpld_th.create_threads()
            instances = sdv_cpld_pr.Processes
            instances += sdv_cpld_th.Threads

            for ins in instances:
                ins.start()
            for ins in instances:
                ins.join()


def sst_cpld_run(max_rounds,max_time):
    cpld_test = UnittestSDVCPLD(max_rounds=max_rounds,max_time=max_time)
    UnittestSDVCPLD.setUpClass()
    cpld_test.setUp()
    pr = multiprocessing.Process(target=cpld_test.test_multi,args=())
    pr.start()

def edvt_cpld_suite():
    cpld_suit = unittest.TestSuite()
    cpld_suit.addTest(UnittestSDVCPLD("test_read_cpld_default"))
    cpld_suit.addTest(UnittestSDVCPLD("test_Test_CPLDRAMReadWrite"))
    return cpld_suit

if __name__ == "__main__":

    create_oplogger()
    unittest.main()
    #sdv_cpld = TestSDVCPLD()
    #sdv_cpld.Test_cpld_ram_rw_loop(0xC000,0x1000,10,0,60*60*12)
    #sdv_cpld.Test_cpld0_default_loop(10,0,60*60*12)
    #sdv_cpld.Test_cpld1_default_loop(10,0,60*60*12)





