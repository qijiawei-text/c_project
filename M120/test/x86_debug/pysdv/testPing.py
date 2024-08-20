import time
import logging
import subprocess
from SDV import *

from OPLogManager import *



class TestPing():
    def __init__(self,host,logger=None):
        self.host = host
        self.logger=logger

    def pingloop(self,count=4,buf_size=None,max_time=60*60*2):
        end_time = time.time() + max_time
        success_rounds=0
        fail_rounds=0
        while time.time() < end_time:
            res = self.ping(count,buf_size)
            if res:
                success_rounds += 1
            else:
                fail_rounds += 1
            self.logger.info("Total rounds:%d, success:%d, fail:%d"%(success_rounds+fail_rounds,success_rounds,fail_rounds))

    def ping(self,count=4,buf_size=None):
        try:
            ping_cmd = ["ping"]
            if count:
                ping_cmd.append("-n")
                ping_cmd.append(str(count))
            if buf_size:
                ping_cmd.append("-l")
                ping_cmd.append(str(buf_size))
            ping_cmd.append(self.host)

            ping  = subprocess.Popen(ping_cmd,
                             stdout = subprocess.PIPE,
                             stderr = subprocess.PIPE)
            out, error = ping.communicate()
            #print(str(out))
            out = str(out,'utf-8').split("\r\n")
            if out:
                for ele in out:
                    #self.logger.info(out)
                    if "Packets:" in ele:
                        self.logger.info(ele)
                        if "(0% loss)" in ele:
                            return True
                        else:
                            return False
            if error:
                self.logger.error(error)
                return false
        except Exception as error:
            self.logger.error("%s Error:%s"%("ping",str(error)))
            return False




def create_logger():
    log_file_conf = {'enable':True, 'log_level':logging.INFO, 'log_filename':"log.log"}
    log_file_conf['log_filename'] = os.path.join(os.getcwd(), "log", time.strftime("%Y-%m-%d-%H-%M-%S") + '.log')
    log_manager = OPLogManager(log_level=logging.INFO, file_log=log_file_conf)
    # log_manager = OPLogManager()
    return log_manager.get_logger("MAINLOG")

if __name__ =='__main__':
    logger = create_logger()
    #eth = SDVEthernet()
    #eth.initEthernet()
    testPing = TestPing("10.220.12.249",logger)
    testPing.pingloop()