#this scrpit is used to export tree file
#chuny 2018-10-13
#optimized by guangjung on 2020-08-04

export YANG_MODPATH=./common:./device:./extension:./fmpm:./optical:./protocol:./snmp:./system:./platform:./oc-system
ALL_YANG_FILES=`ls ./*/*.yang ./*/*/*.yang`
#ALL_YANG_FILES=`ls ./protocol/ospf/*.yang `
CURRENT_DIR=`pwd`

# export yang to tree file for each yang file
function exportTree() {
	for file in $ALL_YANG_FILES
	do
		filename=${file##*/}
		dirpath=${file%/*}
		filename_no_suffix=${filename%.*}

		if [ $dirpath == "./test" ]; then
			continue
		fi

		pyang -f tree $file > $dirpath/$filename_no_suffix.tree

		val=`echo $?`
		if test $val -ne 0;then
			echo "######### yang2tree failed in $filename ..."
		fi
	done
}

# export all yang to one tree file
function exportAllToOneTreeFile()
{
	pyang -f tree ./*/*.yang > all.tree
}

# export single yang to one tree file
function exportSingleTreeFile()
{
	yangpath="./optical/com-oplink-optical.yang"
	treepath="./optical/com-oplink-optical_ggj.tree"
	pyang -f tree $yangpath > $treepath
}

# delete all tree files
function clearTreeFiles()
{
	rm -rf ./*/*.tree
	rm -rf ./*/*/*.tree
	rm -rf ./all.tree
}

function helpinfo() {
	echo "useage: ./export_tree.sh [paras]"
	echo "   ./export_tree.sh"
	echo "   ./export_tree.sh single"
	echo "   ./export_tree.sh merge"
	echo "   ./export_tree.sh clean"
	exit 1
}

if [ "$#" -eq "0" ]; then
	echo "The running time is about 50s. Please wait ..."
	exportTree
elif [ "$#" -eq "1" ]; then
	if [ "$1" == "single" ]; then
		# please edit function exportSingleTreeFile
		exportSingleTreeFile
	elif [ "$1" == "merge" ]; then 
		exportAllToOneTreeFile
	elif [ "$1" == "clean" ]; then 
		clearTreeFiles
	else
		helpinfo
	fi
else 
	helpinfo
fi

exit 0
