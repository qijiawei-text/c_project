#! /usr/bin/python

import argparse
import os
import os.path
import shutil
import re

parser = argparse.ArgumentParser()
parser.add_argument('post', help = 'device type')
parser.add_argument('mode', help = 'set or reset')
args = parser.parse_args()

def range_set():
    if args.mode == 'set':
        if os.path.exists('pre-install-' + args.post + '.txt'):
            with open('pre-install-' + args.post + '.txt') as config_file:
                try:
                    while '<range>' not in next(config_file):
                        pass
                    line = next(config_file)
                    while '</range>' not in line:
                        mylist = line.split(':')
                        filename = mylist[0] + '.yang'
                        if os.path.exists(filename):
                            shutil.copyfile(filename, filename + '.temp')
                            if not os.path.exists(filename + '.save'):
                                shutil.copyfile(filename, filename + '.save')
                            with open(filename + '.temp') as file_iter, open(filename, 'w') as temp:
                                for line in file_iter:
                                    if ' ' + mylist[1] + ' ' in line: 
                                        found = False
                                        temp.write(line)
                                        line = next(file_iter)
                                        try:
                                            while not found:
                                                if re.search(r'range\s.*\.\..*;', line):
                                                    line = re.sub(r'range\s.*\.\..*;', 'range ' + mylist[2].rstrip() + ';', line)
                                                    temp.write(line)
                                                    found = True
                                                else:
                                                    temp.write(line)
                                                    line = next(file_iter)
                                        except StopIteration:
                                            print("skipped " + mylist[0] + ".yang, " + "because do not find range in " + mylist[1])
                                            pass
                                    else:
                                        temp.write(line)
                            os.remove(filename + '.temp')
                        line = next(config_file)
                except StopIteration:
                    pass
    elif args.mode =='reset':
        if os.path.exists('pre-install-' + args.post + '.txt'):
            with open('pre-install-' + args.post + '.txt') as config_file:
                try:
                    while '<range>' not in next(config_file):
                        pass
                    line = next(config_file)
                    while "</range>" not in line:
                        filename = line.split(':')[0] + '.yang'
                        if os.path.exists(filename) and os.path.exists(filename + '.save'):
                            os.remove(filename)
                            os.rename(filename + '.save', filename)
                        line = next(config_file)
                except StopIteration:
                    pass

def startup_set(cfg_head, filename, xml_heads):
    if args.post == 'ILA':
        return 
    if args.mode == 'set':
        if os.path.exists('pre-install-' + args.post + '.txt') and os.path.exists(filename + '-system.xml'):
            with open('pre-install-' + args.post + '.txt') as config_file:
                try:
                    while '<' + cfg_head + '>' not in next(config_file):
                        pass
                    line = next(config_file)    
                    while '</' + cfg_head + '>' not in line:
                        mylist = line.split(':')
                        for n in range(0, len(xml_heads)):
                            if n == 0:
                                mode = 'w'
                            elif n > 0:
                                mode = 'a'
                            with open(filename + '-system.xml') as sys_file, open(filename + '-' + args.post + '.xml', mode) as temp:
                                if n == 0:
                                    temp.write(next(sys_file))
                                line = next(sys_file)
                                while not re.search('<' + xml_heads[n] + r'.*>', line):
                                    line = next(sys_file)
                                temp.write(line)
                                for line in sys_file:
                                    if '</' + xml_heads[n] + '>' not in line:
                                        temp.write(line)
                                    else:
                                        break
                                for module in mylist:
                                    if os.path.exists(filename + '-' + module.rstrip() + '.xml'):
                                        with open(filename + '-' + module.rstrip() + '.xml') as startup_file:
                                            while not re.search('<' + xml_heads[n] + r'.*>', next(startup_file)):
                                                pass
                                            for line in startup_file:
                                                if '</' + xml_heads[n] + '>' not in line:
                                                    temp.write(line)
                                                else:
                                                    break
                                temp.write('</' + xml_heads[n] + '>\n')
                        line = next(config_file)
                except StopIteration:
                    pass
    elif args.mode =='reset':
        if os.path.exists(filename + '-' + args.post + '.xml'):
            os.remove(filename + '-' + args.post + '.xml')

def replace(file_path, old_str, new_str):  
    try:  
        f = open(file_path,'r+')  
        all_lines = f.readlines()  
        f.seek(0)  
        f.truncate()  
        for line in all_lines:  
            line = line.replace(old_str, new_str)  
            f.write(line)  
        f.close()  
    except Exception,e:  
        print e  

range_set()
#replace('org-openroadm-device.yang', 'http://org/openroadm/device', 'http://org/openroadm/device/' + args.post)