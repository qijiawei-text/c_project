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

echo "###### install OCM"
/usr/local/bin/sysrepoctl -i openconfig-channel-monitor.yang -s ../common; return_val
/usr/local/bin/sysrepoctl -i com-oplink-ocm-ext.yang -s ../common; return_val
/usr/local/bin/import_xml --import=openconfig-channel-monitor-startup-HCF.xml --datastore startup --module openconfig-channel-monitor; return_val

echo "###### install attenuator"
/usr/local/bin/sysrepoctl -i openconfig-optical-attenuator.yang -s ../common:../platform:../oc-system; return_val
/usr/local/bin/import_xml --import=openconfig-optical-attenuator-startup-HCF.xml --datastore startup --module openconfig-optical-attenuator; return_val

echo "###### install amplifier"
/usr/local/bin/sysrepoctl -i openconfig-optical-amplifier.yang -s ../common:../platform:../oc-system; return_val
/usr/local/bin/sysrepoctl -i com-oplink-amplifier-ext.yang -s ../common:../platform:../oc-system; return_val
/usr/local/bin/import_xml --import=openconfig-optical-amplifier-startup-HCF.xml --datastore startup --module openconfig-optical-amplifier; return_val

echo "###### install control loop"
/usr/local/bin/sysrepoctl -i com-oplink-control-loop.yang -s ../common; return_val
/usr/local/bin/import_xml --import=com-oplink-control-loop-startup-1-HCF.xml --datastore startup --module com-oplink-control-loop; return_val

/usr/local/bin/sysrepoctl -i com-oplink-control-loop-i.yang -s ../common; return_val
/usr/local/bin/import_xml --import=com-oplink-control-loop-i-startup-1-HCF.xml --datastore startup --module com-oplink-control-loop-i; return_val

echo "###### install optical"
/usr/local/bin/sysrepoctl -i com-oplink-optical.yang -s ../common; return_val
/usr/local/bin/sysrepoctl --enable-feature laser-safty -c com-oplink-optical; return_val
/usr/local/bin/sysrepoctl -i com-oplink-optical-i.yang -s ../common; return_val

echo "###### install connectivit veridation"
/usr/local/bin/sysrepoctl -i com-oplink-connectivity-validation.yang -s ../common; return_val

/usr/local/bin/import_xml --import=com-oplink-optical-startup-HCF.xml --datastore startup --module com-oplink-optical; return_val
/usr/local/bin/import_xml --import=com-oplink-optical-i-startup-HCF.xml --datastore startup --module com-oplink-optical-i; return_val

echo "###### install otdr"
/usr/local/bin/sysrepoctl -i openconfig-gnoi-otdr.yang -s ../common; return_val
/usr/local/bin/import_xml --import=openconfig-gnoi-otdr-startup-HCF.xml --datastore startup --module openconfig-gnoi-otdr; return_val
/usr/local/bin/sysrepoctl -i com-oplink-gnoi-otdr.yang -s ../common; return_val
/usr/local/bin/import_xml --import=com-oplink-gnoi-otdr-startup.xml --datastore startup --module com-oplink-gnoi-otdr; return_val

echo "###### install waveplan"
/usr/local/bin/sysrepoctl -i com-oplink-waveplan.yang -s ../common; return_val
/usr/local/bin/import_xml --import=com-oplink-waveplan-startup-150G.xml --datastore startup --module com-oplink-waveplan; return_val

echo "###### install DFB"
/usr/local/bin/sysrepoctl -i com-oplink-dfb.yang -s ../common; return_val
/usr/local/bin/import_xml --import=com-oplink-dfb-startup-HCF.xml --datastore startup --module com-oplink-dfb; return_val

exit 0
