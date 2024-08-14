#!/bin/bash
# this is check crond   
# by author rivers on 2021-9.23      
# 定义一个变量名   
# name=md   
# login_cmd=$(curl -vk  https://www.ruanyifeng.com/blog/2019/09/curl-reference.html 2>&1 1>error.text)
# login_cmd_result=`echo "$login_cmd" >error.text`
# num=$(ps -ef|grep $name|grep -vc grep)
# echo $login_cmd
# if [ $num -eq 1 ];then       
#     echo "$num running!"   
# else      
#     echo "$num is not running!"
# fi   

YAML_VERSION="0.01.0001"  #format: X.YY.ZZZZ
YAML_VERSION_FILE="./yaml_version.txt"

echo "build time: $(date "+%Y-%m-%d %H:%M:%S")" > $YAML_VERSION_FILE
echo "version: $YAML_VERSION" >> $YAML_VERSION_FILE

function helpinfo() {
	echo "useage: ./yang2yaml.sh [paras]"
	echo "   ./yang2yaml.sh"
	echo "   ./yang2yaml.sh single <yang_file>"
	echo "   ./yang2yaml.sh clean"
	exit 1
}

helpinfo