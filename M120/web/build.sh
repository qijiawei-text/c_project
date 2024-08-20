#!/bin/bash

### $1=copy for pack
SUBDIRS='conf demo'

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
    ### copy file
    ### copy webfiles
    sudo cp -rf demo/* ../../filesystem/usr/local/apache2/htdocs; return_val
    ### copy conf
    sudo cp -f conf/server* ../../filesystem/usr/local/apache2/conf/; return_val
    sudo cp -f conf/httpd.conf ../../filesystem/usr/local/apache2/conf/; return_val
    sudo cp -f conf/httpd-ssl.conf ../../filesystem/usr/local/apache2/conf/extra/; return_val
    exit 0
fi

exit 0

for i in $SUBDIRS; do 
    echo "+++++++++ enter $i"
    cd $i;return_val
    ./build.sh ;return_val
    cd ..;return_val
    echo "--------- exit $i"
done

exit 0

