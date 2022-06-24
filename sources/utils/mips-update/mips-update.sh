#!/bin/sh

MKIMAGE=/usr/sbin/brbox-mkimage
#file types
SINGL_MK_OK="singl_mk_ok"
SINGL_MK_NOK="singl_mk_nok"
MULTI_MK_OK="multi_mk_ok"
MULTI_MK_NOK="multi_mk_nok"
SINGL_BINARY="singl_binary"
DEV_TYPE_FILE=/tmp/BrBoxDevType.txt
BD_TYP_GLMT300NV2="GL_MT300NV2"
BRBOX_IMGTYPE_GLMT300NV2="BrBoxGl300Nv2"
###############################################################################
IsItMultiMkFile() #$1=file $2=text-file-to-store-mkimage-header-text
{
	$MKIMAGE -h 1 $1 2>$2
	[ $? != "0" ] && return 1 #not a Mk file
	cat $2 | grep "Multi-File" > /dev/null
	[ $? != "0" ] && return 1 #Mk-file, but not a Multi-Mk-File
	return 0 #this is a Multi-Mk-file
}
###############################################################################
IsItSingleMkFile() #$1=file $2=text-file-to-store-mkimage-header-text
{
	$MKIMAGE -h 1 $1 2>$2
	[ $? != "0" ] && return 1 #not a Mk-file
	cat $2 | grep "Multi-File" > /dev/null
	[ $? = "0" ] && return 1 #this is a Multi-MK-File
	return 0 #this is a Single-MK-file
}
###############################################################################
CheckFileType() #$1=file $2=text-file-to-store-mkimage-header-text
{
	GLOBAL_VAR_TOTAL_FILES=0
	IsItMultiMkFile $1 $2
	if [ $? != "0" ]; then
		IsItSingleMkFile $1 $2
		if [ $? = "0" ]; then
			#echo -e "Single-MK-File"
			$MKIMAGE -l $1 2>/dev/null
			[ $? != "0" ] && { echo $SINGL_MK_NOK ; return 1; }
			echo $SINGL_MK_OK
		else
			echo $SINGL_BINARY
		fi
	else
		#echo -e "Multi-MK-File"
		TOTAL_SUB_IMAGES=$($MKIMAGE -l -t $1 2>$2)
		[ $? != "0" ] && { echo $MULTI_MK_NOK ; return 1; }
		#count total sub-images
		TOTAL_SUB_IMAGES=$(echo $TOTAL_SUB_IMAGES | grep "are:")
		[ $? != "0" ] && { echo $MULTI_MK_NOK ; return 1; }
		TOTAL_SUB_IMAGES=$(echo "$TOTAL_SUB_IMAGES" | sed s/".*are: "//)
		echo $MULTI_MK_OK
		echo "total-sub-images=$TOTAL_SUB_IMAGES">>$2 #caller needs this value(pass it through mk-header-text file)
	fi
	return 0
}
###############################################################################
GetImageType() #$-input-file
{
	IMGNAME=$($MKIMAGE -l $1 2>&1 | grep "Image Name:")
	[ $? != "0" ] && return 1
	IMGNAME=$(echo $IMGNAME | awk '{print $4}')
	[ $? != "0" ] && return 1
	echo $IMGNAME
	return 0
}
###############################################################################
ProcessImageType() #$1-inputfile $2=IMGTYPE $3=outputfile
{
	BOARD_TYPE=$(cat $DEV_TYPE_FILE)
	BRBOX_ROOT="unknown"

	[ "$BOARD_TYPE" = "$BD_TYP_GLMT300NV2"     ]  && BRBOX_IMG=$BRBOX_IMGTYPE_GLMT300NV2

	if [ "$2" = "$BRBOX_IMG" ]; then
		dd if=$1 bs=64 skip=1 of=$3 1>/dev/null 2>/dev/null #just strip 64byte mkheader and create rootfs.tar.xz
		return $?
	else
		return 1
	fi
}
###############################################################################
ProcessUpdate() #$1=input-update-file $2-output-file(if-applicable)
{
	TMPFILE=$(mktemp)
	echo -n "Checking file health:......... "
	FILE_TYPE=$(CheckFileType $1 $TMPFILE)
	[ $? != "0" ] && { echo -e "Error!!! invalid image($FILE_TYPE)" ; rm -rf $TMPFILE ; return 1; }
	if [ "$FILE_TYPE" = "$SINGL_MK_OK" ]; then
		rm -rf $TMPFILE #for single mk, we dont need this file
		echo -e "$FILE_TYPE.. [OK]"
		printf "Checking file type............ "
		IMGTYPE=$(GetImageType $1)
		[ $? != "0" ] && { echo "[FAIL]" ; return 1; }
		#echo "$IMGTYPE.... [OK]"
		ProcessImageType $1 $IMGTYPE $2
		[ $? != "0" ] && { echo "[FAIL]" ; return 1; }
		echo "$IMGTYPE [OK]"
		return 0
	elif [ "$FILE_TYPE" = "$MULTI_MK_OK" ]; then
		TOTAL_SUB_IMAGES=$(grep "total-sub-images=" $TMPFILE)
		TOTAL_SUB_IMAGES=$(echo $TOTAL_SUB_IMAGES | sed 's/total-sub-images=//' )
		echo -e "$FILE_TYPE total-sub-images=$TOTAL_SUB_IMAGES"
		#TODO: ProcessMultiMKUpdate $1 $TOTAL_SUB_IMAGES $TMPFILE
    rm -rf $TMPFILE #for the moment, delete this file: TODO
		return 1 #$?
	elif [ "$FILE_TYPE" = "$SINGL_BINARY" ]; then
		echo -e "$FILE_TYPE"
		rm -rf $TMPFILE
		return 1
	else
		echo -e "Unknown image type"
		rm -rf $TMPFILE
		return 1
	fi
}
###############################################################################
while getopts u: f
do
    case $f in
	    u) UPDATE_FILE=$OPTARG ;;    #update file path
    esac
done

[ ! -f  "$UPDATE_FILE"  ] && { echo "Error: Update file not found!!!"; return 1; }
TMP_IMAGEFILE=$(mktemp) #creates an emptyfile which will be filled by ProcessUpdate as rootfs.tar.xz
ProcessUpdate $UPDATE_FILE $TMP_IMAGEFILE
[ $? != "0" ] && { echo "Error: Invalid filetype" ; return 1; }
sysupgrade --test $TMP_IMAGEFILE
[ $? != "0" ] && { echo "Error: Invalid openwrt-image" ; return 1; }
echo "Openwrt image health........................ [OK]"