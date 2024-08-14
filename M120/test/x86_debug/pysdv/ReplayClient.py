import os
import asyncio
import enum
import time
import struct
import socket
import logging
from OPLogManager import *
from cgi import log
host = "10.220.12.249"
port = 2048

data_path = "C:\\TestData\\TestData.zip"
#data_path = "C:\\TestData\\test.zip"
#data_test =
class Status(enum.Enum):
    Handshake = 0,
    Transfer = 1

pack_fmt = "!I"
class EchoClientProtocol():
    def __init__(self, sock_ins,data_path,logger=None):
        if not logger:
            self.logger = logging.getLogger("Client")
        self.file_len = os.path.getsize(data_path)
        self.max_frame_size = 102400
        self.sock_ins = sock_ins
        self.sendfile = open(data_path,'rb')
        #self.receivedfile = open(os.path.join(self.dataDir,"received.zip"),'w+b')
        self.logger.info("File len:%d"%(self.file_len))


    def send(self):
        total_len = 0
        while True:
            trunk = self.sendfile.read(self.max_frame_size)
            total_len+=len(trunk)

            trunk = struct.pack(pack_fmt,len(trunk))+trunk
            self.sock_ins.send(trunk)
            #self.logger.info(len(trunk),total_len)
            if len(trunk)<self.max_frame_size:
                trunk = struct.pack(pack_fmt,0)
                self.sock_ins.send(trunk)
                break


        self.logger.info('Data sent finished')



if __name__ == '__main__':
    logger = create_oplogger("client",isFilelog=True)
    rounds = 0
    while True:
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # 创建tcp连接
            sock.connect((host,port))
            logger.info('Rounds %d connect...'%(rounds))
            client = EchoClientProtocol(sock,data_path)
            client.send()

            logger.info('Rounds %d end.'%(rounds))
            rounds+=1
            sock.close()
            #time.sleep(1)
        except Exception as error:
            logger.error(str(error))
            time.sleep(1)

