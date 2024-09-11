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
    POST="OCS"
else
    POST="COMMON"
fi

echo "###### install rpc"
/usr/local/bin/sysrepoctl -i com-oplink-rpc.yang -s ../common; return_val
/usr/local/bin/sysrepoctl -i org-openroadm-database.yang -s ../common; return_val
/usr/local/bin/sysrepoctl -i org-openroadm-de-operations.yang -s ../common; return_val
/usr/local/bin/sysrepoctl -i org-openroadm-file-transfer.yang -s ../common; return_val
/usr/local/bin/sysrepoctl -i org-openroadm-swdl.yang -s ../common; return_val

echo "###### install service "
/usr/local/bin/sysrepoctl -i com-oplink-service.yang -s ../common; return_val
/usr/local/bin/import_xml --import=com-oplink-service-startup-$POST.xml --datastore startup --module com-oplink-service; return_val

echo "###### install system"
/usr/local/bin/sysrepoctl -i ietf-system.yang -s ../common; return_val
/usr/local/bin/sysrepoctl -i ietf-aaa-tacacs.yang -s ../common; return_val
/usr/local/bin/sysrepoctl -c ietf-system -e timezone-name; return_val
/usr/local/bin/sysrepoctl -c ietf-system -e authentication; return_val
/usr/local/bin/sysrepoctl -c ietf-system -e local-users; return_val
/usr/local/bin/sysrepoctl -c ietf-system -e radius; return_val
/usr/local/bin/sysrepoctl -c ietf-aaa-tacacs -e tacacs; return_val
/usr/local/bin/sysrepoctl -c ietf-system -e dns-udp-tcp-port; return_val
/usr/local/bin/sysrepoctl -c ietf-system -e ntp; return_val
/usr/local/bin/sysrepoctl -c ietf-system -e ntp-udp-port; return_val

echo "###### install syslog"
/usr/local/bin/sysrepoctl -i org-openroadm-syslog.yang -s ../common; return_val
/usr/local/bin/sysrepoctl -c org-openroadm-syslog -e selector-sevop-config; return_val
/usr/local/bin/sysrepoctl -c org-openroadm-syslog -e selector-match-config; return_val

/usr/local/bin/import_xml --import=org-openroadm-syslog-startup.xml --datastore startup --module org-openroadm-syslog; return_val

echo "###### install auth"
/usr/local/bin/sysrepoctl -i com-oplink-auth.yang -s ../common; return_val
/usr/local/bin/import_xml --import=com-oplink-auth-startup.xml --datastore startup --module com-oplink-auth; return_val

echo "###### install system xml"
/usr/local/bin/import_xml --import=ietf-system-startup.xml --datastore startup --module ietf-system; return_val



exit 0
