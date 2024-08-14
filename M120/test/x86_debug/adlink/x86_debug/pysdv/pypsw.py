'''
Created on Oct 11, 2017

@author: qxiong06
'''
import socket
import logging
import time
import serial
from util import *
from OPLogManager import *

class PswConn(object):

    '''
    classdocs
    '''

    def __init__(self,uart_name,uart_baudrate=9600,ip="",port=2268,logger=None): 
        if not logger:
            self.logger = logging.getLogger("PSW_CONN")
        else:
            self.logger = logger

        if len(uart_name)>0:
            self.port_type="UART"
            try:
                self.psw_conn= serial.Serial(uart_name,baudrate=uart_baudrate)
            except Exception as err:
                self.logger.error("Open uart %s fail error:%s!"%(uart_name, str(err)))
                raise
        elif len(ip)>0:
            self.port_type="SOCKET"
            try:
                self.psw_conn= socket.socket(socket.AF_INET,socket.SOCK_STREAM)
                self.psw_conn.connect((ip,port))
            except Exception as err:
                self.logger.error("Open psw at %s %d fail error:%s!"%(ip,port,str(err)))
                raise
    
        
    def __del__(self):
        self.psw_conn.close()


    def print_response(self,data):
        self.logger.info(data)
            #print(e.decode())

    def read_data(self,length=128, do_print=False):
        try:
            if "UART" == self.port_type:
                self.psw_conn.timeout = 1
                data = self.psw_conn.read(length)
                
            elif "SOCKET" == self.port_type:
                data = self.psw_conn.recv(length)
            
            data = data.decode()
            if do_print:    
                self.print_response(data)
            return data
        except BaseException as err:
            self.logger.error(str(err))
            return StatusCode.FAIL

    def send_data(self,data):
        try:
            if isinstance(data,str):
                data=data.encode()
            data_len = len(data)
            if "SOCKET" == self.port_type:
                sent_num = self.psw_conn.send(data)
                self.psw_conn.flush()
            elif "UART" == self.port_type:
                sent_num = self.psw_conn.write(data)
                self.psw_conn.flush()
            if sent_num == data_len:
                self.logger.debug("Sent command %s"%(str(data)))
                return StatusCode.SUCCESS
            else:
                self.logger.error("Send commad %s FAIL"%(str(data)))
                return StatusCode.FAIL

        except BaseException as err:
            self.logger.error(str(err))
            return StatusCode.FAIL 



class PyPsw(object):
#class PyPower(object):
    def __init__(self,uart_name,uart_baudrate=9600,ip="",port=2268,logger=None): 
        if not logger:
            self.logger = logging.getLogger("PSW_UART")
        else:
            self.logger = logger
        self.support_idns=[{"Manufacturer":"GW-INSTEK",
                            "Model_number":"PSU60-25",
                            "Serial_number":"GEO153329"}]
        self.conn = PswConn(uart_name,uart_baudrate,ip,port,self.logger)
    
    def send_command_read_response(self,command):
        status = self.conn.send_data(command)
        if StatusCode.SUCCESS == status:
            res = self.conn.read_data(2048)
            self.logger.debug(res)
            return res
        else:
            self.logger.error("Send command %s FAIL!"%(command))
            return StatusCode.FAIL
            
    def query(self, command):
        res = self.send_command_read_response(command+"?\n")
        return res            

    def query_idn(self):
        res = self.query("*idn")
        return res



    #def __init__(self,uart_name,uart_baudrate=9600,ip="",port=2268,logger=None): 
    #    super(PyPsw,self).__init__(uart_name, uart_baudrate, ip, port, logger)
    #    self.support_idns=[{"Manufacturer":"GW-INSTEK",
    #                        "Model_number":"PSU60-25",
    #                        "Serial_number":"GEO153329"}]
     
    def get_support_idns(self):
        for idn in self.support_idns:
            idn_str = ",".join((idn['Manufacturer'],idn['Model_number'],idn['Serial_number']))
            yield idn_str
            
    def check_device(self):
        device_idn = self.query_idn()
        for idn in self.get_support_idns():
            self.logger.debug(idn)
            if idn in device_idn:
                return True
        self.logger.error("Don't support this device! %s"%(device_idn))
        return False

    def query_vol_cur(self):
        res = self.send_command_read_response("APPLy?\n")
        if StatusCode.FAIL != res:
            res = res.split(',')
            res = (float(res[0]),float(res[1]))
            return res
        return res
    
    def query_output(self):
        res = self.send_command_read_response("OUTPut?\n")
        if StatusCode.FAIL != res:
            return int(res)
        return res
    
    def measure_vol(self):
        res = self.send_command_read_response("MEASure:SCALar:VOLTage:DC?\n")
        if StatusCode.FAIL != res:
            return float(res)
        return res
    
    def measure_cur(self):
        res = self.send_command_read_response("MEASure:SCALar:CURRent:DC?\n")
        if StatusCode.FAIL != res:
            return float(res)
        return res
    
    def measure_power(self):
        res = self.send_command_read_response("MEASure:SCALar:POWer:DC?\n")
        if StatusCode.FAIL != res:
            return float(res)
        return res
        
    def query_error(self):
        res = self.send_command_read_response("SYSTem:ERRor?\n")
        self.logger.info(res)
        return res   

    def query_rear_usb_status(self):
        res = self.send_command_read_response("SYSTem:COMMunicate:USB:REAR:STATe?\n")
        return res
    
    def set_output_mode(self,mode='CVLS'):
        res = self.send_command_read_response("OUTPut:MODE %s\n"%(mode))
        return res
    
    def query_output_mode(self):
        res = self.send_command_read_response("OUTPut:MODE?\n")
        if StatusCode.FAIL != res:
            return int(res)
        return res

    def enable_output(self):
        res = self.send_command_read_response("OUTPut ON\n")
        if StatusCode.FAIL != res:
            #return int(res)
            return StatusCode.SUCCESS
        return res

    def disable_output(self):
        res = self.send_command_read_response("OUTPut OFF\n")
        if StatusCode.FAIL != res:
            #return int(res)
            return StatusCode.SUCCESS
        return res

    def set_voltage_current(self,voltage, current):
        res = self.send_command_read_response("APPLy %f,%f\n"%(voltage,current))
        #if StatusCode.FAIL != res:
            #res = res.split(',')
            #res = (float(res[0]),float(res[1]))
            #return res
        return res
    
    def check_voltage_current(self,voltage, current):
        vol, cur = self.query_vol_cur()
        if vol == voltage and cur == current:
            self.logger.debug("The voltage and current are the same with device.")
            return True
        else:
            self.logger.debug("Voltage and current not match. Expect:%fV, %fA;Read:%fV, %fA"%(voltage,current,vol,cur))
            return False

    def set_voltage_current_and_enable(self,voltage,current):
        res = self.set_voltage_current(voltage, current)
        res1 = self.enable_output()
        return (res,res1)
        
    def set_voltage_slew_rise_rate(self,rate=0.6):
        res = self.send_command_read_response("SOURce:VOLTage:SLEW:RISing %f\n"%(rate))
        return res

    def set_voltage_slew_fall_rate(self,rate=0.6):
        res = self.send_command_read_response("SOURce:VOLTage:SLEW:FALLing %f\n"%(rate))
        return res
    
    def query_slew_rate(self):
        output_mode = self.query_output_mode()
        if 0 == output_mode or 1 == output_mode:
            return (output_mode)
        elif 2 == output_mode:
            rise_res = self.send_command_read_response("SOURce:VOLTage:SLEW:RISing?\n")
            fall_res = self.send_command_read_response("SOURce:VOLTage:SLEW:FALLing?\n")
        elif 3 == output_mode:
            rise_res = self.send_command_read_response("SOURce:CURRent:SLEW:RISing?\n")
            fall_res = self.send_command_read_response("SOURce:CURRent:SLEW:FALLing?\n")
        else:
            return StatusCode.FAIL
        return (int(output_mode),float(rise_res),float(fall_res))
        
   

    def SetPSWVoltageCurrent(self,voltage, current,output=True):
        try:
            if not self.check_voltage_current(voltage, current):
                self.set_voltage_current(voltage, current)
                if not self.check_voltage_current(voltage, current):
                    self.logger.error("Can't set the voltage and current!")
                    vol, cur = self.query_vol_cur()
                    self.logger.error("Device voltage:%f, current:%f"%(vol,cur))
                    return StatusCode.FAIL
        
            if output and not self.query_output():
                self.enable_output()
            vol = self.measure_vol()
            return vol
        except Exception as error:
            self.logger.error("FAIL with %s"%(error))
            return StatusCode.FAIL
    
    def SetPSWVoltageSlewRate(self,rise_rate,fall_rate):
        try:
            mode, rise, fall = self.query_slew_rate()
            if 2 == mode and rise == rise_rate and fall == fall_rate:
                return (mode,rise,fall)
            else:
                self.set_output_mode("CVLS")
                self.set_voltage_slew_rise_rate(rise_rate)
                self.set_voltage_slew_fall_rate(fall_rate)
                res = self.query_slew_rate()
                return res
        except Exception as error:
            self.logger.error("FAIL with %s"%(error))
            return StatusCode.FAIL
    
        
if __name__ == '__main__':
    create_oplogger(log_level=logging.DEBUG)
    psw = PyPsw(uart_name="com12")
    psw.query_idn()
    psw.query_vol_cur()
    psw.set_voltage_current_and_enable(12, 5)
    time.sleep(3)
    #psw.disable_output()
    psw.query_voltage()
    #psw.disable_output()
    time.sleep(3)
    psw.query_voltage()
#    psw.set_voltage(12.2, 4)
#    psw.enable_output()
    


