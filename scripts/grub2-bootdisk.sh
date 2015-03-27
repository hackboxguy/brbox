#!/bin/sh
###########################################################
IMAGESIZE=128M
BOOTSIZE=1M
ROOT1SIZE=28M
ROOT2SIZE=28M
STTNGSIZE=2M
USRDATSIZE=64M
ROOT1_LABEL=root1   #linux-1
ROOT2_LABEL=root2   #linux-2
STTNG_LABEL=sttng   #settings
USRDAT_LABEL=usrdat #userdata

#FSSIZE=28M
BR_OUTPUT_FOLDER=/mnt/buildramdisk/x86
SUDO=
IMAGE_VERSION=01.00.12345
OUTPUT_DISK=bootable-usb-disk.img
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
	i) OUTPUT_DISK=$OPTARG ;;
    esac
done

BINARIES_DIR=$BR_OUTPUT_FOLDER/images
IMAGENAME=$(mktemp)
MKIMG_TIMESTAMP=$(date +%Y%m%d%H%M%S)
ROOTFS="$BINARIES_DIR/rootfs.tar"
ROOTMOUNTPOINT=$(mktemp -d)

printf "creating image file ..................................... "
    fallocate -l "$IMAGESIZE" "$IMAGENAME" >/dev/null
    test 0 -eq $? && echo "[OK]" || echo "[FAIL]"

printf "partitioning image %s ... " "$IMAGENAME"
    sgdisk -Z "$IMAGENAME" >/dev/null
    sgdisk -n 1::+"$BOOTSIZE"   -t 1:ef02 -c 1:boot  "$IMAGENAME" >/dev/null
    sgdisk -n 2::+"$ROOT1SIZE"  -c 2:$ROOT1_LABEL    "$IMAGENAME" >/dev/null
    sgdisk -n 3::+"$ROOT2SIZE"  -c 3:$ROOT2_LABEL    "$IMAGENAME" >/dev/null
    sgdisk -n 4::+"$STTNGSIZE"  -c 4:$STTNG_LABEL    "$IMAGENAME" >/dev/null
    sgdisk -n 5::+"$USRDATSIZE" -c 5:$USRDAT_LABEL   "$IMAGENAME" >/dev/null
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
    $SUDO mkfs.ext3 -L $ROOT1_LABEL "${LOOPDEVICE}p2" 1>/dev/null 2>/dev/null
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
set timeout=$GRUB2_TIMEOUT
insmod ext2
menuentry $GRUB_MENU_ENTRY1 {
    search -u $ROOTUUID -s root
    linux /boot/bzImage rootdelay=$ROOTDELAY root=PARTUUID="$PARTUUID" consoleblank=0 rw mem=0x80000000 enable_mtrr_cleanup mtrr_spare_reg_nr=1 brboxsystem=$GRUB_MENU_ENTRY1
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


