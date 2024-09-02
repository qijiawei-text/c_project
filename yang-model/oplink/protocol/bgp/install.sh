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
echo "###### install BGP"
/usr/local/bin/sysrepoctl -i openconfig-bgp-types.yang -s ../.:../../common; return_val
/usr/local/bin/sysrepoctl -i openconfig-bgp.yang -s ../.:../../common; return_val
/usr/local/bin/import_xml --import=openconfig-bgp-startup-$SUB_TYPE.xml --datastore startup --module openconfig-bgp; return_val
exit 0
