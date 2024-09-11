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
POST=$SUB_TYPE

if [ "$POST" != "OLT" ] && [ "$POST" != "ILA" ]; then
   exit 0 
fi

echo "###### install network"
/usr/local/bin/sysrepoctl -i ietf-network.yang -s ../common; return_val
/usr/local/bin/sysrepoctl -i ietf-network-topology.yang -s ../common:./; return_val
/usr/local/bin/sysrepoctl -i ietf-network-state.yang -s ../common; return_val
/usr/local/bin/sysrepoctl -i ietf-network-topology-state.yang -s ../common:./; return_val
/usr/local/bin/sysrepoctl -i org-openroadm-link.yang -s ../common:./; return_val
/usr/local/bin/sysrepoctl -i org-openroadm-network-topology.yang -s ../common:./; return_val
echo "###### import cfg network"
/usr/local/bin/sysrepocfg --import=ietf-network-startup-$POST.xml --datastore startup --module ietf-network; return_val



exit 0
