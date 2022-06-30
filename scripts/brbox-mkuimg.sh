#!/bin/sh
#brbox upgrade-package creator based on mkimage tool
MKIMAGE=./brbox-mkimage
BRBOX_PROJ="BrBoxProj"    #complete project including all sub-modules
BRBOX_ROOT="BrBoxRtBtr"   #rootfs-baytrail
BRBOX_STTNG="BrBoxSttng"  #settings partition
BRBOX_USRDAT="BrBoxUsrDt" #user-data partition
BRBOX_SIGNATURE="BrBoxSign" #signature file
BRBOX_GL300NV2="BrBoxGl300N"
BRBOX_GLA150="BrBoxGlA150"
BRBOX_HLKRM04="BrBoxHlRm04"

USAGE="script for generating BrBox binary update packages"
USAGE1="usage:./$0 -r <rootfs.tar.xz> -v <version.xx.yy> -m <mkimage_tool_path> -t <rootfs_type>"
ROOTFS_FILE="rootfs.tar.xz"
VERSION="00.01"
OUT_FILE=./uOutFile.uimg
PACKAGE_HEADER_STRING=BrBox
###############################################################################
#./brbox-mkuimg.sh -r rootfs.tar.xz -v 00.02 -o out.uimg -m ../disk_skeleton/usr/sbin/brbox-mkimage
#./brbox-mkuimg.sh -r gl-mt300nv2-remote-kit.bin -t BrBoxGl300N -v 00.02 -o out.uimg -m ../disk_skeleton/usr/sbin/brbox-mkimage
#./brbox-mkuimg.sh -r gl-mt300nv2-remote-kit.sign -t BrBoxSign -v 00.02 -o sign.uimg -m ../disk_skeleton/usr/sbin/brbox-mkimage
#./brbox-mkuimg.sh -r gl-mt300nv2.uimg:sign.uimg -t BrBoxProj -v 00.02 -o uProj.uimg -m ../disk_skeleton/usr/sbin/brbox-mkimage
###############################################################################
Create_Project() #$1=input:files $2version-string $3=output-file
{
	printf "Creating Project package ................................ "
	VERSION_STR="$PACKAGE_HEADER_STRING $BRBOX_PROJ V-$2"
	$MKIMAGE -A x86  \
		-O linux \
		-T multi \
		-C none  \
		-a 0     \
		-e 0     \
		-n "$VERSION_STR" \
		-d "$1" "$3"
}
Create_SignatureImage() #$1=inputfile $2=version-string $3=output-file
{
	printf "Creating Signature package  ............................. "
	VERSION_STR="$PACKAGE_HEADER_STRING $BRBOX_SIGNATURE V-$2"
	$MKIMAGE -T $BRBOX_SIGNATURE \
		-C none \
		-a 0x00000000 \
		-e 0x00000000 \
		-n "$VERSION_STR" \
                -A x86 \
		-d "$1" "$3" 1>/dev/null 2>/dev/null
        test 0 -eq $? && echo "[OK]" || { echo "[FAIL]";return 1;}
	return 0
}
CreateBrBoxRoot() #$1=in-rootfs.tar.xz-file $2=VersionString $3=out-file-path
{
	printf "Creating Roofs Upgrade package .......................... "
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
CreateMipsImage() #$1=mips-image.bin $2=VersionString $3=out-file-path $4=type
{
	printf "Creating MIPS Upgrade package  .......................... "
	VERSION_STR="$PACKAGE_HEADER_STRING $4 V-$2"
	$MKIMAGE -T $4 \
		-C none \
		-a 0x00000000 \
		-e 0x00000000 \
		-n "$VERSION_STR" \
                -A mips \
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
while getopts r:v:o:m:t: f
do
    case $f in
	m) MKIMAGE=$OPTARG ;;     #mkimage binary path
	r) ROOTFS_FILE=$OPTARG ;;
	v) VERSION=$OPTARG ;;
	o) OUT_FILE=$OPTARG ;;
	t) BRBOX_ROOT=$OPTARG;;
    esac
done
if [ $BRBOX_ROOT = $BRBOX_GL300NV2 ]; then
	CreateMipsImage $ROOTFS_FILE $VERSION $OUT_FILE $BRBOX_ROOT
elif [ $BRBOX_ROOT = $BRBOX_GLA150 ]; then
	CreateMipsImage $ROOTFS_FILE $VERSION $OUT_FILE $BRBOX_ROOT
elif [ $BRBOX_ROOT = $BRBOX_HLKRM04 ]; then
	CreateMipsImage $ROOTFS_FILE $VERSION $OUT_FILE $BRBOX_ROOT
elif [ $BRBOX_ROOT = $BRBOX_SIGNATURE ]; then
	Create_SignatureImage $ROOTFS_FILE $VERSION $OUT_FILE
elif [ $BRBOX_ROOT = $BRBOX_PROJ ]; then
	Create_Project $ROOTFS_FILE $VERSION $OUT_FILE
else
	CreateBrBoxRoot $ROOTFS_FILE $VERSION $OUT_FILE
fi
