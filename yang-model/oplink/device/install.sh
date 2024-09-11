#!/bin/bash

####  check execute result
function return_val()
{
    val=`echo $?`
    if test $val -ne 0;then
        echo "######### `pwd` ERROR..."
#        python pre-install.py $POST reset
    #    exit -1
    fi
}

if [ $# -ne 1 ]; then
    echo "use age : ./install.sh <sub_type>"
    echo "    sub_type :OCS | HCF | OLSP | OTDR"
    exit 1
fi

SUB_TYPE=$1

POST=$SUB_TYPE

SUBDIRS='../common ../fmpm ../system ../oc-system ../platform ../optical ../protocol ../snmp'

echo "############ INSTALL system type: product type:$SUB_TYPE"

echo "###### pre-process yang model and start up files"
#python pre-install.py $POST set

for i in $SUBDIRS; do 
    echo "+++++++++ enter $i"
    cd $i;return_val
    if [[ "../optical" == "$i" ]]; then
     #customized install script for each device type
      ./install-$SUB_TYPE.sh; return_val
      cd -;return_val
      echo "--------- exit $i"
      continue;
    fi

    ./install.sh $SUB_TYPE; return_val
    cd -;return_val
    echo "--------- exit $i"
done

if [ "$POST" == "OCS" ] || [ "$POST" == "OCS-32X32" ]; then
    POST="OCS"
fi

echo "###### install com-oplink-parameter-range"
/usr/local/bin/sysrepoctl -i com-oplink-parameter-range.yang -s ../common; return_val
/usr/local/bin/import_xml --import=com-oplink-parameter-range-startup-$POST.xml --datastore startup --module com-oplink-parameter-range; return_val

echo "###### install org-openroadm-device"
/usr/local/bin/sysrepoctl -i org-openroadm-device.yang -s ../common:../optical:../system; return_val
echo "###### install com-oplink-device"
/usr/local/bin/sysrepoctl -i com-oplink-device.yang -s ../common:../optical:../system; return_val

SUBDIRS='../extension'

for i in $SUBDIRS; do
    echo "+++++++++ enter $i"
    cd $i
    ./install.sh $SUB_TYPE
    cd -
    echo "--------- exit $i"
done

echo "import data"
/usr/local/bin/import_xml --import=org-openroadm-device-startup-$POST.xml --datastore startup --module org-openroadm-device; return_val
/usr/local/bin/import_xml --import=com-oplink-device-startup-$POST.xml --datastore startup --module com-oplink-device; return_val

#python pre-install.py $POST reset
exit 0
