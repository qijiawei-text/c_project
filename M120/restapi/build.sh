#!/bin/bash

### $1=copy for pack
#SUBDIRS='openapi'

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
    ### copy restapi config files
    mkdir -p ../../filesystem/etc/rest/
    cp -rf openapi/mapping/module_path.json ../../filesystem/etc/rest/; return_val
    cp -rf openapi/mapping/list_key.json ../../filesystem/etc/rest/; return_val
fi

exit 0


