#!/bin/bash
#this script builds the builroot based image
BR_FOLDER=./buildroot
IMAGE_VERSION=XX.YY

BR_BOARD_SYSTEM_CONFIG=baytrail
BR_BOARD_CONFIG=qemu_x86_64_defconfig
BR_BOARD_LINUX_CONFIG=linux.config 

#RAW_DISK_IMAGE=./raw-images/disk.grub1.img.xz
GRUB1_PREPARE_SCRIPT=grub1-prepare.sh
GRUB1_COPY_SCRIPT=grub1-copy.sh
GRUB1_SUDO_EXE_SCRIPT=sudo-executor-grub1-prepare.sh

#output images
BOOTABLE_USB_IMG=bootable-usb-disk.img
BRBOX_ROOT_IMG=uBrBoxRoot.uimg

IMAGE_ONLY=0
PREPARE_ONLY=0

#root-fs types for differenet h/w (taken from brbox-mkimage/image.h)
ROOTFS_TYPE_BTR="BrBoxRtBtr"
ROOTFS_TYPE_RP1="BrBoxRtRp1"
ROOTFS_TYPE_RP2="BrBoxRtRp2"
ROOTFS_TYPE_BBB="BrBoxRtBbb"
ROOTFS_TYPE_WDB="BrBoxRtWdb"

BOARD_TYPE_BTR="baytrail"                 #base-build
BOARD_TYPE_BTR_MEDIA="baytrail-media"                 #base-build
BOARD_TYPE_RP1="raspi1"                   #base-build
BOARD_TYPE_RP1_SMSW="raspi1-smartsw"      #application-build
BOARD_TYPE_RP1_DSPT="raspi1-disptst"      #application-build
BOARD_TYPE_RP2="raspi2"                   #base-build
BOARD_TYPE_BBB="bbb"                      #base-build
BOARD_TYPE_WDB="wandboard"                #base-build
###############################################################################
while getopts b:o:v:c:s:ip f
do
    case $f in
	b) BR_BOARD_SYSTEM_CONFIG=$OPTARG;;  #board/system type for different application
	o) BR_OUTPUT_FOLDER=$OPTARG;;        #buildroot output folder
	v) IMAGE_VERSION=$OPTARG ;;          #image version
	s) SUDOPW=$OPTARG ;;                 #sud password
	i) IMAGE_ONLY=1 ;;                   #-i can be used only for binary image creation(build will be skipped)
        p) PREPARE_ONLY=1 ;;                 #just prepare buildroot, dont build
    esac
done

#	t) BOARD_TYPE=$OPTARG ;;             #board-type= baytrail/raspi2
#TODO: switch case for br-board-system-config
if [ $BR_BOARD_SYSTEM_CONFIG = "$BOARD_TYPE_RP1" ]; then
	BR_BOARD_CONFIG=raspberrypi_defconfig
	BR_BOARD_LINUX_CONFIG_PATH=board/raspberrypi/
	BOOT_IMG_SCRIPT=$(pwd)/scripts/raspi-bootdisk.sh
elif [ $BR_BOARD_SYSTEM_CONFIG = "$BOARD_TYPE_RP1_SMSW" ]; then
	BR_BOARD_CONFIG=raspberrypi_defconfig
	BR_BOARD_LINUX_CONFIG_PATH=board/raspberrypi/
	BOOT_IMG_SCRIPT=$(pwd)/scripts/raspi-bootdisk.sh
elif [ $BR_BOARD_SYSTEM_CONFIG = "$BOARD_TYPE_RP1_DSPT" ]; then
	BR_BOARD_CONFIG=raspberrypi_defconfig
	BR_BOARD_LINUX_CONFIG_PATH=board/raspberrypi/
	BOOT_IMG_SCRIPT=$(pwd)/scripts/raspi-bootdisk.sh
elif [ $BR_BOARD_SYSTEM_CONFIG = "$BOARD_TYPE_RP2" ]; then
	BR_BOARD_CONFIG=raspberrypi2_defconfig
	BR_BOARD_LINUX_CONFIG_PATH=board/raspberrypi2/
	BOOT_IMG_SCRIPT=$(pwd)/scripts/raspi-bootdisk.sh
elif [ $BR_BOARD_SYSTEM_CONFIG = "$BOARD_TYPE_BTR_MEDIA" ]; then
	BR_BOARD_CONFIG=qemu_x86_64_defconfig
	BR_BOARD_LINUX_CONFIG_PATH=board/qemu/x86_64/
	BOOT_IMG_SCRIPT=$(pwd)/scripts/grub2-bootdisk.sh
else   #default is baytrail(x86_64)
	BR_BOARD_CONFIG=qemu_x86_64_defconfig
	BR_BOARD_LINUX_CONFIG_PATH=board/qemu/x86_64/
	BOOT_IMG_SCRIPT=$(pwd)/scripts/grub2-bootdisk.sh
fi

svn up > /dev/null
BUILDNUMBER=$(LANG=en_US; svn info 2>&1 | grep "Last Changed Rev:" | sed -e 's/.* //')
TMP_BUILDNUM=$(printf "%05d" $BUILDNUMBER)
BUILDNUMBER=$(printf "$IMAGE_VERSION.%05d" $BUILDNUMBER)
[ ! -d  "$BR_OUTPUT_FOLDER" ] && { echo "Error: output folder $BR_OUTPUT_FOLDER not found!"   ; exit -1; }

BR_OUTPUT_FOLDER=$BR_OUTPUT_FOLDER/$IMAGE_VERSION
if [ $IMAGE_ONLY = 0 ]; then
	mkdir -p $BR_OUTPUT_FOLDER
	echo $BR_BOARD_SYSTEM_CONFIG > $BR_OUTPUT_FOLDER/BoardConfig.txt #remember which build-config was triggered
	cp configs/$BR_BOARD_SYSTEM_CONFIG/$BR_BOARD_CONFIG         $BR_FOLDER/configs/
	cp configs/$BR_BOARD_SYSTEM_CONFIG/$BR_BOARD_LINUX_CONFIG   $BR_FOLDER/$BR_BOARD_LINUX_CONFIG_PATH
	cp configs/$BR_BOARD_SYSTEM_CONFIG/busybox.config           $BR_FOLDER/package/busybox/
	#TODO: configs/$BR_BOARD_SYSTEM_CONFIG/uclibc.config        $BR_FOLDER/package/uclibc/
	pushd .
	cd $BR_FOLDER
	make O=$BR_OUTPUT_FOLDER $BR_BOARD_CONFIG BRBOX_RELVERSION=$IMAGE_VERSION BRBOX_BUILDNUM=$TMP_BUILDNUM
	if [ $PREPARE_ONLY = 1 ]; then
		popd
		exit 0
	fi	
	make O=$BR_OUTPUT_FOLDER BRBOX_RELVERSION=$IMAGE_VERSION BRBOX_BUILDNUM=$TMP_BUILDNUM
	BUILD_RESULT=$?	
	popd
else
	BUILD_RESULT=0  #for image-only, just set build-result as success
fi
[ "$BUILD_RESULT" != "0" ] && echo "Error!!! build failed!!!!" && exit 1 

if [ $BR_BOARD_SYSTEM_CONFIG = "$BOARD_TYPE_RP1" ]; then
	./scripts/sudo-grub2-bootdisk.sh $BOOT_IMG_SCRIPT $BR_OUTPUT_FOLDER $BUILDNUMBER $BR_OUTPUT_FOLDER/images/$BOOTABLE_USB_IMG $SUDOPW
	ROOTFS_TYPE=$ROOTFS_TYPE_RP1
elif [ $BR_BOARD_SYSTEM_CONFIG = "$BOARD_TYPE_RP1_SMSW" ]; then
	./scripts/sudo-grub2-bootdisk.sh $BOOT_IMG_SCRIPT $BR_OUTPUT_FOLDER $BUILDNUMBER $BR_OUTPUT_FOLDER/images/$BOOTABLE_USB_IMG $SUDOPW
	ROOTFS_TYPE=$ROOTFS_TYPE_RP1
elif [ $BR_BOARD_SYSTEM_CONFIG = "$BOARD_TYPE_RP1_DSPT" ]; then
	./scripts/sudo-grub2-bootdisk.sh $BOOT_IMG_SCRIPT $BR_OUTPUT_FOLDER $BUILDNUMBER $BR_OUTPUT_FOLDER/images/$BOOTABLE_USB_IMG $SUDOPW
	ROOTFS_TYPE=$ROOTFS_TYPE_RP1
elif [ $BR_BOARD_SYSTEM_CONFIG = "$BOARD_TYPE_RP2" ]; then
	./scripts/sudo-grub2-bootdisk.sh $BOOT_IMG_SCRIPT $BR_OUTPUT_FOLDER $BUILDNUMBER $BR_OUTPUT_FOLDER/images/$BOOTABLE_USB_IMG $SUDOPW
	ROOTFS_TYPE=$ROOTFS_TYPE_RP2
elif [ $BR_BOARD_SYSTEM_CONFIG = "$BOARD_TYPE_BTR_MEDIA" ]; then
	./scripts/sudo-grub2-bootdisk.sh $BOOT_IMG_SCRIPT $BR_OUTPUT_FOLDER $BUILDNUMBER $BR_OUTPUT_FOLDER/images/$BOOTABLE_USB_IMG $SUDOPW
	ROOTFS_TYPE=$ROOTFS_TYPE_BTR
else 
	#baytrail
	./scripts/sudo-grub2-bootdisk.sh $BOOT_IMG_SCRIPT $BR_OUTPUT_FOLDER $BUILDNUMBER $BR_OUTPUT_FOLDER/images/$BOOTABLE_USB_IMG $SUDOPW
	#create Rootfs upgrade package
	ROOTFS_TYPE=$ROOTFS_TYPE_BTR
fi

./scripts/brbox-mkuimg.sh -r $BR_OUTPUT_FOLDER/images/rootfs.tar.xz -v $BUILDNUMBER -o $BR_OUTPUT_FOLDER/images/$BRBOX_ROOT_IMG -m $BR_OUTPUT_FOLDER/host/usr/sbin/brbox-mkimage -t $ROOTFS_TYPE

