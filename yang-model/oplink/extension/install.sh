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


echo "###### install Oplink extensions"
/usr/local/bin/sysrepoctl -i com-oplink-swdl-ext.yang -s ../common:../system:../device; return_val
/usr/local/bin/sysrepoctl -i com-oplink-physical-ext.yang -s ../common:../system:../device; return_val

exit 0
