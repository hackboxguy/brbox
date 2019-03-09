#!/bin/sh
#this script gets called by buildroot after building everything(but before creating rootf.tar)
#here we are copying version information to /boot/version-num.txt of the target file system
#BRBOX_RELVERSION=00.01 #default values incase if make was called without rel-ver
#BRBOX_BUILDNUM=12345   #default values incase if make was called without build-num
#BRBOX_SYSCONFIG application specific distribution e.g:raspi1-rbox or bbbmmc-rbox etc..
if [ ! -d "${TARGET_DIR}/boot" ]; then
	#for bbb /target/boot folder doesnt exists
	mkdir -p ${TARGET_DIR}/boot
fi

if [ ! -d "${TARGET_DIR}/boot/grub" ]; then
        #for bbb /target/boot folder doesnt exists
        mkdir -p ${TARGET_DIR}/boot/grub
fi

echo "$BRBOX_RELVERSION.$BRBOX_BUILDNUM" > ${TARGET_DIR}/boot/version-num.txt
echo "$BRBOX_SYSCONFIG" > ${TARGET_DIR}/boot/sysconfig.txt
rm -rf ${TARGET_DIR}/etc/avahi/services/http.service
ln -s /tmp/http.service ${TARGET_DIR}/etc/avahi/services/http.service 
