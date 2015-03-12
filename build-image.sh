#!/bin/sh
#this script builds the builroot based image
#BR_OUTPUT_FOLDER=./buildroot
BR_FOLDER=./buildroot
IMAGE_VERSION=XX.YY
BR_BOARD_CONFIG=qemu_x86_64_defconfig
BR_BOARD_LINUX_CONFIG=linux-3.17.config
RAW_DISK_IMAGE=./raw-images/disk.grub1.img.xz
GRUB1_PREPARE_SCRIPT=grub1-prepare.sh
GRUB1_COPY_SCRIPT=grub1-copy.sh
###############################################################################
while getopts o:v:c: f
do
    case $f in
	o) BR_OUTPUT_FOLDER=$OPTARG;;  #buildroot output folder
	v) IMAGE_VERSION=$OPTARG ;;    #image version
    esac
done
#c) DEF_CONFIG_FILE=$OPTARG;;   #output out.disk.img file

#PREPARE_SCRIPT=$(pwd)/scripts/$GRUB1_PREPARE_SCRIPT
#COPY_SCRIPT=$(pwd)/scripts/$GRUB1_COPY_SCRIPT
#echo "$PREPARE_SCRIPT"
#echo "$COPY_SCRIPT"
#exit 0

svn up > /dev/null
BUILDNUMBER=$(LANG=en_US; svn info 2>&1 | grep "Last Changed Rev:" | sed -e 's/.* //')
BUILDNUMBER=$(printf "$IMAGE_VERSION.%05d" $BUILDNUMBER)
[ ! -d  "$BR_OUTPUT_FOLDER" ] && { echo "Error: output folder $BR_OUTPUT_FOLDER not found!"   ; exit -1; }

BR_OUTPUT_FOLDER=$BR_OUTPUT_FOLDER/$IMAGE_VERSION
mkdir -p $BR_OUTPUT_FOLDER
#echo $BUILDNUMBER
#echo $BR_OUTPUT_FOLDER
#echo "cur dir = CURRENT_WRK_DIR"
#exit 0

cp configs/$BR_BOARD_CONFIG         $BR_FOLDER/configs/
cp configs/$BR_BOARD_LINUX_CONFIG   $BR_FOLDER/board/qemu/x86_64/
cp configs/busybox.config           $BR_FOLDER/package/busybox/

pushd .
cd $BR_FOLDER
#make -C $BR_FOLDER -f $BR_FOLDER/Makefile O=$BR_OUTPUT_FOLDER $BR_BOARD_CONFIG
#make -C $BR_FOLDER -f $BR_FOLDER/Makefile O=$BR_OUTPUT_FOLDER
make O=$BR_OUTPUT_FOLDER $BR_BOARD_CONFIG
make O=$BR_OUTPUT_FOLDER
popd

#SCRIPTS_DIR=$(pwd)/scripts
PREPARE_SCRIPT=$(pwd)/scripts/$GRUB1_PREPARE_SCRIPT
COPY_SCRIPT=$(pwd)/scripts/$GRUB1_COPY_SCRIPT
./scripts/grub1-bootdisk.sh -o $BR_OUTPUT_FOLDER -r $RAW_DISK_IMAGE -i $BR_OUTPUT_FOLDER/images/out.disk.img -v $BUILDNUMBER -p $PREPARE_SCRIPT -c $COPY_SCRIPT
#./grub1-bootdisk.sh -o /mnt/buildramdisk/x86 -r ../raw-images/disk.grub1.img.xz -i /mnt/buildramdisk/out.disk.img
#./build-image.sh -o /mnt/buildramdisk -v 01.00

