#!/bin/bash

function return_val()
{
    val=`echo $?`
    if test $val -ne 0; then
        echo ""
        echo "##################################################"
        echo -e "\033[41;37m##              Build Project Error             ##\033[0m"
        echo "##################################################"
        echo ""
        exit -1
    fi
}

function build_process()
{

    if [ "${1}" == "debug" ] ; then
        echo -e "\033[43;37m  apilib build debug version \033[0m"
    else
        echo -e "\033[43;37m  apilib build release version \033[0m"
    fi

    if [ ! -f $BUILD_PATH/Makefile ] ; then
        rm $BUILD_PATH -rf;
        mkdir  $BUILD_PATH;
        cd     $BUILD_PATH;
        $CMAKE_PROCESS; return_val
        make -j4; return_val
    else
        cd     $BUILD_PATH;
        make -j4; return_val
    fi

    cd     ../
}


if [ "${1}" == "clean" ] ; then
    rm -rf build
    exit 0
else
    BUILD_PATH="./build"
    CMAKE_PROCESS="cmake  ../"
fi

build_process ${1}

exit 0
