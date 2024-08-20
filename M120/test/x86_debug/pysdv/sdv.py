import os
from enum import IntEnum
from util import *
import struct
import ctypes
import logging
from ctypes import *
import time
import inspect

'''
This is the wrapper for libsdv.so.
It provides the access interface of IIC/UART/CPLD to libsdv.so.
'''
FAN0_IDX = 0
FAN1_IDX = 1
FAN2_IDX = 2
FAN3_IDX = 3


POWER0_IDX = 0
POWER1_IDX = 1


OPLK_ERROR = -1
OPLK_OK = 0


EEPROM_128KB_SIZE = (128 * 1024)
EEPROM_64KB_SIZE = (64 * 1024)
EEPROM_2Kb_SIZE = (256)
I2C_MAX_LEN = (256)
EEPROM_128KB_PAGE_LEN   =  256

SUSI_ID_I2C_EXTERNAL	=	0

class SDV(object):
    '''
    This is the base class for other interface.
    It provide the logging, basic util interfaces.
    The base return value assumption: 0 is success. This is align to driver. 
    '''
    def __init__(self,logger=None):
        if not logger:
            self.logger = logging.getLogger("SDV")
        else:
            self.logger = logger
        #print(self.logger)
        self.libsdv_pathes = ["libsdv.so","/lib64/libsdv.so", "lib/libsdv.so"]
        self.libSUSI_pathes= ["/lib64/libSUSI-4.00.so.1"]
        self.libsdv_path = None
        self.libSUSI_path = None
        for ele in self.libsdv_pathes:
            if os.path.exists(ele):
                self.libsdv_path = ele
                break
            
        for ele in self.libSUSI_pathes:
            if os.path.exists(ele):
                self.libSUSI_path = ele
        if not self.libsdv_path:
            self.libsdv_path = util.find_library("sdv")
        if not self.libsdv_path:
            self.logger.exception("Can't find libsdv.so")
            raise
        if not self.libSUSI_path:
            self.logger.exception("Can't find libSUSI")
        #print(self.libsdv_path)
        self.logger.debug("libsdv path:%s" % (self.libsdv_path))
        self.libsdv = cdll.LoadLibrary(self.libsdv_path)
        self.libsusi = cdll.LoadLibrary(self.libSUSI_path)
        self.logger.debug("libsdv:%s" % (self.libsdv))
        
    def get_all_members(self):
       all_mem = inspect.getmembers(self.libsdv) 
       print(all_mem)

    def byte_2_uchar(self,byte_value):
        return struct.unpack("B", byte_value)[0]

    def print_exceptoin(self,func_name,error):
        '''
        Make sure the error log is fix.
        This is usable to find the error log after test.
        '''
        self.logger.exception("%s FAIL error:%s"%(func_name, str(error)))


    def check_driver_res(self,res,func_name):
        '''
        Make sure the error log is fix.
        This is usable to find the error log after test.
        '''
        self.logger.debug("%s, res:%s"%(func_name,str(res)))
        if OPLK_ERROR == res:
            self.logger.error("%s, res:%s"%(func_name,str(res)))
            return StatusCode.FAIL
        else:
            if OPLK_OK == res:
                return StatusCode.SUCCESS
            else:
                return res


    def check_read_res(self,res,read_buf,func_name):
        status = self.check_driver_res(res,func_name)
        if StatusCode.SUCCESS == status:
            readed_info = read_buf.raw
            return readed_info
        elif StatusCode.FAIL == status:
            return StatusCode.FAIL
        else:
            readed_info = read_buf.raw
            if res == len(readed_info):
                return readed_info
            return status

    def check_write_res(self,res,func_name):
        return self.check_driver_res(res, func_name)
    
    def drv_prototype(self,obj,func_name):
        try:
            if hasattr(obj, func_name):
                res = getattr(obj, func_name)()
                return self.check_driver_res(res, func_name)
            else:
                self.logger.error("%s don't have function :%s"%(str(obj),func_name))
                return StatusCode.FAIL
        except Exception as error:
            self.print_exceptoin(func_name,error)
            return StatusCode.FAIL
    
    def sdv_drv_prototype(self,func_name):
        return self.drv_prototype(self.libsdv, func_name)

        

class SDVCPLD(SDV):
    def __init__(self,logger=None):
        super(SDVCPLD,self).__init__(logger)
        self.SDV_CpldI2cInit()

    def __del__(self):
        #self.SDV_CpldI2cClose()
        pass
        
    def SDV_CpldI2cInit(self):
        return self.sdv_drv_prototype("SDV_CpldI2cInit")
    
    def SDV_CpldI2cClose(self):
        return self.sdv_drv_prototype("SDV_CpldI2cClose")
    
    def SDV_GetCpldValue(self,addr,read_len):
        try:
            ulAddress = c_ulong(addr)
            ulReadLen = c_ulong(read_len)
            buf_len = read_len
            if buf_len == 0:
                buf_len = 1
            pucBuf = create_string_buffer(buf_len)
            res = self.libsdv.SDV_GetCpldValue(ulAddress,ulReadLen,pucBuf)
            return self.check_read_res(res,pucBuf,"SDV_GetCpldValue")
        except Exception as error:
            self.print_exceptoin("SDV_GetCpldValue",error)
            return StatusCode.FAIL

    def SDV_SetCpldValue(self,addr,data):
        try:
            ulAddress = c_ulong(addr)
            data_len = len(data)
            ulWriteLen = c_ulong(data_len)
            if data_len == 0:
                data_len = 1
            pucBuf = create_string_buffer(data_len)
            for i in range(len(data)):
                pucBuf[i]=data[i]
            res = self.libsdv.SDV_SetCpldValue(ulAddress,ulWriteLen,pucBuf)
            return self.check_write_res(res,"SDV_SetCpldValue")
        except Exception as error:
            self.print_exceptoin("SDV_SetCpldValue",error)
            return StatusCode.FAIL 

    def SDV_FanCpldRead(self,addr):
        try:
            ulAddress = c_ulong(addr)
            pucBuf = c_char(0)
            res = self.libsdv.SDV_FanCpldRead(ulAddress,pointer(pucBuf))
            status = self.check_driver_res(res,"SDV_FanCpldRead")
            if StatusCode.SUCCESS == status:
                read_value = pucBuf.value
                read_value = struct.unpack("B", read_value)[0]
                return read_value
            else:
                return status
        except Exception as error:
            self.print_exceptoin("SDV_FanCpldRead",error)
            return StatusCode.FAIL

    def SDV_FanCpldWrite(self,addr, data):
        try:
            ulAddress = c_ulong(addr)
            pucBuf = c_char(data)
            res = self.libsdv.SDV_FanCpldWrite(ulAddress,pointer(pucBuf))
            return self.check_write_res(res,"SDV_FanCpldWrite")
        except Exception as error:
            self.print_exceptoin("SDV_FanCpldWrite",error)
            return StatusCode.FAIL


       
class SDVUART(SDV):
    class UARTCHOOSE(IntEnum):
        CPLD_UART_EDFA1 = 0
        CPLD_UART_EDFA2 = 1
        CPLD_UART_WSS   = 2
        CPLD_UART_OCM   = 3
        CPLD_UART_EXTEND= 4
        CPLD_UART_POE   = 5
        CPLD_UART_CNT   = 6

    
    #    UART_BAUDRATE_9600      =        9600
    #    UART_BAUDRATE_19200     =        19200
    #    UART_BAUDRATE_38400     =        38400
    #    UART_BAUDRATE_57600     =        57600
    #    UART_BAUDRATE_115200    =        115200
    UART_BAUDRATES = (9600,115200)
    UART_STOPBITS  = (1,2)
    UART_FLOWCTLS = (0,1)

    def __init__(self,logger):
        super(SDVUART,self).__init__(logger)
        self.SDV_UartInit()

    def __del__(self):
        self.SDV_UartClose()
    
    def SDV_UartInit(self):
        return self.sdv_drv_prototype("SDV_UartInit")
        
    def SDV_UartClose(self):
        return self.sdv_drv_prototype("SDV_UartClose")
    
    def SDV_UartLock(self):
        return self.sdv_drv_prototype("SDV_UartLock")

    def SDV_UartUnLock(self):
        return self.sdv_drv_prototype("SDV_UartUnLock")

    def SDV_UartWriteRead(self,mode_index,data):
        try:
            start_time = time.time()
            ucModeIndex = c_ubyte(mode_index)
            buf_len = len(data)
            nBufLen     = c_uint(buf_len)
            nRetry      = c_uint(10)
            pcBuf       = create_string_buffer(buf_len)
            pcBufr      = create_string_buffer(buf_len)
            for idx in range(buf_len):
                pcBuf[idx]=data[idx]
            print("Data len:%d"%(buf_len))
            round_idx = 0
            while True:
                round_idx +=1
                self.SDV_UartLock()
                res0 = self.libsdv.SDV_UartWrite(ucModeIndex, pcBuf, nBufLen,nRetry)
                res1 = self.libsdv.SDV_UartRead(ucModeIndex,pcBufr,nBufLen,nRetry)
                self.SDV_UartUnLock()
                idx = 0
                if OPLK_ERROR != res0 and OPLK_ERROR != res1:
                    while idx < buf_len:
                        if pcBufr[idx] != pcBuf[idx]:
                            self.logger.exception("Fail at %d expect:0x%x read:0x%x!"%(idx, self.byte_2_uchar(pcBuf[idx]), self.byte_2_uchar(pcBufr[idx])))
                            self.logger.exception("Written:%s"%(str(bytearray(pcBuf))))
                            self.logger.exception("Read:%s"%(str(bytearray(pcBufr))))
                        idx += 1
                else:
                    self.logger.exception("FAIL:write res%s, read res:%s"%(str(res0),str(res1)))
                if time.time() - start_time > 30:
                    self.logger.info("Runds:%d for %s"%(round_idx, str(mode_index)))
                    start_time = time.time()


        except Exception as error:
            self.print_exceptoin("SDV_UartWriteRead", error)
            return StatusCode.FAIL

    def SDV_UartRead(self,mode_index,buf_len):
        try:
            ucModeIndex = c_ubyte(mode_index)
            nBufLen     = c_uint(buf_len)
            nRetry      = c_uint(10)
            pcBuf       = create_string_buffer(buf_len)
            #self.SDV_UartLock()
            res = self.libsdv.SDV_UartRead(ucModeIndex, pcBuf, nBufLen, nRetry)
            #self.SDV_UartUnLock()
            return self.check_read_res(res,pcBuf,"SDV_UartRead")
        except Exception as error:
            self.print_exceptoin("SDV_UartRead", error)
            return StatusCode.FAIL

    def SDV_UartWrite(self,mode_index,data):
        try:
            ucModeIndex = c_ubyte(mode_index)
            buf_len = len(data)
            nBufLen     = c_uint(buf_len)
            nRetry      = c_uint(10)
            pcBuf       = create_string_buffer(buf_len)
            for idx in range(buf_len):
                pcBuf[idx]=data[idx]
            #self.SDV_UartLock()
            res = self.libsdv.SDV_UartWrite(ucModeIndex, pcBuf, nBufLen,nRetry)
            #self.SDV_UartUnLock()
            return self.check_write_res(res, "SDV_UartWrite")
        except Exception as error:
            self.print_exceptoin("SDV_UartWrite", error)
            return StatusCode.FAIL
    
    def SDV_UartSetting(self,func_name,setting):
        try:
            ulSetting = c_ulong(setting)
            #self.SDV_UartLock()
            res = getattr(self.libsdv,func_name)(ulSetting)
            #self.SDV_UartUnLock()
            return self.check_write_res(res, func_name)
        except Exception as error:
            self.print_exceptoin(func_name, error)
            return StatusCode.FAIL

    def SDV_UartSetBaud(self,baudrate):
        set_baud_func = "SDV_UartSetBaud"
        return self.SDV_UartSetting(set_baud_func, baudrate)

    def SDV_UartSetStopBit(self,stop_bit):
        return self.SDV_UartSetting("SDV_UartSetStopBit", stop_bit)
                
    def SDV_UartSetFlowCtl(self,flow_ctl):
        return self.SDV_UartSetting("SDV_UartSetFlowCtl", flow_ctl)




class SDVIIC(SDV):
    def __init__(self, logger=None):
        super(SDVIIC,self).__init__(logger)
        self.sdvcpld = SDVCPLD(self.logger)
        self.SDV_IicInit()

    def __del__(self):
        self.SDV_IicClose()


    def SDV_IicInit(self):
        try:
            res = self.libsdv.SDV_IicInit()
            if OPLK_OK != res:
                self.logger.exception("FAIL")
                return StatusCode.FAIL
            else:
                self.logger.debug("SUCCESS")
                return StatusCode.SUCCESS
        except Exception as err:
            self.logger.exception("FAIL:%s" % (str(err)))
            return StatusCode.FAIL

    def SDV_IicClose(self):
        try:
            res = self.libsdv.SDV_IicClose()
            if OPLK_OK != res:
                self.logger.exception("FAIL.")
                return StatusCode.FAIL
            else:
                self.logger.debug("SUCCESS")
                return StatusCode.SUCCESS
        except Exception as err:
            self.logger.exception("SDV_IicClose fail:%s" % (str(err)))
            return StatusCode.FAIL

    def SusiI2CGetFrequency(self,id=SUSI_ID_I2C_EXTERNAL):
        try:
            Id = c_uint32(id)
            freq = c_uint32(0)
            status = self.libsusi.SusiI2CGetFrequency(Id,pointer(freq))
            if OPLK_OK == status:
                freq_value= str(freq.value)
                #self.logger.info("Readed info:%s "%(readed_info))
                return freq_value
            else:
                self.logger.error("FAIL res:%d" %(status))
                return StatusCode.FAIL
        except Exception as error:
            self.logger.exception("FAIL:%s"%(str(error)))
            return StatusCode.FAIL

    def SusiI2CSetFrequency(self,freq,id=SUSI_ID_I2C_EXTERNAL):
        try:
            Id = c_uint32(id)
            Freq = c_uint32(freq)
            status = self.libsusi.SusiI2CSetFrequency(Id,Freq)
            if OPLK_OK == status:
                return StatusCode.SUCCESS
            else:
                return StatusCode.FAIL
        except Exception as error:
            self.logger.exception("FAIL:%s"%(str(error)))
            return StatusCode.FAIL

    def prepare_rw_ctypes(self,addr, buf_len):
        ulAddr = c_long(addr) #address
        ulLen = c_long(buf_len)
        if buf_len == 0:
            buf_len = 1
        read_buf = create_string_buffer(buf_len)
        return [ulAddr,read_buf, buf_len]

    def SDV_IICRun3ParaRead(self, addr,read_len,func_name):
        #wrapper for (addr,buffer, buffer_len)read functions
        try:
            u1Addr, read_buf, buf_len = self.prepare_rw_ctypes(addr,read_len)
            func = getattr(self.libsdv, func_name)
            res = func(u1Addr, read_buf, buf_len)
            #Handle the 0 length error.
            if read_len == 0 and res == -1:
               return StatusCode.SUCCESS 
            return self.check_read_res(res, read_buf, func_name)
        except Exception as error:
            self.logger.exception("SDV_IICRun3ParaRead %s exception:%s"%(func_name, str(error)))
            return StatusCode.FAIL

    def SDV_IICRun3ParaWrite(self, addr,write_data,func_name):
        #wrapper for (addr,buffer, buffer_len)write functions
        try:
            u1Addr, write_buf, buf_len = self.prepare_rw_ctypes(addr,len(write_data))
            for i in range(len(write_data)):
                write_buf[i]=write_data[i]
            func = getattr(self.libsdv, func_name)
            res = func(u1Addr, write_buf, buf_len)
            res = self.check_write_res(res, func_name)
            if StatusCode.FAIL == res:
                self.logger.error("%s write 0x%x length data at 0x%x FAIL!"%(func_name,len(write_data),addr))
            return res
        except Exception as error:
            self.logger.exception("SDV_IICRun3ParaWrite %s exception:%s"%(func_name,str(error)))
            return StatusCode.FAIL

    def SDV_IICRun4ParaRead(self, slaver_addr,cmd,read_len,func_name):
        #wrapper for (addr,buffer, buffer_len)read functions
        try:
            ucSlaveAddr = c_char(slaver_addr)
            ulCmd= c_uint(cmd)
            dummy,read_buf,buf_len = self.prepare_rw_ctypes(0, read_len)
            func = getattr(self.libsdv, func_name)
            res = func(ucSlaveAddr, ulCmd, read_buf, buf_len)
            #Handle the 0 length error.
            if read_len == 0 and res == -1:
               return StatusCode.SUCCESS 
            return self.check_read_res(res, read_buf, func_name)
        except Exception as error:
            self.logger.exception("SDV_IICRun3ParaRead %s exception:%s"%(func_name, str(error)))
            return StatusCode.FAIL

    def SDV_IICRun4ParaWrite(self, slaver_addr,cmd,write_data,func_name):
        #wrapper for (addr,buffer, buffer_len)write functions
        try:
            ucSlaveAddr = c_char(slaver_addr)
            ulCmd= c_uint(cmd)
            dummy,write_buf,buf_len = self.prepare_rw_ctypes(0, len(write_data))
            for i in range(len(write_data)):
                write_buf[i]=write_data[i]
            func = getattr(self.libsdv, func_name)
            res = func(ucSlaveAddr, ulCmd, write_buf, buf_len)
            res =  self.check_write_res(res, func_name)
            if StatusCode.FAIL == res:
                self.logger.error("%s write 0x%x length data at 0x%x with cmd:%d FAIL!"%(func_name,len(write_data),slaver_addr,cmd))
            return res
        except Exception as error:
            self.logger.exception("SDV_IICRun4ParaWrite %s exception:%s"%(func_name,str(error)))
            return StatusCode.FAIL


    def SDV_IICRunParaReadWithID(self, addr,read_len,id,func_name):
        #wrapper for (addr,buffer, buffer_len, id)read functions
        try:
            uAddr,read_buf, buf_len = self.prepare_rw_ctypes(addr,read_len)
            func = getattr(self.libsdv, func_name)
            res = func(uAddr, read_buf,  buf_len,id)
            return self.check_read_res(res, read_buf, func_name)
        except Exception as error:
            self.logger.error("SDV_IICRunParaReadWithID %s exception:%s"%(func_name, str(error)))
            return StatusCode.FAIL


    def SDV_IICRunParaWriteWithID(self, addr,write_data,id,func_name):
        #wrapper for (addr,buffer, buffer_len)write functions
        try:
            prepared = self.prepare_rw_ctypes(addr,len(write_data))
            for i in range(len(write_data)):
                prepared[1][i]=write_data[i]
            func = getattr(self.libsdv, func_name)
            res = func(prepared[0], prepared[1],  prepared[2],id)
            return self.check_write_res(res, func_name)
        except Exception as error:
            self.logger.exception("SDV_IICRun3ParaWrite %s exception:%s"%(func_name,str(error)))
            return StatusCode.FAIL


    def SDV_ReadBoardTemp(self):
        try:
            temp = c_int()
            res = self.libsdv.SDV_ReadBoardTemp(pointer(temp))
            if OPLK_OK == res:
                readed_info = str(temp.value)
                self.logger.info("Readed info:%s "%(readed_info))
                return readed_info
            else:
                self.logger.error("%s FAIL res:%d" %("SDV_ReadBoardTemp", res))
                return StatusCode.FAIL
        except Exception as error:
            self.logger.exception("SDV_ReadBoardTemp exception:%s"%(str(error)))
            return StatusCode.FAIL

    def SDV_PowerInfoRead(self,addr, read_len, power_id):
        return self.SDV_IICRunParaReadWithID(addr, read_len, power_id, "SDV_PowerInfoRead")


    def SDV_EEpromCfgRead(self,addr,read_len):
        return self.SDV_IICRun3ParaRead(addr, read_len, "SDV_EEpromCfgRead")

    def SDV_EEpromIDRead(self,addr,read_len):
        return self.SDV_IICRun3ParaRead(addr, read_len, "SDV_EEpromIDRead")

    def SDV_EEpromIDReadByCpldI2c(self,addr,read_len):
        return self.SDV_EEpromIDReadByCpldI2c(addr, read_len, "SDV_EEpromIDReadByCpldI2c")


    def SDV_EEpromFanRead(self,addr,read_len,fan_id):
        return self.SDV_IICRunParaReadWithID(addr, read_len, fan_id, "SDV_EEpromFanRead")

    def SDV_EEpromCfgWrite(self,addr,write_data):
        return self.SDV_IICRun3ParaWrite(addr,write_data,"SDV_EEpromCfgWrite")


    def SDV_EEpromIDWrite(self,addr,write_data):
        return self.SDV_IICRun3ParaWrite(addr,write_data,"SDV_EEpromIDWrite")

    def SDV_EEpromIDWriteByCpldI2c(self,addr,write_data):
        return self.SDV_IICRun3ParaWrite(addr,write_data,"SDV_EEpromIDWriteByCpldI2c")

    def SDV_EEpromFanWrite(self,addr, write_data,fan_id):
        return self.SDV_IICRunParaWriteWithID(addr, write_data, fan_id,"SDV_EEpromFanWrite")
    
    def SDV_EEpromTDCMRead(self,addr, read_len):
        eeprom_addr = 0xA8
        return self.SDV_EEPromRWFlow(eeprom_addr, addr, data_len=read_len,rw='r', func="SDV_TdcmIicRead")

    def SDV_EEpromTDCMWrite(self,addr, write_data):
        eeprom_addr = 0xA8
        return  self.SDV_EEPromRWFlow(eeprom_addr, addr, write_data, rw='w', func="SDV_TdcmIicWrite")

    def SDV_EEpromPAM4Read(self,addr, read_len):
        eeprom_addr = 0xA8
        return self.SDV_EEPromRWFlow(eeprom_addr, addr, data_len=read_len,rw='r', func="SDV_Pam4IicRead")

    def SDV_EEpromPAM4Write(self,addr, write_data):
        eeprom_addr = 0xA8
        return  self.SDV_EEPromRWFlow(eeprom_addr, addr, write_data, rw='w', func="SDV_Pam4IicWrite")

    def SDV_EEPromRWFlow(self,eeprom_addr, addr,data=None,data_len=0,rw='r',func="SDV_Pam4IicRead"):
        EEPROM_128KB_PAGE_LEN = 256
        EEPROM_128KB_CFG_WRITE_LEN = 128 
        SUSI_I2C_EXT_CMD		= 	2	 << 30
        if data:
            data_len = len(data)
        usDataLen = data_len 
        cur_addr = addr
        res = 0
        read_data = []
        write_idx = 0
        
        while data_len > 0:
            cur_addr = cur_addr & 0xFFFF | SUSI_I2C_EXT_CMD
            #self.logger.info("data len:%x, cur_addr:%x"%(data_len,cur_addr))
            if 'r' == rw:
                if data_len >= EEPROM_128KB_PAGE_LEN:
                    usDataLen = EEPROM_128KB_PAGE_LEN
                
                read_data_block = self.SDV_IICRun4ParaRead(eeprom_addr, cur_addr,usDataLen ,func)
                if StatusCode.FAIL == read_data_block:
                    self.logger.error("Function {func} FAIL at:{addr}".format(func=func,addr=hex(cur_addr)))
                    return StatusCode.FAIL
                if StatusCode.SUCCESS == read_data_block:
                    read_data +=  ""
                else:
                    read_data += read_data_block
            elif 'w' == rw:
                if data_len >= EEPROM_128KB_CFG_WRITE_LEN:
                    usDataLen = EEPROM_128KB_CFG_WRITE_LEN
                res = self.SDV_IICRun4ParaWrite(eeprom_addr, cur_addr, data[write_idx:write_idx+usDataLen], func) 
                write_idx += usDataLen
                if StatusCode.SUCCESS != res:
                    self.logger.error("Function {func} FAIL at:{addr}".format(func=func,addr=hex(cur_addr)))
                    return StatusCode.FAIL
            else:
                self.logger.exception("Wrong parameter")
                return StatusCode.FAIL

            data_len -= usDataLen
            cur_addr += usDataLen
            #self.logger.info("rest data len:%x"%(data_len))
        if 'r' == rw:
            return read_data
        return StatusCode.SUCCESS



class SDVEthernet(SDV):
    def __init__(self,logger=None):
        super(SDVEthernet,self).__init__(logger)

    def initEthernet(self):
        res = self.libsdv.SDV_Mv6390Init()
        print(res)
        self.check_driver_res(res,"initEthernet")
        
        
if __name__ == '__main__':
    sdv_iic = SDVIIC()
    sdv_iic.get_all_members()

