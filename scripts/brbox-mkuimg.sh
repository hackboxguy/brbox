#!/bin/sh
#brbox upgrade-package creator based on mkimage tool
MKIMAGE=./brbox-mkimage
BRBOX_PROJ="BrBoxProj"    #complete project including all sub-modules
BRBOX_ROOT="BrBoxRoot"    #rootfs
BRBOX_STTNG="BrBoxSttng"  #settings partition
BRBOX_USRDAT="BrBoxUsrDt" #user-data partition 
USAGE="script for generating BrBox binary update packages"
USAGE1="usage:./$0 -r <rootfs.tar.xz> -v <version.xx.yy> -m <mkimage_tool_path>"
ROOTFS_FILE="rootfs.tar.xz"
VERSION="00.01"
OUT_FILE=./uOutFile.uimg
PACKAGE_HEADER_STRING=BrBox
###############################################################################
#./brbox-mkuimg.sh -r rootfs.tar.xz -v 00.02 -o out.uimg -m ../disk_skeleton/usr/sbin/brbox-mkimage
###############################################################################
CreateBrBoxRoot() #$1=in-rootfs.tar.xz-file $2=VersionString $3=out-file-path
{
	printf "Creating Roofs Upgrade package ....................... "
	#todo: check if $1 is compressed tar.xz file
	VERSION_STR="$PACKAGE_HEADER_STRING $BRBOX_ROOT V-$2"
	$MKIMAGE -T $BRBOX_ROOT \
		-C lzma2 \
		-a 0x00000000 \
		-e 0x00000000 \
		-n "$VERSION_STR" \
                -A x86 \
		-d $1 $3 1>/dev/null 2>/dev/null
    	test 0 -eq $? && echo "[OK]" || { echo "[FAIL]";return 1;}
	return 0
}
###############################################################################
if [ $# -lt 1  ]; then 
	echo $USAGE
	echo $USAGE1
	exit $ERR_ARGC
fi
##############parse the arguments##############################################
while getopts r:v:o:m: f
do
    case $f in
	m) MKIMAGE=$OPTARG ;;     #mkimage binary path
	r) ROOTFS_FILE=$OPTARG ;;
	v) VERSION=$OPTARG ;;
	o) OUT_FILE=$OPTARG ;;
    esac
done

CreateBrBoxRoot $ROOTFS_FILE $VERSION $OUT_FILE
