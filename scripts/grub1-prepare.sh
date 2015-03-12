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
#not needed to copy bzImage if initramdisk is disabled
cp $BR_OUTPUT_FOLDER/images/bzImage $TEMP_MOUNT_POINT/boot/
sed -i "s|timeout.*|timeout 1|" $TEMP_MOUNT_POINT/boot/grub/menu.lst
#create version file under /boot/
echo $VERSION > $TEMP_MOUNT_POINT/boot/version-num.txt
sync
umount $TEMP_MOUNT_POINT
sync
partx -d /dev/loop0
losetup -d /dev/loop0
rm -rf $TEMP_MOUNT_POINT
echo "usb image $OUTPUT_DISK_IMAGE created successfully"
