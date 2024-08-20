'''
Created on Oct 11, 2017

@author: qxiong06
'''
import os
import time
import logging
import logging.handlers
import multiprocessing
class OPLogger(object):
    '''
    classdocs
    '''
    log_format_str = '%(asctime)s - %(name)s - %(levelname)s - %(funcName)s:%(lineno)d - %(message)s'
    def __init__(self, log_level,enable_console=True,enable_file=False, log_file=None):
        '''
        Constructor
        '''
        self.enable_console= enable_console
        self.log_level = log_level
        self.enable_file = enable_file
        self.log_file = log_file
        self.log_format_str = OPLogger.log_format_str
        self.logger = logging.getLogger()
        self.file_handler = None
        self.console_handler = None
    
    #only config console log
    def config_console_log(self,logger):
        # create console handler and set level to debug
        if self.enable_console:
            self.console_handler = logging.StreamHandler()
            self.console_handler.setLevel(self.log_level)
            formatter = logging.Formatter(self.log_format_str)
            self.console_handler.setFormatter(formatter)
            logger.addHandler(self.console_handler)
    
    #config
    def config_file_log(self,logger):
        #file log
        #print("enable_file",self.enable_file)
        if self.enable_file:
            #self.file_handler = logging.handlers.RotatingFileHandler(self.file_log_conf['log_filename'],maxBytes=10240, backupCount=100)
            self.file_handler = logging.FileHandler(self.log_file)
            print(self.log_file)
            self.file_handler.setLevel(self.log_level)
            self.file_handler.setFormatter(logging.Formatter(self.log_format_str))
            logger.addHandler(self.file_handler)
            return self.file_handler

    def getLogger(self,name):
        logger = logging.getLogger(name)
        return logger
    
class SimpleLogger(OPLogger):
    def __init__(self, log_level ,enable_console=True,enable_file=False, log_file=None):
        super(SimpleLogger,self).__init__(log_level, enable_console, enable_file, log_file)
        self.logger.setLevel(log_level)
        self.config_console_log(self.logger)
        self.config_file_log(self.logger)
    def __del__(self):
        print("Delete:",self)

class OPMultiprocessLogger(OPLogger):
    ##On windows this class varible can't inherent to subprocess.
    ##It must be transfer from parameter when create subprocess.
    logger_queue = multiprocessing.Queue(-1)
    listener_pid = 0
    current_pid = 0
    def __init__(self,log_level=logging.NOTSET, enable_console=True,enable_file=True,log_file=None):
        super(OPMultiprocessLogger,self).__init__(log_level, enable_console,True,log_file)
        self.listener_process = None
        self.is_listener_process_alive = False
        self.logger_queue = OPMultiprocessLogger.logger_queue
        self.log_level = log_level
        OPMultiprocessLogger.current_pid= os.getpid()
        print("main queue:",self.logger_queue)
        self.create_file_logger_process_ifneed()
        

    def listerner_configurer(self):
        root = logging.getLogger()
        root.setLevel(self.log_level)
        self.config_console_log(root)
        self.config_file_log(root)
        OPMultiprocessLogger.listener_pid = os.getpid()
        OPMultiprocessLogger.current_pid = os.getpid()

    def log_listener_process(self,queue):
        print('log listener process started')
        #This is the listener process top-level loop: wait for logging events
        # (LogRecords)on the queue and handle them, quit when you get a None for a LogRecord.
        self.listerner_configurer()
        while True:
            try:
                record = queue.get()
                #print(record)
                if record is None:  # We send this as a sentinel to tell the listener to quit.
                    break
                logger = logging.getLogger(record.name)
                logger.handle(record)  # No level or filter logic applied - just do it!
            except Exception:
                import sys, traceback
                print('Whoops! Problem:', file=sys.stderr)
                traceback.print_exc(file=sys.stderr)
        self.file_handler.close()
        print('log listerner process ended')

    def create_file_logger_process_ifneed(self):
        if not self.listener_process:
            self.listener_process = multiprocessing.Process(target=self.log_listener_process,
                                       args=(self.logger_queue,))
            self.listener_process.start()
            self.is_listener_process_alive = True

    def stop_file_logger_process(self):
        self.logger_queue.put_nowait(None)
        self.listener_process.join()
        self.is_listener_process_alive = False

    def stop_logger(self):
        self.stop_file_logger_process()
        
    @classmethod
    def get_queue(cls):
        return cls.logger_queue
    
    # The worker configuration is done at the start of the worker process run.
    # Note that on Windows you can't rely on fork semantics, so each process
    # will run the logging configuration code when it starts.
    @classmethod
    def worker_configurer(cls,queue,log_level):
        current_pid = os.getpid()
        #if current_pid==cls.current_pid:
        #    return
        print("pids:",cls.listener_pid,cls.current_pid,os.getpid())
        cls.current_pid=os.getpid()
        cls.logger_queue = queue
        h = logging.handlers.QueueHandler(queue)  # Just the one handler needed
        #config the root handler
        root = logging.getLogger()
        # send all messages
        h.setLevel(log_level)
        root.setLevel(log_level)
        h.setFormatter(logging.Formatter(OPLogger.log_format_str))
        #print(log_level)
        root.addHandler(h)
    
    @classmethod
    def getLogger(cls,name,log_level=logging.DEBUG):
        '''
        For multiprocess logging to the same file.
        '''
        queue = OPMultiprocessLogger.logger_queue
        print("sub queue",queue)
        logger = cls.worker_configurer(queue,log_level)
        logger = logging.getLogger(name)
        return logger
    

    def __del__(self):
        print('delete',self)
        #if self.is_listener_process_alive:
        #    self.stop_file_logger_process()





def create_oplogger(log_level=logging.DEBUG,file_name=None,log_dir=None,isFilelog=False,is_multiprocess=False):
    if not log_dir:
        log_dir =os.path.join( os.getcwd(), "log")
        os.makedirs(log_dir,exist_ok=True)
    log_file_path = os.path.join(log_dir, time.strftime("%Y-%m-%d-%H-%M-%S")+'-'+str(file_name)+'.log')

    if is_multiprocess and isFilelog:
        log_manager = OPMultiprocessLogger(log_level, True, True, log_file_path)
        return log_manager
    else:
        log_manager = SimpleLogger(log_level,True, isFilelog,log_file_path)
        return log_manager
    # log_manager = OPLogManager()









