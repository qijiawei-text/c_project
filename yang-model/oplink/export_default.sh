#!/bin/bash
#this scrpit is used to export default value.

function return_val()
{
    val=`echo $?`
    if test $val -ne 0;then
        echo "######### `pwd` ERROR..."
        exit -1
    fi
}

SUB_TYPE=$1
PRODUCT_NAME=$1
cd device;

echo "###### start to uninstall $SUB_TYPE yang-files"
sudo ./uninstall.sh $SUB_TYPE
sudo ./uninstall.sh NR1004
sudo ./uninstall.sh NR1004-OPS-4

echo "###### start to in $SUB_TYPE yang-files"
sudo ./install.sh $SUB_TYPE;
cd ..

AMP_yang_module_array=("ietf-system" "ietf-snmp" "openconfig-system" "org-openroadm-device" "openconfig-optical-attenuator" "openconfig-optical-amplifier" "openconfig-transport-line-protection" "org-openroadm-tca" "openconfig-telemetry" "org-openroadm-syslog" "com-oplink-control-loop-OLSP" "openconfig-channel-monitor" "openconfig-lldp" "openconfig-platform")
OPS_yang_module_array=("ietf-system" "ietf-snmp" "openconfig-system" "org-openroadm-device" "openconfig-transport-line-protection" "org-openroadm-tca" "openconfig-telemetry" "org-openroadm-syslog" "openconfig-lldp" "openconfig-platform")

function function_export() {
	echo "export yang '$1' to ./$SUB_TYPE/$1.json"
	sudo /usr/local/bin/sysrepocfg -X -e report-all -m $1 -d startup -f json > $1.json
}

mkdir -p $SUB_TYPE
cd $SUB_TYPE
if [ "$PRODUCT_NAME" == "NR1004" ]; then
    for item in "${AMP_yang_module_array[@]}"
    do
        function_export $item
    done
elif [ "$PRODUCT_NAME" == "NR1004-OPS-4" ]; then
    for item in "${OPS_yang_module_array[@]}"
    do
        function_export $item
    done
else 
    echo "only support AMP and OPS"
fi
cd ..
exit 0
