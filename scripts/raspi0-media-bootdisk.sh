#!/bin/sh
###########################################################
IMAGESIZE=460M #356M
ROOT1_LABEL=ROOT1   #linux-1
ROOT2_LABEL=ROOT2   #linux-2
STTNG_LABEL=STTNG   #settings
USRDAT_LABEL=USRDAT #userdata

BR_OUTPUT_FOLDER=/mnt/buildramdisk/x86
SUDO=
IMAGE_VERSION=01.00.12345
OUTPUT_DISK=0
GRUB_MENU_ENTRY1='brbox1'
GRUB_MENU_ENTRY2='brbox2'
GRUB2_TIMEOUT=1
ROOTDELAY=5
###########################################################
while getopts o:v:i: f
do
    case $f in
	o) BR_OUTPUT_FOLDER=$OPTARG;;  #buildroot output folder
	v) IMAGE_VERSION=$OPTARG ;;    #image version
	i) OUTPUT_DISK=$OPTARG ;;      #path of the complete usb bootable disk
    esac
done
#v) IMAGE_VERSION=$OPTARG ;;    #image version
[ "$OUTPUT_DISK" = 0 ] && OUTPUT_DISK=$BR_OUTPUT_FOLDER/images/raspi0-media.img

BINARIES_DIR=$BR_OUTPUT_FOLDER/images
RPI_FMW_DIR=$BINARIES_DIR/rpi-firmware
RPI_KERNEL=$BINARIES_DIR/zImage
RPI_DTB=$BINARIES_DIR/*.dtb
IMAGENAME=$(mktemp)
MKIMG_TIMESTAMP=$(date +%Y%m%d%H%M%S)
ROOTFS="$BINARIES_DIR/rootfs.tar.xz"
BOOTMOUNTPOINT=$(mktemp -d)
ROOTMOUNTPOINT=$(mktemp -d)
ROOT2MOUNTPOINT=$(mktemp -d)
STTNGMOUNTPOINT=$(mktemp -d)
BOOT_MARKER_RPI=$(pwd)/sources/scripts/raspi2/boot/cmdline.txt #kernel cmdline args file 
BOOT_CONFIG_RPI=$(pwd)/sources/scripts/raspi2/boot/config.txt
SAMPLE_MEDIA=$(pwd)/sources/sample-media/sample-video.mkv
CUSTOM_STARTUP=$(pwd)/sources/scripts/custom-startups/rc.local
printf "creating image file ..................................... "
    fallocate -l "$IMAGESIZE" "$IMAGENAME" >/dev/null
    test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

printf "Find free loopdevice %s ... " "$LOOPDEVICE"
    LOOPDEVICE=$($SUDO losetup -f)
    test 0 -eq $? && printf "found %s .............. [OK]\n" "$LOOPDEVICE" || echo "[FAIL]"

printf "Creating loopdevice ..................................... "
    $SUDO losetup "$LOOPDEVICE" "$IMAGENAME"
    test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

####################create the partitions############################
sudo parted -s $LOOPDEVICE mklabel msdos
sudo parted -s $LOOPDEVICE mkpart primary fat32 0% 5%    #boot-partition
sudo parted -s $LOOPDEVICE set 1 boot on  
sudo parted -s $LOOPDEVICE mkpart primary ext3 5% 40%    #root1-partition
sudo parted -s $LOOPDEVICE mkpart primary 40% 75%        #root2-partition
sudo parted -s $LOOPDEVICE mkpart extended 75% 100%      #extended-partition
sudo parted -s $LOOPDEVICE mkpart logical  ext3 75% 98%  #settings-partition
sudo parted -s $LOOPDEVICE mkpart logical  ext3 98% 100%

####################format the partitions############################
printf "Formating boot partition ................................ "
    $SUDO mkfs.vfat -n boot "${LOOPDEVICE}p1" 1>/dev/null 2>/dev/null
    test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

printf "Formating root1 partition ............................... "
    $SUDO mkfs.ext3 -L $ROOT1_LABEL "${LOOPDEVICE}p2" 1>/dev/null 2>/dev/null
    test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

printf "Formating root2 partition ............................... "
    $SUDO mkfs.ext3 -L $ROOT2_LABEL "${LOOPDEVICE}p3" 1>/dev/null 2>/dev/null
    test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

#note: /dev/loop0p4 is allocated to extended partition
printf "Formating settings partition ............................ "
    $SUDO mkfs.ext3 -L $STTNG_LABEL "${LOOPDEVICE}p5" 1>/dev/null 2>/dev/null
    test 0 -eq $? && echo "[OK]" || echo "[FAIL]"
#note: /dev/loop0p6 is allocated to userdata partition

####################mount the partitions############################
printf "Mounting loopdevice boot partition ...................... "
    $SUDO mount "${LOOPDEVICE}p1" "$BOOTMOUNTPOINT"
    test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

printf "Mounting loopdevice root1 partition ..................... "
    $SUDO mount "${LOOPDEVICE}p2" "$ROOTMOUNTPOINT"
    test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

printf "Mounting loopdevice root2 partition ..................... "
    $SUDO mount "${LOOPDEVICE}p3" "$ROOT2MOUNTPOINT"
    test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

printf "Mounting loopdevice settings partition .................. "
    $SUDO mount "${LOOPDEVICE}p5" "$STTNGMOUNTPOINT"
    test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

##################copy data to partitions#########################
printf "copying boot files - this may take some time ............ "
    $SUDO cp $RPI_FMW_DIR/* "$BOOTMOUNTPOINT" 1>/dev/null 2>/dev/null
    $SUDO cp $RPI_KERNEL "$BOOTMOUNTPOINT"
    $SUDO cp $RPI_DTB "$BOOTMOUNTPOINT"
    $SUDO cp $BOOT_MARKER_RPI "$BOOTMOUNTPOINT"
    $SUDO cp $BOOT_CONFIG_RPI "$BOOTMOUNTPOINT"
    test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

printf "copying root1 files - this may take some time ........... "
    $SUDO tar -C "$ROOTMOUNTPOINT" -Jxf "$ROOTFS"
    test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

printf "copying root2 files - this may take some time ........... "
    $SUDO tar -C "$ROOT2MOUNTPOINT" -Jxf "$ROOTFS"
    test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

printf "copying settings files - this may take some time ........ "
    $SUDO cp $SAMPLE_MEDIA "$STTNGMOUNTPOINT" #copy sample media file to settings partition
    $SUDO cp $CUSTOM_STARTUP "$STTNGMOUNTPOINT" #copy startup script to settings partition
    test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

##################unmount the partitions############################
 printf "unmounting and deleting mountpoint0 %s .." "$BOOTMOUNTPOINT"
     $SUDO umount "$BOOTMOUNTPOINT"
     rm -rf "$BOOTMOUNTPOINT" 
     test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

 printf "unmounting and deleting mountpoint1 %s .." "$ROOTMOUNTPOINT"
     $SUDO umount "$ROOTMOUNTPOINT"
     rm -rf "$ROOTMOUNTPOINT" 
     test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

 printf "unmounting and deleting mountpoint2 %s .." "$ROOT2MOUNTPOINT"
     $SUDO umount "$ROOT2MOUNTPOINT"
     rm -rf "$ROOT2MOUNTPOINT" 
     test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

 printf "unmounting and deleting mountpoint3 %s .." "$STTNGMOUNTPOINT"
     $SUDO umount "$STTNGMOUNTPOINT"
     rm -rf "$STTNGMOUNTPOINT" 
     test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

 printf "syncing ................................................. "
     sync
     test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

 printf "creating output image ................................... "
     cp $IMAGENAME $OUTPUT_DISK
     chmod +r $OUTPUT_DISK 
     chmod +w $OUTPUT_DISK 
     test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

 printf "cleaning up ............................................. "
     $SUDO losetup -d "$LOOPDEVICE" 
     rm -rf "$IMAGENAME"
     test 0 -eq $? && echo "[OK]" || echo "[FAIL]"


