#!/bin/sh
#run this script as sudo to create a grub1-bootable disk
TEMP_MNT_FOLDER=temp_mnt
###############################################################################
while getopts o:v:i: f
do
    case $f in
	o) BR_OUTPUT_FOLDER=$OPTARG ;;  #buildroot output folder
	v) VERSION=$OPTARG ;;
	i) OUTPUT_DISK_IMAGE=$OPTARG;;  #output out.disk.img file
    esac
done
[ ! -f  "$OUTPUT_DISK_IMAGE" ]     && { echo "Error: disk image $OUTPUT_DISK_IMAGE not found!" ; exit -1; }
[ ! -d  "$BR_OUTPUT_FOLDER" ] && { echo "Error: output folder $BR_OUTPUT_FOLDER not found!"   ; exit -1; }

TEMP_MOUNT_POINT=$(pwd)
TEMP_MOUNT_POINT=$TEMP_MOUNT_POINT/$TEMP_MNT_FOLDER
losetup -f $OUTPUT_DISK_IMAGE
partx -a /dev/loop0
mkfs.ext2 -L root /dev/loop0p1
mkdir -p $TEMP_MOUNT_POINT
mount /dev/loop0p1 $TEMP_MOUNT_POINT
tar -C $TEMP_MOUNT_POINT -xf $BR_OUTPUT_FOLDER/images/rootfs.tar
cp $BR_OUTPUT_FOLDER/images/bzImage $TEMP_MOUNT_POINT/boot/
#todo: reduce grub 10sec countdown
#$TEMP_MOUNT_POINT/boot/grub/menu.lst timeout		10
sed -i "s|timeout.*|timeout 1|" $TEMP_MOUNT_POINT/boot/grub/menu.lst
#todo: create version file under /boot/
sync
umount $TEMP_MOUNT_POINT
sync
partx -d /dev/loop0
losetup -d /dev/loop0
echo "usb image $OUTPUT_DISK_IMAGE created successfully"
