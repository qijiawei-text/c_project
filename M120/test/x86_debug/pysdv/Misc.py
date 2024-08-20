'''
Created on Oct 18, 2017

@author: qxiong06
'''
import os
import platform

def filter_file_path(folder,file_name_filter):
    res =  os.walk(folder)
    for dirpath, dirnames,filenames in res:
        for filename in filenames:
            if not file_name_filter:
                full_path = os.path.join(dirpath,filename)
                #print(full_path)
                yield full_path
            else:
                if file_name_filter(filename):
                    full_path = os.path.join(dirpath,filename)
                    #print(full_path)
                    yield full_path


def GetPlatform():
  sysstr = platform.system()
  if(sysstr =="Windows"):
    print ("Call Windows tasks")
  elif(sysstr == "Linux"):
    print ("Call Linux tasks")
  else:
    print ("Other System tasks")
    return sysstr

def IsLinux():
    if (platform.system()=="Linux"):
        return True
    else:
        return False

def IsWindows():
    if (platform.system()=="Windows"):
        return True
    else:
        return False
