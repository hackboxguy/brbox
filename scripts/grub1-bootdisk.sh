#!/bin/sh
#this script creates usb-bootable-disk with grub1 as a bootloader
IMAGE_VERSION=00.01.12345
###############################################################################
#./grub1-bootdisk.sh -o /mnt/buildramdisk/x86 -r ../raw-images/disk.grub1.img.xz -i /mnt/buildramdisk/out.disk.img -p <prepare_script> -c <copy_script>
while getopts o:r:v:i:p:c: f
do
    case $f in
	o) BR_OUTPUT_FOLDER=$OPTARG;;  #buildroot output folder
	r) RAW_DISK_IMAGE=$OPTARG;;     #raw disk.img file
	v) IMAGE_VERSION=$OPTARG ;;
	i) OUTPUT_DISK_IMAGE=$OPTARG;;  #output out.disk.img file
	p) PREPARE_SCRIPT=$OPTARG;;     #prepare script
	c) COPY_SCRIPT=$OPTARG;;        #copy script
    esac
done
[ ! -f  "$RAW_DISK_IMAGE"    ] && { echo "Error: disk image $RAW_DISK_IMAGE not found!" ; exit -1; }
[ ! -d  "$BR_OUTPUT_FOLDER/"  ] && { echo "Error: br-output folder $BR_OUTPUT_FOLDER not found!"   ; exit -1; }
[ ! -f  "$BR_OUTPUT_FOLDER/host/sbin/grub" ] && { echo "Error: grub binary file not found in $BR_OUTPUT_FOLDER!" ; exit -1; }

unxz -c $RAW_DISK_IMAGE > $OUTPUT_DISK_IMAGE
sudo $PREPARE_SCRIPT -o $BR_OUTPUT_FOLDER -i $OUTPUT_DISK_IMAGE -v $IMAGE_VERSION
$COPY_SCRIPT $BR_OUTPUT_FOLDER/host/sbin/grub $OUTPUT_DISK_IMAGE

