#!/bin/bash

### $1=copy for pack

SUBDIRS='util-lib dsc-lib hal mgmt-lib'
CUR_USER=`who am i | awk '{print $1}'`

####  check execute result
function return_val()
{
    val=`echo $?`
    if test $val -ne 0;then
        echo "#########  `pwd` is ERROR..."
        exit -1
    fi
}

for i in $SUBDIRS; do 
    echo "+++++++++ enter $i"
    cd $i; return_val
    
    if [ "$1" == "copy" ]; then
        ./build.sh copy; return_val
        cd ..; return_val
        
        continue
    fi
    
    ############# file attribute ############
    find ./ -name "*.[ch]" | grep -v build | xargs dos2unix
    find ./ -name "*.sh" | grep -v build | xargs dos2unix
    #find ./ -name "*.[ch]" | grep -v build | xargs chmod 644
    find ./ -name "*.sh" | grep -v build | xargs chmod 755

    ############# code beautifier ############
    printf "\n### line[$LINENO] start reformat.py *.c *.h ###\n"
    find ./ -name "*.[ch]" | grep -v build | xargs reformat.py
    printf "### end reformat.py 'pwd' *.c *.h ###\n\n"
    find ./ -name "*.[ch]" | grep -v build | xargs chown $CUR_USER:$CUR_USER
    find ./ -name "*.sh" | grep -v build | xargs chown $CUR_USER:$CUR_USER

    ############# scan code, make strong ############
    printf "### [$0:$LINENO] start tscan code ###\n"

    ############# build ############
    printf "### [$0:$LINENO] start build ###\n"
    ./build.sh;return_val

    cd ..; return_val
    echo "--------- exit $i"
done


exit 0
