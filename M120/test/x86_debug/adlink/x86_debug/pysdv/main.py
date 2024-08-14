import sys
from TestSDVIIC import *
from TestSDVCPLD import *
from TestSDVUART import *
from MultiProcessTest import  *
from OPLogManager import OPLogManager

def create_logger():
    log_file_conf = {'enable':False, 'log_level':logging.INFO, 'log_filename':"log.log"}
    log_file_conf['log_filename'] = os.path.join(os.getcwd(), "log", time.strftime("%Y-%m-%d-%H-%M-%S") + '.log')
    log_manager = OPLogManager(log_level=logging.INFO, file_log=log_file_conf)
    # log_manager = OPLogManager()
    return log_manager.get_logger("MAINLOG")

def i2c_multithreads(logger):
    test_functions=[{"Func":"Test_SDV_ReadBoardTemp",     "args":(1000000,60*60*12)},
                    {"Func":"Test_SDV_PowerInfoRead",     "args":(1000,60*60*12)},
                    {"Func":"Test_SDV_EEpromCfgReadWrite","args":(1000,60*60*12)},
                    {"Func":"Test_SDV_EEpromIDReadWrite", "args":(1000,60*60*12)}
                    ]
    sdv_iic = TestSDVIIC(logger)
    sdv_iic_th = Multithread(sdv_iic,test_functions,logger)
    sdv_iic_th.create_threads()
    return sdv_iic_th.Threads

cpld_funcs = [{"Func":"Test_cpld_ram_rw","args":(0xC000,0x1000,0,60*60*12)},
             {"Func":"Test_cpld_ram_rw","args":(0xD000,0x1000,0,60*60*12)},
             {"Func":"Test_cpld_ram_rw","args":(0xE000,0x1000,0,60*60*12)},
             {"Func":"Test_cpld_ram_rw","args":(0xF000,0xFFF ,0,60*60*12)}]

def cpld_multithreads(logger):

    sdv_cpld = TestSDVCPLD(logger)
    sdv_cpld_th = Multithread(sdv_cpld,cpld_funcs,logger)
    sdv_cpld_th.create_threads()
    return sdv_cpld_th.Threads

def cpld_multiprocesses(logger):
    sdv_cpld = TestSDVCPLD(logger)
    sdv_cpld_pr = MultiProcess(sdv_cpld,cpld_funcs,logger)
    return sdv_cpld_pr.create_processes()

#iic = i2c_multithreads(logger)
#cpld = cpld_multithreads(logger)
#all_th = iic+cpld
#print(all_th)
#all_pr = cpld_multiprocesses(logger)
#for pr in all_pr:
#    pr.join()
#sdv_cpld = TestSDVCPLD(logger)
#sdv_cpld.Test_read_cpld_default()
#sdv_cpld.Test_cpld0_default()
#sdv_cpld.Test_cpld1_default()
#sdv_cpld.Test_cpld_ram_rw()
#data = sdv.libsdv.SDV_EEpromCfgRead(0, 16)
#print(data)
#sdviic = TestSDVIIC(logger)
#sdviic.Test_SDV_PowerInfoReadSingle()
#sdv.Test_SDV_PowerInfoRead()
#print(sdv.libsdv.SDV_PowerInfoRead(0, 512, 1))
#sdv.Test_SDV_EEpromCfgReadWrite()
#sdv.Test_SDV_EEpromCfgReadWriteSingle()
#func_pair = [[SDVIIC.OnBoardDeviceReadFunc[6],SDVIIC.OnBoardDeviceReadFunc[7]]]
#sdv.Test_SDV_EEpromReadWrite_3Para(func_pair)
#sdv.Test_SDV_EEpromCfgReadWrite()
#sdv.Test_SDV_EEpromIDReadWrite()
#sdv.Test_SDV_EEpromFanReadWrite()
#sdv.Test_SDV_EEpromIDReadWriteSingle()
#sdv.Test_SDV_PowerInfoRead()

if __name__ == '__main__':
    print("Start ...")
    logger = create_logger()
    test_uart = TestSDVUART(logger)
    test_uart.Test_uart_loop_devices([0x5A]*5120)
    test_uart.Test_uart_loop_devices()





