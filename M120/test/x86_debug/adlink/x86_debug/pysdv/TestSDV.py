import time
import logging
import random
import math
import binascii
from util import *


class TestSDV(object):
    patterns = [0xAA,0x55,0xFF,0x00,0x5A,0xA5,0x05,0x50,0xA0,0x0A,0x0F,0xF0]
    def __init__(self,logger=None,log_freq=10):
        if not logger:
            self.logger = logging.getLogger("TESTSDV")
        else:
            self.logger = logger
        self.log_freq = log_freq

    def generate_random_patterns(self,length):
        data = []
        for i in range(int(length)):
            data.append(random.choice(TestSDV.patterns))
        #self.logger.info("Random data:%s"%(str(data)))
        return data

    def generate_lengthes(self,max_len):
        '''
        lengthes are 0,1,3,4,5,...,2^n-1,2^n,2^n+1,max_len-1,max_len
        '''
        lens = [0]
        if max_len<1:
            return lens
        max_p_i = int(math.log2(max_len))
        for p_i in range(0, max_p_i):
            len_i = int(math.pow(2,p_i))
            if len_i > 2:
                lens.append(len_i-1)
            lens.append(len_i)
            lens.append(len_i+1)
        lens.append(max_len-1)
        lens.append(max_len)
        return lens
   
    def generate_address_max_len(self,length):
        addr_max_lens = []
        addrs = self.generate_lengthes(length)
        for addr in addrs:
            #For all address
            addr_max_lens.append([addr, length - addr])
        return addr_max_lens
    
    def _2hex_martrix(self, data, start_addr=0):
        data = bytearray(data)
        hex_data = binascii.hexlify(data)
        hex_mar = ""
        row_len = 32
        row_num = math.ceil(len(hex_data)/row_len)
        for idx in range(row_num):
            hex_row = str(hex_data[idx*row_len:idx*row_len+row_len]) 
            hex_row = ' '.join(hex_row[i:i+2] for i in range(2, len(hex_row),2))
            hex_mar += str(hex(int((start_addr+idx*row_len)/2)))+"  "+ hex_row + "\n"
        return hex_mar

    def _print_rw_data(self,written,read,not_match_idx,extra):
        print_len = 256
        if len(written)<print_len:
            self.logger.error("Written data:\n%s"%(self._2hex_martrix(written, 0)))
            self.logger.error("Read data:\n%s"%(self._2hex_martrix(read, 0)))
            self.logger.error("Read write data not match expect:0x%x, but found:0x%x at %d,Extra info:%s"%(written[not_match_idx],read[not_match_idx],not_match_idx,str(extra)))
        else :
            start_addr = not_match_idx - (not_match_idx%print_len+print_len)
            end_addr = not_match_idx +  (not_match_idx%print_len+print_len)
            if start_addr < 0:
                start_addr = 0
            if end_addr > len(written):
                end_addr = len(written)
            self.logger.error("Written data:\n%s"%(self._2hex_martrix(written[start_addr:end_addr], start_addr)))
            self.logger.error("Read data:\n%s"%(self._2hex_martrix(read[start_addr:end_addr], start_addr)))
            self.logger.error("Read write data not match expect:0x%x, but found:0x%x at %d,Extra info:%s"%(written[not_match_idx],read[not_match_idx],not_match_idx,str(extra)))
            
    def compare_rw_data(self,written,read,extra=None):
        try:
            data_len = 0
            if len(written) > len(read):
                data_len = len(read)
                self.logger.error("Read write data not match written length:%d,read length:%d,Extra info:%s"%(len(written),len(read),str(extra)))
            else:
                data_len = len(written)
            for i in range(data_len):
                if not written[i]==read[i]:
                    self._print_rw_data(written, read, i,extra)
                    return  StatusCode.RW_NOT_MATCH
            return StatusCode.SUCCESS
        except Exception as error:
            self.logger.exception("compare_rw_data error:%s. Extra:%s"%(str(error),str(extra)))
            return StatusCode.FAIL
    
    def rw_test(self,write_method, read_method, read_args,*args):
        '''
        The write_method must be: write(address, data, [device_idx])
        The read_method must be:  read(address, length, [device_idx])
        The device_idx can be omitted. 
        The read_args are, address, length, [device_idx]
        '''
        args_len = len(read_args)
        if args_len<2 or args_len >3:
            self.logger.exception("Wrong parameter")
            return StatuCode.FAIL
        elif args_len == 3:
            device_idx = read_args[2]
        else:
            device_idx = None
        addr = read_args[0]
        length = read_args[1]
        
        data = self.generate_random_patterns(length)
        #print("write length",length,len(data))
        #print(write_method.__name__,read_method.__name__,self._2hex_martrix(data))
        self.logger.debug("FuncW:%s,FuncR:%s,addr:%d,len:%d,device_idx:%s"
                                                 %(write_method.__name__,read_method.__name__,addr,length,str(device_idx)))
        if 3 == args_len:
            res = write_method(addr,data,device_idx)
        else:
            res = write_method(addr, data)
        if res != StatusCode.FAIL:
            if 3 == args_len:
                res = read_method(addr, length, device_idx)
            else:
                res = read_method(addr, length)
            if StatusCode.FAIL == res:
                self.logger.error("Read FAIL! res:%s",res)
                self.logger.error("FuncW:%s,FuncR:%s,addr:%d,len:%d,device_idx:%s"
                                                 %(write_method.__name__,read_method.__name__,addr,length,str(device_idx)))
                return StatusCode.READ_FAIL
            else:
                comp_res = self.compare_rw_data(data, res)
                if StatusCode.SUCCESS == comp_res:
                    self.logger.debug("Read Write data match!")
                    return comp_res
                else:
                    self.logger.error("FAIL %s",str(comp_res))
                    self.logger.error("FuncW:%s,FuncR:%s,addr:%d,len:%d,device_idx:%s"
                                                 %(write_method.__name__,read_method.__name__,addr,length,str(device_idx)))
                    return comp_res
        else:
            self.logger.error("FuncW:%s,FuncR:%s,addr:%d,len:%d,device_idx:%s"
                                                 %(write_method.__name__,read_method.__name__,addr,length,str(device_idx)))

            self.logger.error("Write FAIL! res:%s", res)
            return StatusCode.WRITE_FAIL


    
        
        

    def check_res(self,res,func="", extra=""):
        if res:
            return "%s SUCCESS with %s"%(str(func),str(extra))
        else:
            return "%s FAIL with %s"%(str(func),str(extra))
    
    def test_rounds_loop(self,max_rounds=0, max_time=60*60*8):
        '''
        If max_rounds is 0, there is no rounds limitation.
        '''
        def rounds_loop_func(func,*args,**kargs):
    
            is_fail = False 
            start_time = time.time()
            end_time = max_time + start_time
            pre_log_time = time.time()
            idx = 0
            while (idx < max_rounds) or (not max_rounds):
                idx += 1
                #paras.append(idx)
                res = func(*args,**kargs)
                if StatusCode.FAIL == res:
                    is_fail = True
                if time.time() - pre_log_time > self.log_freq:
                    self.logger.info("%s,Rounds:%d,%s"%(func.__name__,idx,res))
                    pre_log_time = time.time()
                if time.time()  > end_time:
                    break
            if is_fail:
                res = StatusCode.FAIL
            else:
                res = StatusCode.SUCCESS
            if time.time() - pre_log_time > self.log_freq:
                self.logger.info("%s,Total rounds:%d,status:%s"%(func.__name__,idx,str(res)))
            return res

        return rounds_loop_func
    def test_addr_len_loop(self,length,start_addr=0,addr_align=1):
        '''
        addr_align mean the address alignment. 
        If addr_align==0,the start address fixed to the start address.
        If addr_align==1,the address can be any address.
        If addr_align>1, the start address must align to the addr_align.
        '''
        def addr_len_loop(func,*args,**kargs):
            if 0 == addr_align:
                addrs_max_lens = [(0,length)]
            else:
                addrs_max_lens = self.generate_address_max_len(length)
            ###DEBUG##############
            #addrs_max_lens = [(0,length)]
            ###########################
            total_rounds = 0
            write_fail = 0
            read_fail = 0
            rw_not_match = 0
            success = 0
            start_time = time.time()
            pre_log_time = time.time()
            is_logging = False
            for ele in addrs_max_lens:
                addr = ele[0]
                if addr_align>1 and addr%addr_align != 0:
                    continue 
                max_len = ele[1]
                lens = self.generate_lengthes(max_len)
                ##DEBUG############
                #lens=[max_len]
                ##################
                addr += start_addr
                for len_i in lens:
                    res = func(addr,len_i,*args,**kargs) 
                    if StatusCode.SUCCESS == res:
                        success += 1
                    if StatusCode.WRITE_FAIL == res:
                        write_fail += 1
                        is_logging = True
                    elif StatusCode.READ_FAIL == res:
                        read_fail += 1
                        is_logging = True
                    elif StatusCode.RW_NOT_MATCH == res:
                        rw_not_match += 1
                        is_logging = True
                    total_rounds = success + write_fail + read_fail + rw_not_match
                    if is_logging:
                        self.logger.warning("%s,current addr:%d,current length:%d"%(func.__name__,addr,len_i)) 
                        self.logger.warning("Total rounds:%d, write fail:%d, read fail:%d, read write not match:%d"%(total_rounds,write_fail,read_fail,rw_not_match))
                    elif time.time() - pre_log_time > self.log_freq:
                        self.logger.info("%s,current addr:%d, current length:%d"%(func.__name__,addr,len_i))
                        self.logger.info("Total rounds:%d, write fail:%d, read fail:%d, read write not match:%d"%(total_rounds,write_fail,read_fail,rw_not_match))
                        pre_log_time = time.time()
                    is_logging = False
            if time.time() - pre_log_time > self.log_freq:
                self.logger.info("%s,One address,length rounds finished. Total rounds:%d, write fail:%d, read fail:%d, read write not match:%d"%(func.__name__,total_rounds,write_fail,read_fail,rw_not_match))

            if write_fail or read_fail or rw_not_match:
                return StatusCode.FAIL
            else:
                return StatusCode.SUCCESS
        return addr_len_loop

if __name__ == '__main__':
    pass
    testsdv = TestSDV()
    data = TestSDV.patterns*11