#!/bin/bash
# Guangjun Guo, 2020/09/08, optimize based on Jimmy's j2s.sh
# https://github.com/oplinkoms/yang2swagger
#CUR_DIR=`pwd`
## install JAVA jdk
#sudo apt install openjdk-17-jdk
#export JAVA_HOME=/usr/lib/jvm/java-17-openjdk-amd64
#export PATH=$JAVA_HOME/bin:$PATH

## install swagger-generator-cli.jar tool
#wget https://github.com/bartoszm/yang2swagger/releases/download/1.2/swagger-generator-cli-1.2-executable.jar
#sudo cp swagger-generator-cli-1.2-executable.jar /usr/lib/jvm/
#sudo ln -s /usr/lib/jvm/swagger-generator-cli-1.2-executable.jar /usr/lib/jvm/swagger-generator-cli.jar
SWAGGER_GEN_TOOL=/usr/lib/jvm/swagger-generator-cli.jar

OUTPUT_DIR="../yaml_tmp"  # temporary yaml files
YANG_DIR="."
BASIC_YANG_FILES=`ls ../basic_yang/*.yang`

YAML_VERSION="0.01.0001"  #format: X.YY.ZZZZ
YAML_VERSION_FILE="$OUTPUT_DIR/yaml_version.txt"
YAML_TARGET_DIR="../yaml"   # target yaml directory
YANG_MODEL_ROOT_DIR="../../../../yang-model"

#manual example command:
# java -jar ~/.m2/repository/com/mrv/yangtools/swagger-generator-cli/1.1.14-SNAPSHOT/swagger-generator-cli-1.1.14-SNAPSHOT-executable.jar module openconfig-optical-amplifier -output ../yaml/openconfig-optical-amplifier.yaml

function replaceBaseYangFiles() {
	#for file in $(ls ../basic_yang/*.yang)
	for file in $BASIC_YANG_FILES
	do
		filename=${file##*/}
		rm -rf $filename
		cp -rf $file  $filename
	done
}

function commonYang2Yaml() {
	#
	yang_files=$1

	if [ ! "$#" -eq "1" ]; then
		echo "Invalid parameters"
		exit -1
	fi

	#create temporary output directory
	mkdir -p $OUTPUT_DIR
	
	for file in $yang_files 
	do
		echo "*********begin: $file **********"
		module_name=${file%.*}
		#echo "module_name: $module_name"
	
		java -jar $SWAGGER_GEN_TOOL -yang-dir $YANG_DIR module ${module_name} -output $OUTPUT_DIR/${module_name}.yaml
		#java -jar $SWAGGER_GEN_TOOL module ${module_name} -output $OUTPUT_DIR/${module_name}.yaml
	
		#check result
		val=`echo $?`
		if test $val -ne 0;then
			echo "*********error in  $file   "
			exit -1
		fi
		echo "*********end: $file **********"
	done
}

function convertSdnYang2Yaml() {
# multiple lines comment
#:<<!
#!
	#1) independent optical modules (EDFA/OPS/OCM/OTDR/Auto control)
	yang_models="openconfig-optical-amplifier.yang
				 openconfig-transport-line-protection.yang
				 openconfig-channel-monitor.yang
                 openconfig-optical-attenuator.yang
				 openconfig-gnoi-otdr.yang
				 com-oplink-connectivity-validation.yang
				 openconfig-wavelength-router.yang
				 openconfig-transport-line-connectivity.yang
				 com-oplink-dfb.yang
				 com-oplink-laser-safty.yang"
	#
	# convert yang to yaml
	commonYang2Yaml "$yang_models"
	
	#2) optical modules that augument to com-oplink-optical.yang
	yang_models="com-oplink-optical.yang"
	# convert yang to yaml
	commonYang2Yaml "$yang_models"

	#3) FMPM (Alarm/PM/TCA)
	yang_models="org-openroadm-alarm.yang
				 org-openroadm-pm.yang
				 org-openroadm-tca.yang"
	# convert yang to yaml
	commonYang2Yaml "$yang_models"

	#4) RPCs (database/operations/file transfer/swdl/oplink rpc)
	yang_models="org-openroadm-database.yang
				 org-openroadm-de-operations.yang
				 org-openroadm-file-transfer.yang
				 org-openroadm-swdl.yang
				 com-oplink-rpc.yang"
	# convert yang to yaml
	commonYang2Yaml "$yang_models"

	#5) SYSTEM (system/tacacs/syslog/service/auth)
	yang_models="ietf-system.yang
				 org-openroadm-syslog.yang
				 com-oplink-service.yang
				 com-oplink-auth.yang"
	# convert yang to yaml
	commonYang2Yaml "$yang_models"

	#6) DEVICE
	yang_models="org-openroadm-device.yang"
	# convert yang to yaml
	commonYang2Yaml "$yang_models"
    
    #7) oc-system
	yang_models="openconfig-acl.yang
                 openconfig-system.yang"
	# convert yang to yaml
	commonYang2Yaml "$yang_models"
    
    #8) platform (Parse error when yaml is loaded by https://editor.swagger.io/, TODO)
	#yang_models="openconfig-platform.yang"
	# convert yang to yaml
	#commonYang2Yaml "$yang_models"
}

function deleteTmpFiles() {
	#delete old yang files
	rm -rf *.yang
	
	#delete temporary target output
	rm -rf $OUTPUT_DIR
}

function copyYang2CurrentDir() {
	#delete old files
	deleteTmpFiles
	
	#copy 400GZR yang
	#yang_source_dir="common common/netopeer2 device extension fmpm oc-system optical platform protocol protocol/bgp protocol/ospf snmp system "
    yang_source_dir="common common/netopeer2 device extension fmpm oc-system optical platform protocol snmp system "
	for file_dir in $yang_source_dir
	do
		cp  $YANG_MODEL_ROOT_DIR/oplink/$file_dir/*.yang .
		#check result
		val=`echo $?`
		if test $val -ne 0;then
			echo "*********  $file_dir is not existed   "
			exit -1
		fi
	done 
}

function copyYaml2TargetDir() {

	echo "build time: $(date "+%Y-%m-%d %H:%M:%S")" > $YAML_VERSION_FILE
	echo "version: $YAML_VERSION" >> $YAML_VERSION_FILE
	if [ "$1" == "single" ]; then
		#replace single yaml
		echo "copy single yaml"
	else
		#default replace all
		rm -rf $YAML_TARGET_DIR/*
	fi

	cp -rf $OUTPUT_DIR/* $YAML_TARGET_DIR
}

function exportYaml() {

	#step1 copy all yang to current dir
	copyYang2CurrentDir

	#step2 replace base yang files
	replaceBaseYangFiles

	#step3 convert oplink customized yang to yaml
	convertSdnYang2Yaml
	
	#step4 copy targetfile
	copyYaml2TargetDir
	
	#step5 delete all temporary files
	deleteTmpFiles

}

function exportSingleYaml() {
	#step1 copy all yang to current dir
	copyYang2CurrentDir

	#step2 replace base yang files
	replaceBaseYangFiles
	
	#step3 check yang file
	if [ ! -f $1 ]; then 
		echo "yang file is not existed. yang: $1"
		deleteTmpFiles
		exit 1
	fi

	#step4 convert single yang to yaml
	commonYang2Yaml $1
	
	#step5 copy single targetfile
	copyYaml2TargetDir "single"
	
	#step6 delete all temporary files
	deleteTmpFiles
	
}

function helpinfo() {
	echo "useage: ./yang2yaml.sh [paras]"
	echo "   ./yang2yaml.sh"
	echo "   ./yang2yaml.sh single <yang_file>"
	echo "   ./yang2yaml.sh clean"
	exit 1
}

if [ "$#" -eq "0" ]; then
	exportYaml
elif [ "$#" == "1" ]; then
	if [ "$1" == "clean" ]; then
		deleteTmpFiles
	else
		helpinfo
	fi
elif [ "$#" == "2" ] ; then
	if [ "$1" == "single" ]; then
		exportSingleYaml "$2"
	else
		helpinfo
	fi
else  
	helpinfo
fi

echo "********* OK **********"

