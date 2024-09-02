#!/bin/bash
if [ $# -ne 1 ]; then
    echo "use age : ./install.sh <sub_type>"
    echo "    sub_type : ROADM | ILA | OLSP | NR1004"
    exit 1
fi

SUB_TYPE=$1

#uninstall augument module first
SUBDIRS='../optical'
for i in $SUBDIRS; do
    echo "+++++++++ enter $i"
    cd $i
    ./uninstall-$SUB_TYPE.sh
    cd -
    echo "--------- exit $i"
done

SUBDIRS='../extension'
for i in $SUBDIRS; do
    echo "+++++++++ enter $i"
    cd $i
    ./uninstall.sh 
    cd -
    echo "--------- exit $i"
done

/usr/local/bin/sysrepoctl -u com-oplink-parameter-range;
/usr/local/bin/sysrepoctl -u com-oplink-device;
/usr/local/bin/sysrepoctl -u org-openroadm-device;

SUBDIRS='../oc-system ../platform ../snmp ../fmpm ../system ../protocol ../common'
for i in $SUBDIRS; do 
    echo "+++++++++ enter $i"
    cd $i
    ./uninstall.sh $SUB_TYPE
    cd -
    echo "--------- exit $i"
done

exit 0
