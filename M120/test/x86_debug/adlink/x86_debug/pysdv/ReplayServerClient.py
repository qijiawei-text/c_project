#!/usr/bin/python
# -*- coding: utf-8 -*-
import struct
import os
import sys
import time
import socket
import threading
import multiprocessing
import hashlib
from OPLogManager import *
from platform import platform
from Misc import *

success_rounds = 0
fail_rounds = 0

pack_fmt = "!I"
class EchoServerProtocol():
    '''
    receive package format of [[4bytes length]readl data]
    '''
    def __init__(self, serve_dir, sock_ins,logger=None):
        if not logger:
            self.logger = logging.getLogger("SERVER")
        else:
            self.logger = logger
        self.pack_fmt = "!I"
        if os.path.isdir(serve_dir):
            self.dataDir = serve_dir
        else:
            raise ("Need a directory path:%s"%(serve_dir))

        self.sock_ins =sock_ins
        self.frame_size = 0
        self.received_frame_size = 0
        self.read_frame_size = 1024*100
        self.handle_turnk_size = 1024*1000
        self.start_time = 0
        self.total_time = 0
        self.file_size = 0
        self.total_received = 0



    def md5_for_file(self,file_path, block_size=2**20):
        self.print_status(file_path)
        md5 = hashlib.md5()
        f = open(file_path,'rb')
        while True:
            data = f.read(block_size)
            if not data:
                break
            md5.update(data)
        md5_res = md5.hexdigest()
        self.logger.info("MD5 res:%s"%(md5_res))
        return md5_res


    def check_md5(self,md5_val):
        global success_rounds
        global fail_rounds
        if md5_val == self.md5_for_file(self.file_path):
            self.logger.info("SUCCESS")
            success_rounds += 1
        else:
            self.logger.info("FAIL")
            fail_rounds += 1
        os.remove(self.file_path)
        self.logger.info("Total roudns:%d, Success rounds:%d, fail rounds:%d"%(success_rounds+fail_rounds, success_rounds, fail_rounds))
        self.logger.info("Delete %s"%(self.file_path))
        return (success_rounds,fail_rounds)

    def print_status(self,file_path):
        self.total_time = time.time()-self.start_time
        file_size = os.path.getsize(file_path)
        self.logger.info("File size:%d KB, total time:%d s, speed:%d KB/s"%(file_size/1024,self.total_time,file_size/1024/self.total_time))

    def get_frame_size(self,frame_data):
        pack_size = struct.calcsize(self.pack_fmt)
        try:
            size = struct.unpack(self.pack_fmt, frame_data[0:pack_size])[0]
            #print("pack_size, frame_size:",pack_size,size)
            if size == 0:
                self.logger.info("Reach the end of the file.")
            return size
        except Exception as error:
            print(error)
            return 0


    def handle_received_data(self,received_data):
        ''''''
        #print("handle data len",len(received_data))
        pack_size = struct.calcsize(self.pack_fmt)
        if len(received_data)<pack_size:
            self.logger.error("Error:data length should bigger than pack_size")
            return (False,0)
        self.frame_size = self.get_frame_size(received_data)
        if self.frame_size == 0:
            return (False,0)
        if self.frame_size > self.handle_turnk_size:
            self.logger.error("frame size bigger than the max limitation:",str(self.frame_size), str(self.handle_turnk_size))
            return (False,0)
        while len(received_data)>=self.frame_size+pack_size:
            val_data = received_data[pack_size:self.frame_size+pack_size]
            self.receivedfile.write(val_data)
            #next frame
            received_data = received_data[self.frame_size+pack_size:]
            #print("received_data len",len(received_data))

            if len(received_data)>=pack_size:
                self.frame_size = self.get_frame_size(received_data)
                #print("next len",self.frame_size)
                if self.frame_size == 0:
                    return (False,0)

        self.receivedfile.flush()
        self.received_frame_size = len(received_data)
        #print("rest received_data:",len(received_data))
        #print("end next len",self.get_frame_size(received_data))
        return (True,received_data)

    def open_receive_file(self):
        file_path = os.path.join(self.dataDir,time.strftime("%Y-%m-%d-%H-%M-%S")+'-received')
        i=0
        while os.path.exists(file_path):
            file_path = file_path+str(i)
            i += 1

        receivedfile = open(file_path,'w+b')
        self.logger.info("received file path:%s"%(file_path))
        return (file_path, receivedfile)

    def receiver(self,md5_val):
        self.logger.info("start receive...")
        pack_size = struct.calcsize(self.pack_fmt)
        self.file_path,self.receivedfile = self.open_receive_file()
        self.start_time = time.time()
        while True:
            try:
                frame_data = self.sock_ins.recv(self.read_frame_size)
                if frame_data:
                    if self.received_frame_size == 0:
                        recvd_frame_data = frame_data
                    else:
                        recvd_frame_data = recvd_frame_data+frame_data
                    self.received_frame_size = len(recvd_frame_data)
                    #print("frame size, received_frame_size",len(frame_data),self.received_frame_size)

                    if  self.received_frame_size < self.handle_turnk_size:
                        continue

                if not frame_data and self.received_frame_size < pack_size:
                    #Didn't read data
                    self.logger.error("No more data, but not terminated correctly, close connecttion")
                    self.sock_ins.close()
                    self.receivedfile.flush()
                    self.receivedfile.close()
                    self.check_md5(md5_val)
                    return

                #The received data size must > pack_size if reach here.
                #print("Handle received data",self.received_frame_size,self.frame_size)
                success,recvd_frame_data =  self.handle_received_data(recvd_frame_data)

                if not success:
                    self.logger.info("No more data, close connecttion")
                    self.sock_ins.close()
                    self.receivedfile.flush()
                    self.receivedfile.close()
                    self.check_md5(md5_val)
                    return
                else:
                    self.received_frame_size  = len(recvd_frame_data)
            except Exception as error:
                self.sock_ins.close()
                self.receivedfile.flush()
                self.receivedfile.close()
                self.check_md5(md5_val)
                print(error)
                return

class EchoServer():
    def __init__(self,host,port,serve_dir=None,md5_val="",logger=None):
        self.host = host
        self.port = port
        self.serve_dir = serve_dir
        if not logger:
            self.logger = logging.getLogger("SERVER")
        else:
            self.logger = logger
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # 创建tcp连接
        self.sock.bind((host, port))  # 定于端口和ip
        self.sock.listen(5)  # 监听
        self.logger.info("Listening at %s:%d"%(self.host,self.port))
        self.md5 = md5_val


    def start_server(self):
        while True:
            try:
                newsock, address = self.sock.accept()
                self.logger.info ("accept another connection from%s"%(str(address)))
                receiver = EchoServerProtocol(self.serve_dir,newsock,self.logger)
                tmpThread = threading.Thread(target=receiver.receiver, args=(self.md5,))  # 如果接收到文件，创建线程
                tmpThread.start()  # 执行线程
                #success_rounds,fail_rounds=check_md5(receiver,success_rounds,fail_rounds)
                #check_thread = threading.Thread(target=check_md5,args=(receiver,))
                #check_thread.start()
            except Exception as error:
                self.logger.error("Error:%s"%(str(error)))




class EchoClientProtocol():
    def __init__(self, sock_ins,logger=None):
        if not logger:
            self.logger = logging.getLogger("CLIENT")
        else:
            self.logger = logger

        self.max_frame_size = 102400
        self.sock_ins = sock_ins
        self.pack_fmt = "!I"



    def send(self,data_path):
        file_len = os.path.getsize(data_path)
        sendfile = open(data_path,'rb')
        #self.receivedfile = open(os.path.join(self.dataDir,"received.zip"),'w+b')
        self.logger.info("File len:%d"%(file_len))
        total_len = 0
        while True:
            trunk = sendfile.read(self.max_frame_size)
            total_len+=len(trunk)

            trunk = struct.pack(self.pack_fmt,len(trunk))+trunk
            self.sock_ins.send(trunk)
            #self.logger.info(len(trunk),total_len)
            if len(trunk)<self.max_frame_size:
                trunk = struct.pack(pack_fmt,0)
                self.sock_ins.send(trunk)
                break

        self.logger.info('Data sent finished')
        sendfile.close()



class EchoClient():
    def __init__(self,host,port,file_path,logger=None):
        self.host = host
        self.port = port
        self.file_path = file_path
        if not logger:
            self.logger = logging.getLogger("CLIENT")
        else:
            self.logger = logger

    def start_client(self):
        rounds = 0
        while True:
            try:
                self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # 创建tcp连接
                self.sock.connect((self.host,self.port))
                self.logger.info('Rounds %d connect...'%(rounds))
                client = EchoClientProtocol(self.sock,self.logger)
                client.send(self.file_path)
                self.logger.info('Rounds %d end.'%(rounds))
                rounds+=1
                self.sock.close()
                time.sleep(1)
            except Exception as error:
                self.logger.error(str(error))
                time.sleep(1)


def create_start_server(host,port,serve_dir,md5_val):
    eServer = EchoServer(host,port,serve_dir,md5_val)
    if IsWindows():
        pr = threading.Thread(target=eServer.start_server)
    elif IsLinux():
        pr = multiprocessing.Process(target=eServer.start_server)
    #pr.start()
    return [pr]

def create_start_client(host,port,file_path):
    server_host = host
    server_port = port
    eClient = EchoClient(host,port,file_path)
    #
    if IsWindows():
        pr = threading.Thread(target=eClient.start_client)
    elif IsLinux():
        pr = multiprocessing.Process(target=eClient.start_client)
    #pr.start()
    return [pr]




linux_ser_host = "10.220.12.249"
linux_cli_host = "10.220.12.39"
win_cli_host = "10.220.12.249"
win_ser_host = "10.220.12.39"
port = 2048
data_dir = "/media/workdir/"
data_path = "/media/workdir1/TestData.zip"
linux_log_dir = "/home/oplink/bootdisk/"
win_file_path = "C:\\TestData\\TestData.zip"
win_data_dir = "C:\\TestData\\receive_dir"
win_log_dir  = "C:\\TestData"
exit_md5 = "45493961c410df14d197acac4b24cc1c"

def start_linux_server_client():
    prs = []
    ser = create_start_server(linux_ser_host,port,data_dir,exit_md5)
    prs+=ser
    cli = create_start_client(linux_cli_host,port,data_path)
    prs+=cli
    return prs

def main():
    args = sys.argv

    if len(args)<2:
        args.append("all")
    prs=[]
    print(args)
    if IsWindows():
        if args[1]=="server":
            ser = create_start_server(win_ser_host,port,win_data_dir,exit_md5)
            prs+=ser
        elif args[1]=="client":
            cli = create_start_client(win_cli_host,port,win_file_path)
            prs+=cli
        else:
            ser = create_start_server(win_ser_host,port,win_data_dir,exit_md5)
            prs+=ser
            cli = create_start_client(win_cli_host,port,win_file_path)
            prs+=cli
    elif IsLinux():
        if args[1]=="server":
            ser = create_start_server(linux_ser_host,port,data_dir,exit_md5)
            prs+=ser
        elif args[1]=="client":
            cli = create_start_client(linux_cli_host,port,data_path)
            prs+=cli
        else:
            ser = create_start_server(linux_ser_host,port,data_dir,exit_md5)
            prs+=ser
            cli = create_start_client(linux_cli_host,port,data_path)
            prs+=cli
    for pr in prs:
        pr.start()
    for pr in prs:
        pr.join()

if __name__ == '__main__':
    prs = []
    if IsWindows():
        logger = create_oplogger("server",win_log_dir,isFilelog=True)
    elif IsLinux():
        logger = create_oplogger("server",linux_log_dir,isFilelog=True)
    main()



