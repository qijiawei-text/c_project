#!/bin/bash

####  check execute result
function return_val()
{
    val=`echo $?`
    if test $val -ne 0;then
        echo "#########  `pwd` is ERROR..."
        exit -1
    fi
}

### build test_telemetry
echo "+++++++++ enter test_telemetry build"
cd function_test; return_val
    cd test_telemetry; return_val
        sudo rm build -fr;return_val
        mkdir -p  build; return_val
        cd build; return_val
            cmake .. ; return_val
            make -j4;return_val
        cd ..; return_val
    cd ..;return_val
cd ..;return_val
echo "--------- exit test_telemetry build"

exit 0

