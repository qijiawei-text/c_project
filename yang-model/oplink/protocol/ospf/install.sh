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
echo "###### install ospfv2"
/usr/local/bin/sysrepoctl -i openconfig-ospf-types.yang -s ../.:../../common; return_val
/usr/local/bin/sysrepoctl -i openconfig-ospfv2.yang -s ../.:../../common; return_val
/usr/local/bin/import_xml --import=openconfig-ospfv2-startup-$SUB_TYPE.xml --datastore startup --module openconfig-ospfv2; return_val
exit 0
