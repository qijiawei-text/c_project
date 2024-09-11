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

if [ "$POST" == "OCS" ] || [ "$POST" == "OCS-32X32" ]; then
    echo "ignore install lldp and rstp yang"
    exit 0
fi

echo "###### install L2"
/usr/local/bin/sysrepoctl -i openconfig-lldp.yang -s ../.:../common; return_val
/usr/local/bin/sysrepoctl -i com-oplink-lldp-ext.yang -s ../.:../common; return_val
/usr/local/bin/import_xml --import=openconfig-lldp-startup.xml --datastore startup --module openconfig-lldp; return_val
/usr/local/bin/sysrepoctl -i org-openroadm-rstp.yang -s ../common; return_val
/usr/local/bin/import_xml --import=org-openroadm-rstp-startup.xml --datastore startup --module org-openroadm-rstp; return_val


if [ "$POST" == "MEWU" ] ; then
  cd bgp; return_val
  ./install.sh $SUB_TYPE
  cd -
fi

exit 0
