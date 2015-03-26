#!/bin/sh

IMAGESIZE=32M
FSSIZE=28M
BR_OUTPUT_FOLDER=/mnt/buildramdisk/x86
BINARIES_DIR=$BR_OUTPUT_FOLDER/images
SUDO=
#SUDOPW=none
IMAGE_VERSION=01.00.12345
#OUTPUT_FILENAME="bootable-usb-disk.img"
OUTPUT_DISK=bootable-usb-disk.img
while getopts o:v:i: f
do
    case $f in
	o) BR_OUTPUT_FOLDER=$OPTARG;;  #buildroot output folder
	v) IMAGE_VERSION=$OPTARG ;;    #image version
	i) OUTPUT_DISK=$OPTARG ;;
    esac
done


IMAGENAME=$(mktemp)
MKIMG_TIMESTAMP=$(date +%Y%m%d%H%M%S)
#OUTPUT_FILENAME="disk_$MKIMG_TIMESTAMP.img.gz"
#OUTPUT_DISK="$BINARIES_DIR/$OUTPUT_FILENAME"
ROOTFS="$BINARIES_DIR/rootfs.tar"
ROOTMOUNTPOINT=$(mktemp -d)

printf "creating image file ..................................... "
    #$SUDO fallocate -l "$IMAGESIZE" "$IMAGENAME"
    fallocate -l "$IMAGESIZE" "$IMAGENAME" >/dev/null
    test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

printf "partitioning image %s ... " "$IMAGENAME"
    #$SUDO sgdisk -Z "$IMAGENAME"
    #$SUDO sgdisk -n 1::+1M -t 1:4 -c 1:boot "$IMAGENAME"
    #$SUDO sgdisk -n 2::+"$FSSIZE" -c 2:root "$IMAGENAME"
    sgdisk -Z "$IMAGENAME" >/dev/null
    sgdisk -n 1::+1M -t 1:ef02 -c 1:boot "$IMAGENAME" >/dev/null
    sgdisk -n 2::+"$FSSIZE" -c 2:root "$IMAGENAME" >/dev/null
    printf "partitioning . "
    test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

printf "Find free loopdevice %s ... " "$LOOPDEVICE"
    LOOPDEVICE=$($SUDO losetup -f)
    test 0 -eq $? && printf "found %s .............. [OK]\n" "$LOOPDEVICE" || echo "[FAIL]"

printf "Creating loopdevice ..................................... "
    $SUDO losetup "$LOOPDEVICE" "$IMAGENAME"
    $SUDO partx -a  "$LOOPDEVICE"
    test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

printf "Formating root partition ................................ "
    $SUDO mkfs.ext3 -L root "${LOOPDEVICE}p2" 1>/dev/null 2>/dev/null
    test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

printf "Mounting loopdevice root partition ...................... "
    $SUDO mount "${LOOPDEVICE}p2" "$ROOTMOUNTPOINT"
    test 0 -eq $? && echo "[OK]" || echo "[FAIL]"


printf "copying files - this may take some time ................. "
    $SUDO tar -C "$ROOTMOUNTPOINT" -xf "$ROOTFS"
    test 0 -eq $? && echo "[OK]" || echo "[FAIL]"


printf "generating grub config files ............................ "
    ROOTUUID=$($SUDO /usr/bin/linux64 /sbin/blkid "$LOOPDEVICE"p2 -s UUID |cut -d\" -f2)
    PARTUUID=$($SUDO sgdisk -i=2 "$LOOPDEVICE" |grep 'Partition unique GUID' |cut -d\  -f4)
    test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

printf "compiled in grub.cfg .................................... "
cat <<ENDOFGRUBCFG >grub.cfg.in
insmod part_gpt
insmod part_msdos
insmod ext2
search -u $ROOTUUID -s root
configfile /boot/grub/grub.cfg
ENDOFGRUBCFG
test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

printf "real grub.cfg ........................................... "
cat <<ENDOFGRUBCFG >grub.cfg
set timeout=5
insmod ext2
menuentry 'brbox' {
    search -u $ROOTUUID -s root
    linux /boot/bzImage rootdelay=15 root=PARTUUID="$PARTUUID" consoleblank=0 rw mem=0x80000000 enable_mtrr_cleanup mtrr_spare_reg_nr=1
}
ENDOFGRUBCFG
test 0 -eq $? && echo "[OK]" || echo "[FAIL]"


printf "regenerate grub.img ..................................... "
    $BR_OUTPUT_FOLDER/host/usr/bin/grub-mkimage -d $BR_OUTPUT_FOLDER/host/usr/lib/grub/i386-pc \
        -O i386-pc -o $BR_OUTPUT_FOLDER/images/grub.img -c ./grub.cfg.in \
        acpi cat boot linux ext2 fat part_msdos part_gpt normal biosdisk \
        search echo search_fs_uuid normal ls ata configfile halt help \
        hello read png vga lspci echo minicmd vga_text terminal
    test 0 -eq $? && echo "[OK]" || echo "[FAIL]"


 printf "installing grub - this may take some time ............... "
     $SUDO /usr/bin/linux64 $BR_OUTPUT_FOLDER/host/usr/sbin/grub-bios-setup \
         -b $BR_OUTPUT_FOLDER/host/usr/lib/grub/i386-pc/boot.img \
         -c $BR_OUTPUT_FOLDER/images/grub.img -d / "$LOOPDEVICE"
     test 0 -eq $? && echo "[OK]" || echo "[FAIL]"


 printf "installing grub.cfg ..................................... "
     $SUDO /usr/bin/linux64 cp grub.cfg "$ROOTMOUNTPOINT"/boot/grub/grub.cfg
     test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

 echo $IMAGE_VERSION > ./version-num.txt
 $SUDO cp ./version-num.txt "$ROOTMOUNTPOINT"/boot/

 printf "unmounting and deleting mountpoint %s ..." "$ROOTMOUNTPOINT"
     $SUDO umount "$ROOTMOUNTPOINT"
     rm -rf "$ROOTMOUNTPOINT" 
     test 0 -eq $? && echo "[OK]" || echo "[FAIL]"


 printf "syncing ................................................. "
     sync
     test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

 #printf "Compressing Image: %s ... " "$OUTPUT_DISK"
 #    gzip -c "$IMAGENAME" >"$OUTPUT_DISK"
 #    test 0 -eq $? && echo "[OK]" || echo "[FAIL]"
 printf "creating output image ................................... "
     cp $IMAGENAME $OUTPUT_DISK
     test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

 printf "cleaning up ............................................. "
     $SUDO partx -d "$LOOPDEVICE" 
     $SUDO losetup -d "$LOOPDEVICE" 
     rm -rf "$IMAGENAME"
     rm ./version-num.txt
     rm grub.cfg.in
     rm grub.cfg
     test 0 -eq $? && echo "[OK]" || echo "[FAIL]"


