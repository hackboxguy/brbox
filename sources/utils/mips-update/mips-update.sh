#!/bin/sh

MKIMAGE=/usr/sbin/brbox-mkimage
#file types
SINGL_MK_OK="singl_mk_ok"
SINGL_MK_NOK="singl_mk_nok"
MULTI_MK_OK="multi_mk_ok"
MULTI_MK_NOK="multi_mk_nok"
SINGL_BINARY="singl_binary"
DEV_TYPE_FILE=/tmp/BrBoxDevType.txt
BD_TYP_GLMT300NV2="GL_MT300NV2" #board-type
BRBOX_IMGTYPE_GLMT300NV2="BrBoxGl300N"
BRBOX_IMGTYPE_GLA150="BrBoxGlA150"
BRBOX_IMGTYPE_SIGNATURE="BrBoxSign"
PUBLIC_KEY_FILE=/etc/update_signature.txt
TEST_ONLY="no"
PRINT_HELP="no"
###############################################################################
PrintHelp()
{
	echo "mips-update.sh: firmware update tool"
	echo "usage:$0 -u <mipsProject.uimg> (updates the system with provided file)"
	echo "usage:$0 -u <mipsProject.uimg> -t (tests the image but actual flash will not be updated)"
	return 0
}
ExtractSubImage() #$1=input-proj-file $2=header-info-from-previous-probe $3=output-signature-file $4-filetype
{
	$MKIMAGE -s -l -X $4 -o $3 $1
	[ $? != "0" ] && return 1 #unable to extract signature file
	return 0 #signature file extracted successfully
}
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
		echo "$IMGTYPE.. [OK]"
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
while getopts u:th f
do
    case $f in
	    u) UPDATE_FILE=$OPTARG ;;    #update file path
		t) TEST_ONLY="yes";;
		h) PRINT_HELP="yes";;
    esac
done

[ "$PRINT_HELP" = "yes"  ] && { PrintHelp; return 0; }
[ -z "$UPDATE_FILE"  ] && { PrintHelp; return 0; }

[ ! -f  "$UPDATE_FILE"  ] && { echo "Error: Update file not found!!!"; return 1; }

sysupgrade --test $UPDATE_FILE 1>/dev/null 2>/dev/null
if [ $? = "0" ]; then
  #this is a valid openwrt image, for backward compatibility, dont do any extra checks(just trigger update)
  sysupgrade $UPDATE_FILE
  exit $?
fi

#############check if this is a project file(image+signature)###############
TMP_HEADERFILE=$(mktemp)
TMP_SIGFILEUIMG=$(mktemp)
TMP_SIGFILE=$(mktemp)
TMP_SUBIMAGE=$(mktemp)
echo -n "Probing for project file:................... "
IsItMultiMkFile $UPDATE_FILE $TMP_HEADERFILE
if [ $? = "0" ]; then
  echo -e "[OK]"
  #extract and strip signature sub-image(BrBoxSign)
  ExtractSubImage $UPDATE_FILE $TMP_HEADERFILE $TMP_SIGFILEUIMG $BRBOX_IMGTYPE_SIGNATURE
  [ $? != "0" ] && { echo "Error: unable to extract signature file" ; rm -rf $TMP_HEADERFILE $TMP_SIGFILEUIMG $TMP_SIGFILE ;return 1; }
	echo "Extracting BrBoxSign........................ [OK]"
  dd if=$TMP_SIGFILEUIMG bs=64 skip=1 of=$TMP_SIGFILE 1>/dev/null 2>/dev/null #just strip 64byte mkheader and create .sigfile
  [ $? != "0" ] && { echo "Error: unable to strip mkheader from signature" ; rm -rf $TMP_HEADERFILE $TMP_SIGFILEUIMG $TMP_SIGFILE ;return 1; }

 #extract and strig sub-image(BrBoxGl300N)
  ExtractSubImage $UPDATE_FILE $TMP_HEADERFILE $TMP_SUBIMAGE $BRBOX_IMGTYPE_GLMT300NV2
  if [ $? = "0" ]; then
    echo "Extracting BrBoxGl300N...................... [OK]"
    TMP_SUBIMAGE_RAW=$(mktemp)
    dd if=$TMP_SUBIMAGE bs=64 skip=1 of=$TMP_SUBIMAGE_RAW 1>/dev/null 2>/dev/null #just strip 64byte mkheader and create subimg
    openssl dgst -verify $PUBLIC_KEY_FILE -keyform PEM -sha256 -signature $TMP_SIGFILE -binary $TMP_SUBIMAGE_RAW > /dev/null
    if [ $? = "0" ]; then
      echo "Signature verification of BrBoxGl300N....... [OK]"
      rm -rf $TMP_HEADERFILE $TMP_SIGFILEUIMG $TMP_SIGFILE $TMP_SUBIMAGE_RAW
      UPDATE_FILE=$TMP_SUBIMAGE #replace update image with extracted file and continue normal update process
    else
      echo "Signature verification of BrBoxGl300N....... [NOK]"
      rm -rf $TMP_HEADERFILE $TMP_SIGFILEUIMG $TMP_SIGFILE $TMP_SUBIMAGE $TMP_SUBIMAGE_RAW
      return 1
    fi
  else
    echo "Extracting BrBoxGl300N Failed!!............. [NOK]"
    rm -rf $TMP_HEADERFILE $TMP_SIGFILEUIMG $TMP_SIGFILE $TMP_SUBIMAGE
    return 1
  fi
else
  echo -e "[NO]" #not a project file
  #return from here to force always signed image update
fi
#############################################################################
TMP_IMAGEFILE=$(mktemp) #creates an emptyfile which will be filled by ProcessUpdate as rootfs.tar.xz
ProcessUpdate $UPDATE_FILE $TMP_IMAGEFILE
[ $? != "0" ] && { echo "Error: Invalid filetype" ; rm -rf $TMP_IMAGEFILE ; return 1; } #TODO: check if this file is encrypted
sysupgrade --test $TMP_IMAGEFILE
[ $? != "0" ] && { echo "Error: Invalid openwrt-image" ; return 1; }
echo "Openwrt image health........................ [OK]"

echo -n "Updating flash.............................. "
#check if project file check in the beginning has already created a tmp file
if [ $UPDATE_FILE = $TMP_SUBIMAGE ]; then
  rm -rf $TMP_SUBIMAGE
fi
if [ "$TEST_ONLY" = "no"  ]; then
	sysupgrade $TMP_IMAGEFILE #system will automatically reboot
else
	sysupgrade --test $TMP_IMAGEFILE
fi
#sysupgrade -i $TMP_IMAGEFILE #system will automatically reboot
rm -rf $TMP_IMAGEFILE

if [ $? = "0" ]; then
	echo -e "[OK]"
	return 0
else
	echo -e "[NOK]"
	return 1
fi
