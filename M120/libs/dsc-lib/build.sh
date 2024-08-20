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

if [ "$1" == "copy" ]; then
    cd build; return_val
    sudo cp -f liboplkdsc.so $LINK_DIR; return_val
    cd ..; return_val
    
    exit 0
fi

################## complie ################
sudo rm build -fr;return_val
mkdir -p  build; return_val
cd build; return_val
cmake ..; return_val
echo "make"
make -j4; return_val
################## complie ################

echo "#########  current Dir:`pwd` ..."
sudo cp -f liboplkdsc.so $LINK_DIR ; return_val
cd ..; return_val

exit 0
