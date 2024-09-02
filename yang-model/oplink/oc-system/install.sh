#!/bin/bash

####  check execute result
function return_val()
{
    val=`echo $?`
    if test $val -ne 0;then
        echo "######### `pwd` ERROR..."
        #exit -1
    fi
}

SUB_TYPE=$1
POST=$SUB_TYPE;

if [ "$POST" == "OTDR" ]; then
    exit 0
fi

echo "###### install platform"
/usr/local/bin/sysrepoctl -i openconfig-alarm-types.yang -s ../common:../platform; return_val
/usr/local/bin/sysrepoctl -i openconfig-system-logging.yang -s ../common:../platform; return_val
/usr/local/bin/sysrepoctl -i openconfig-system.yang -s ../.:../common:../platform; return_val
/usr/local/bin/sysrepoctl -i com-oplink-system-ext.yang -s ../.:../common:../platform; return_val
/usr/local/bin/sysrepoctl -i com-oplink-system-logging.yang -s ../common:../platform; return_val
echo "###### import platform startup"
/usr/local/bin/import_xml --import=openconfig-system-startup.xml --datastore startup --module openconfig-system; return_val

echo "###### install acl"
/usr/local/bin/sysrepoctl -i openconfig-acl.yang -s ./:../common:../platform; return_val
/usr/local/bin/import_xml --import=openconfig-acl-startup.xml --datastore startup --module openconfig-acl; return_val

exit 0
