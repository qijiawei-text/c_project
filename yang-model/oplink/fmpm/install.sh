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

if [ "$POST" == "OCS" ] || [ "$POST" == "OCS-32X32" ]; then
    POST="OCS"
fi

if [ "$POST" == "OTDR" ]; then
    exit 0
fi

/usr/local/bin/sysrepoctl -i org-openroadm-alarm.yang -s ../.:../common; return_val
/usr/local/bin/sysrepoctl -i org-openroadm-pm.yang -s ../.:../common; return_val
/usr/local/bin/sysrepoctl -i org-openroadm-tca.yang -s ../.:../common; return_val
/usr/local/bin/sysrepoctl -i com-oplink-alarm.yang -s ../common; return_val
/usr/local/bin/sysrepoctl -i com-oplink-pm.yang -s ../common; return_val
/usr/local/bin/sysrepoctl -i openconfig-telemetry-types.yang -s ../common; return_val
/usr/local/bin/sysrepoctl -i openconfig-telemetry.yang -s ../common; return_val

/usr/local/bin/sysrepoctl -i openconfig-transport-pms.yang -s ../common; return_val
/usr/local/bin/sysrepoctl -i com-oplink-ocpms.yang -s ../common; return_val

echo "###### install tca for $POST"
/usr/local/bin/import_xml --import=org-openroadm-tca-startup-$POST.xml --datastore startup --module org-openroadm-tca; return_val

echo "###### install alarm for module name"
/usr/local/bin/import_xml --import=com-oplink-alarm-startup-$POST.xml --datastore startup --module com-oplink-alarm; return_val

echo "###### install pm for module name"
/usr/local/bin/import_xml --import=com-oplink-pm-startup-$POST.xml --datastore startup --module com-oplink-pm; return_val

exit 0
