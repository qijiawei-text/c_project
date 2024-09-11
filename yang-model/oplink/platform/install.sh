#!/bin/bash

####  check execute result
function return_val()
{
    val=`echo $?`
    if test $val -ne 0;then
        echo "######### `pwd` ERROR..."
#        exit -1
    fi
}

SUB_TYPE=$1

POST=$SUB_TYPE

echo "###### install platform"
/usr/local/bin/sysrepoctl -i openconfig-platform.yang -s ../common:../oc-system; return_val
/usr/local/bin/sysrepoctl -i com-oplink-platform.yang -s ../common:../oc-system; return_val
/usr/local/bin/sysrepoctl -i openconfig-transport-line-common.yang -s ../common:../oc-system; return_val
/usr/local/bin/sysrepoctl -i com-oplink-platform-ext.yang -s ../common:../oc-system; return_val

echo "###### install cpu, power, fan"
/usr/local/bin/sysrepoctl -i openconfig-platform-psu.yang -s ../common:../oc-system; return_val
/usr/local/bin/sysrepoctl -i openconfig-platform-cpu.yang -s ../common:../oc-system; return_val
/usr/local/bin/sysrepoctl -i openconfig-platform-fan.yang -s ../common:../oc-system; return_val
/usr/local/bin/sysrepoctl -i openconfig-platform-transceiver.yang -s ../common:../oc-system; return_val

echo "###### import platform startup"
/usr/local/bin/import_xml --import=openconfig-platform-startup-$POST.xml --datastore startup --module openconfig-platform; return_val
/usr/local/bin/import_xml --import=com-oplink-platform-startup-$POST.xml --datastore startup --module com-oplink-platform; return_val
exit 0
