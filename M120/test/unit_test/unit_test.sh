#!/bin/bash
#Unit Test Bash script

SYS_TYPE=$(echo $1 | tr [A-Z] [a-z])
SUB_TYPE=$2
ila=ILA

####  check execute result
function return_val()
{
    val=`echo $?`
    if test $val -ne 0;then
        echo "######### ERROR..."
        echo "##### stop sysrepod "
        sudo pkill -9 sysrepod
        echo "##### stop csm "
        sudo pkill -9 csm
        if [[ $SYS_TYPE == "ops" && $SUB_TYPE == "8CH-OPS" ]]
        then
            echo "##### uninstall yang model "
            cd ../../../../yang-model/oplink/Device-Model/ops/ ; return_val
            sudo ./uninstall.sh $SYS_TYPE $SUB_TYPE ; return_val
            cd ../../../../apps/test/unit_test/ ; return_val
        else
            echo "##### uninstall yang model "
            cd ../../../../yang-model/oplink/Device-Model/optical/ ; return_val
            sudo ./uninstall.sh $SYS_TYPE $SUB_TYPE ; return_val
            cd ../../../../apps/test/unit_test/ ; return_val
        fi
        echo "#------------------------------------------------------#"

        echo "                 #### ERROR ####"
        
        echo "#------------------------------------------------------#"
        exit -1
    fi
}

 echo "##### uninstall yang model "
 cd ../../../yang-model/oplink/Device-Model/optical
 sudo ./uninstall.sh rdm 2DROADM
 sudo ./uninstall.sh edfa ILA
 sudo ./uninstall.sh toa PAM4
 cd ../../../../apps/test/unit_test/

date
if [[ $SYS_TYPE == "ops" && $SUB_TYPE == "8CH-OPS" ]]
then
    echo "##### install yang model "
    cd ../../../yang-model/oplink/Device-Model/ops
    sudo ./install.sh $SYS_TYPE $SUB_TYPE ; return_val
else
    echo "##### install yang model "
    cd ../../../yang-model/oplink/Device-Model/optical
    sudo ./install.sh $SYS_TYPE $SUB_TYPE ; return_val 
fi

echo "##### start sysrepod "
cd ../../../../apps_pub/sysrepo/build/src/
sudo gnome-terminal -x bash -c "sysrepod -d ; return_val"

echo "##### start csm "
cd ../../../../apps/csm/build/
sudo gnome-terminal -x bash -c "./csm -d ; return_val"

sleep 20s

cd ../../test/build/unit_test
if [[ $SYS_TYPE == "rdm" && $SUB_TYPE == "2DROADM" ]]
then
    echo "##### run DSC_system_test "
    ./DSC_system_test ; return_val
    sleep 5s
    echo "##### run DSC_ocm_test "
    ./DSC_ocm_test ; return_val
    sleep 5s
    echo "##### run DSC_edfa_test "
    ./DSC_edfa_test ; return_val
    sleep 5s
    echo "##### run DSC_port_test "
    ./DSC_port_test ; return_val
    sleep 5s
    echo "##### run DSC_waveplan_test "
    ./DSC_waveplan_test ; return_val
    sleep 3s
    echo "##### run DSC_connection_test "
    ./DSC_connection_test ; return_val
    sleep 5s
    echo "##### run DSC_alarm_test "
    ./DSC_alarm_test ; return_val
    sleep 3s
    echo "##### run DSC_osc_test "
    ./DSC_osc_test ; return_val
    sleep 5s
    echo "##### run DSC_tca_test "
    ./DSC_tca_test ; return_val
fi

if [[ $SYS_TYPE == "edfa" && $SUB_TYPE == "ILA" ]]
then
    echo "##### run DSC_system_test "
    ./DSC_system_test ; return_val
    sleep 5s
    echo "##### run DSC_edfa_test "
    ./DSC_edfa_test ; return_val
    sleep 5s
    echo "##### run DSC_alarm_test "
    ./DSC_alarm_test ; return_val
    sleep 5s
    echo "##### run DSC_protocols_test "
    ./DSC_system_test ; return_val
    sleep 5s
    echo "##### run DSC_telemetry_test "
    ./DSC_system_test ; return_val
fi

if [[ $SYS_TYPE == "toa" && $SUB_TYPE == "PAM4" ]]
then
    echo "##### run DSC_system_test "
    ./DSC_system_test ; return_val
    sleep 5s
    echo "##### run DSC_port_test "
    ./DSC_port_test ; return_val
    sleep 5s
    echo "##### run DSC_ocm_test "
    ./DSC_ocm_test ; return_val
    sleep 5s
    echo "##### run DSC_edfa_test "
    ./DSC_edfa_test ; return_val
    sleep 5s
    echo "##### run DSC_waveplan_test "
    ./DSC_waveplan_test ; return_val
    sleep 5s
    echo "##### run DSC_connection_test "
    ./DSC_connection_test ; return_val
    sleep 5s
    echo "##### run DSC_alarm_test "
    ./DSC_alarm_test ; return_val
    sleep 5s
    echo "##### run DSC_pm_test "
    ./DSC_pm_test ; return_val
    sleep 5s
    echo "##### run DSC_pam4_test "
    ./DSC_pam4_test ; return_val
    sleep 5s
    echo "##### run DSC_control_loop_test "
    ./DSC_control_loop_test ; return_val
    sleep 5s
    echo "##### run DSC_tdcm_test "
    ./DSC_tdcm_test ; return_val
fi

if [[ $SYS_TYPE == "ops" && $SUB_TYPE == "8CH-OPS" ]]
then
     echo "##### run DSC_ops_test "
    ./DSC_ops_test ; return_val
fi

echo "##### stop sysrepod "
sudo pkill -9 sysrepod

echo "##### stop csm "
sudo pkill -9 csm

if [[ $SYS_TYPE == "ops" && $SUB_TYPE == "8CH-OPS" ]]
then
    echo "##### uninstall yang model "
    cd ../../../../yang-model/oplink/Device-Model/ops/ ; return_val
    sudo ./uninstall.sh $SYS_TYPE $SUB_TYPE ; return_val
    cd ../../../../apps/test/unit_test/ ; return_val
else
    echo "##### uninstall yang model "
    cd ../../../../yang-model/oplink/Device-Model/optical/ ; return_val
    sudo ./uninstall.sh $SYS_TYPE $SUB_TYPE ; return_val
    cd ../../../../apps/test/unit_test/ ; return_val
fi

echo "#------------------------------------------------------#"

    echo "      #### Test successfully completed ####"
    
echo "#------------------------------------------------------#"