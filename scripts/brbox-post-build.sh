#!/bin/sh
#this script gets called by buildroot after building everything(but before creating rootf.tar)
#here we are copying version information to /boot/version-num.txt of the target file system
#BRBOX_RELVERSION=00.01 #default values incase if make was called without rel-ver
#BRBOX_BUILDNUM=12345   #default values incase if make was called without build-num
echo "$BRBOX_RELVERSION.$BRBOX_BUILDNUM" > ${TARGET_DIR}/boot/version-num.txt
ln -s /tmp/http.service ${TARGET_DIR}/etc/avahi/services/http.service 
