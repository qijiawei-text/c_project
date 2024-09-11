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

/usr/local/bin/sysrepoctl -i ietf-snmp.yang -s ../.:../common:../system; return_val
#features supported(sysrepoctl --feature-enbale=?? --module=ietf-snmp):
#proxy;notification-filter;sshtm;tlstm;tsm;


/usr/local/bin/import_xml --import=ietf-snmp-startup.xml --datastore startup --module ietf-snmp; return_val

exit 0
