#!/bin/bash
#Auto Test Bash script

SYS_TYPE=$(echo $1 | tr [A-Z] [a-z])
SUB_TYPE=$2
ila=ILA

####  check execute result
function return_val()
{
    val=`echo $?`
    if test $val -ne 0;then
        echo "######### ERROR..."
        exit -1
    fi
}

date
echo "##### update "
cd ../../../
#./update.sh debug ; return_val

echo "##### build "
#./build.sh debug ; return_val

echo "##### run unit test "
cd apps/test/unit_test/
./unit_test.sh rdm 2DROADM > rdm.log ; return_val
./unit_test.sh edfa ILA > edfa.log ; return_val
./unit_test.sh toa PAM4 > toa.log; return_val