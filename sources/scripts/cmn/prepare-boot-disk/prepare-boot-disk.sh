#/bin/sh
RASPI1_DISPTST=/mnt/userdata/raspi1-disptst.img.xz
DISK_IMAGE=$RASPI1_DISPTST
#todo: check user argument, and select correct image type
#add lcd display message
#return if sdcard is not found.
###############################################################################
PrintLcdMsg() #$1=line_nuber $2-msg-string
{
/opt/fmw/bin/dispclt --dispclear >/dev/null
#sleep 1
/opt/fmw/bin/dispclt --printline=line1,"  preparing disk...  " > /dev/null
#sleep 1
/opt/fmw/bin/dispclt --printline=$1,"$2" > /dev/null
}
###############################################################################
printf "detecting external disk........ "
if [ -b "/dev/sda" ]; then
	echo "[OK]"
else
	echo "[FAIL]"
	exit 1
fi

#/opt/fmw/bin/dispclt --dispclear >/dev/null
#PrintLcdMsg line1 "  prepering disk...  "
###############################################################################
#/opt/fmw/bin/dispclt --printline=line2,"   step-1-of-12   " > /dev/null
PrintLcdMsg line2 "   step-1-of-12   "
printf "unmounting partitions.......... "
for f in `ls /dev/sda*`
do
	umount $f 1>/dev/null 2>/dev/null
done
echo "[OK]"
###############################################################################
#/opt/fmw/bin/dispclt --printline=line2,"   step-2-of-12   " > /dev/null
PrintLcdMsg line2 "   step-2-of-12   "
printf "writing image to disk.......... "
xzcat -d $DISK_IMAGE | dd of=/dev/sda bs=1M 1>/dev/null 2>/dev/null
test 0 -eq $? && echo "[OK]" || echo "[FAIL]"
###############################################################################
#/opt/fmw/bin/dispclt --printline=line2,"   step-3-of-12   " > /dev/null
PrintLcdMsg line2 "   step-3-of-12   "
printf "syncing disk................... "
sync
test 0 -eq $? && echo "[OK]" || echo "[FAIL]"
###############################################################################
#/opt/fmw/bin/dispclt --printline=line2,"   step-4-of-12   " > /dev/null
PrintLcdMsg line2 "   step-4-of-12   "
printf "unmounting partitions again.... "
for f in `ls /dev/sda*`
do
	umount $f 1>/dev/null 2>/dev/null
done
echo "[OK]"
###############################################################################
#/opt/fmw/bin/dispclt --printline=line2,"   step-5-of-12   " > /dev/null
PrintLcdMsg line2 "   step-5-of-12   "
printf "extending the partition........ "
echo "d
6
d
5
d
4
n
e


n

+500M
n


w
"|fdisk /dev/sda 1>/dev/null 2>/dev/null
test 0 -eq $? && echo "[OK]" || echo "[FAIL]"
###############################################################################
#/opt/fmw/bin/dispclt --printline=line2,"   step-6-of-12   " > /dev/null
PrintLcdMsg line2 "   step-6-of-12   "
printf "unmounting partitions again.... "
#more partitions will show up, unmount them again
for f in `ls /dev/sda*`
do
	umount $f 1>/dev/null 2>/dev/null
done
echo "[OK]"
###############################################################################
#/opt/fmw/bin/dispclt --printline=line2,"   step-7-of-12   " > /dev/null
PrintLcdMsg line2 "   step-7-of-12   "
printf "formatting partition /dev/sda5. "
mkfs.ext3 /dev/sda5 1>/dev/null 2>/dev/null
test 0 -eq $? && echo "[OK]" || echo "[FAIL]"
###############################################################################
#/opt/fmw/bin/dispclt --printline=line2,"   step-8-of-12   " > /dev/null
PrintLcdMsg line2 "   step-8-of-12   "
printf "formatting partition /dev/sda6. "
mkfs.ext3 /dev/sda6 1>/dev/null 2>/dev/null
test 0 -eq $? && echo "[OK]" || echo "[FAIL]"
###############################################################################
#/opt/fmw/bin/dispclt --printline=line2,"   step-9-of-12   " > /dev/null
PrintLcdMsg line2 "   step-9-of-12   "
printf "unmounting partitions again.... "
#more partitions will show up, unmount them again
for f in `ls /dev/sda*`
do
	umount $f 1>/dev/null 2>/dev/null
done
echo "[OK]"
###############################################################################
#/opt/fmw/bin/dispclt --printline=line2,"   step-10-of-12   " > /dev/null
PrintLcdMsg line2 "   step-10-of-12   "
printf "copying media files............ "
TMPFILE=$(mktemp -d)
mount /dev/sda6 $TMPFILE
cp /mnt/userdata/test.h264 $TMPFILE
cp /mnt/userdata/rc.local $TMPFILE
test 0 -eq $? && echo "[OK]" || echo "[FAIL]"
###############################################################################
#/opt/fmw/bin/dispclt --printline=line2,"   step-11-of-12   " > /dev/null
PrintLcdMsg line2 "   step-11-of-12   "
printf "syncing disk................... "
sync
test 0 -eq $? && echo "[OK]" || echo "[FAIL]"
###############################################################################
#/opt/fmw/bin/dispclt --printline=line2,"   step-12-of-12   " > /dev/null
PrintLcdMsg line2 "   step-12-of-12   "
umount $TMPFILE
sync
rm -rf "$TMPFILE"
printf "done,disk is ready :-)......... "
echo "[OK]"
/opt/fmw/bin/dispclt --dispclear >/dev/null
#PrintLcdMsg line2 "   disk ready :-)  "
/opt/fmw/bin/dispclt --printline=line2,"   disk ready :-)  " > /dev/null
###############################################################################

