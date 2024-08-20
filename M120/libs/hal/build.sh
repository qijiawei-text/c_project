#!/bin/bash

### $1=copy for pack

####  check execute result
function return_val()
{
    val=`echo $?`
    if test $val -ne 0;then
        echo "#########  `pwd` is ERROR..."
        exit -1
    fi
}

echo $PRJ_NAME


if [ "$1" == "copy" ]; then
    sudo cp -f ./RegisterFile-* $ETC_DIR/; return_val
    cd ./dev/; return_val
    sudo cp -f ./build/hal $BIN_EXPORT_DIR; return_val  
    cd ../api_lib/; return_val
    sudo cp -f ./build/libhalapi.so $LINK_DIR; return_val
    
    cd ..; return_val
    exit 0
fi

############# code beautifier ############
printf "\n### [$0:$LINENO] start reformat.py -r ../hal ###\n"
#reformat.py -r ../hal
printf "### end reformat.py ###\n\n"


if [ "${1}" == "clean" ] ; then
    cd ./api_lib/
    ./build.sh clean
    cd ../dev/
    ./build.sh clean
    exit 0
else
    cd ./dev/
    ./build.sh clean;
    ./build.sh; return_val
    sudo cp -f ./build/hal $BIN_EXPORT_DIR; return_val   
    cd ../api_lib/
    ./build.sh clean;
    ./build.sh; return_val
    sudo cp -f ./build/libhalapi.so $LINK_DIR; return_val
    sudo cp -f ./build/api_main $BIN_EXPORT_DIR; return_val
fi
		
cd ..; return_val

exit 0
