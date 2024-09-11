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

# import netopeer2 yang and config
echo "###### install NETCONF"
cd netopeer2
# using opensource netopeer2 setup.sh to install yang-model
./setup.sh
# this function will be implemented by ietf-keystore-startup.xml and ietf-netconf-server-startup.xml
#./merge_hostkey.sh
#./merge_config.sh
echo "###### import NETCONF"
/usr/local/bin/sysrepocfg --import=ietf-keystore-startup.xml --datastore startup --module ietf-keystore  -f xml; return_val
/usr/local/bin/sysrepocfg --import=ietf-netconf-server-startup.xml --datastore startup --module ietf-netconf-server -f xml; return_val
/usr/local/bin/sysrepocfg --import=ietf-netconf-acm-startup.xml --datastore startup --module ietf-netconf-acm; return_val
cd ..

echo "###### import interface yang"
/usr/local/bin/sysrepoctl -i org-openroadm-interfaces.yang; return_val
/usr/local/bin/sysrepoctl -i openconfig-platform-types.yang -s ../common; return_val
/usr/local/bin/sysrepoctl -i openconfig-transport-types.yang; return_val

echo "###### install ipc"
/usr/local/bin/sysrepoctl -i com-oplink-ipc.yang; return_val

exit 0
