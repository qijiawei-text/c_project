import multiprocessing
import threading
import enum
from OPLogManager import *
import logging

class MultiEnum(enum.Enum):
    MultiThread = 0
    MultiProcess = 1
    MultiThreadProcess = 2

class MultiProcess(object):
    def __init__(self,obj_ins,test_functions,logger=None):
        if not logger:
            self.logger = logging.getLogger("SDV")
        else:
            self.logger = logger
        self.ObjIns = obj_ins
        self.TestFunctions = test_functions
        self.Processes=[]

    def create_processes(self):
        for idx,test_func in enumerate(self.TestFunctions):
            func = getattr(self.ObjIns, test_func["Func"])
            self.logger.info(func)
            pr = multiprocessing.Process(target=func,name=test_func["Func"],args=test_func["args"])
            self.Processes.append(pr)
        return self.Processes

    def run_processes(self):
        for pr in self.Processes:
            pr.start()
        for pr in self.Processes:
            pr.join()


class MultiThread(object):
    def __init__(self,obj_ins,test_functions,logger=None):
        if not logger:
            self.logger = logging.getLogger("SDV")
        else:
            self.logger = logger
        self.ObjIns = obj_ins
        self.TestFunctions = test_functions
        self.Threads=[]

    def create_threads(self):
        #try:
        for idx,test_func in enumerate(self.TestFunctions):
            func = getattr(self.ObjIns, test_func["Func"])
            self.logger.info(func)
            th = threading.Thread(target=func,name=test_func["Func"],args=test_func["args"])
            self.Threads.append(th)
        #for th in self.Threads:
        #    th.start()
        return self.Threads

        #except Exception as error:
        #    self.logger.error("%s FAIL error:%s"%("create_threads", str(error)))
    def run_threads(self):
        for th in self.Threads:
            th.start()
        for th in self.Threads:
            th.join()

    def waiting_for_threads(self):
        for th in self.Threads:
            th.join()
        print("Multithreads test end.")




if __name__ == '__main__':
    print("Start ...")
    logger = create_logger()
    test_ins = Test_SDVIIC(logger)
    run_test_processes(test_ins)