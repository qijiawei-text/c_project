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
echo "###### install OCS"

/usr/local/bin/sysrepoctl -i openconfig-transport-line-connectivity.yang -s ../common; return_val

/usr/local/bin/import_xml --import=openconfig-transport-line-connectivity-startup.xml --datastore startup --module openconfig-transport-line-connectivity; return_val


exit 0
