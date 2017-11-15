#/bin/sh
RASPI_DISK=/mnt/userdata/raspi-simulator.img.xz
#RASPI_DISK=/tmp/raspi-simulator.img.xz
DISK_IMAGE=$RASPI_DISK
##############################################################################
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
###############################################################################
PrintLcdMsg line2 "   step-1-of-4   "
printf "unmounting partitions.......... "
for f in `ls /dev/sda*`
do
	umount $f 1>/dev/null 2>/dev/null
done
echo "[OK]"
###############################################################################
#/opt/fmw/bin/dispclt --printline=line2,"   step-2-of-12   " > /dev/null
PrintLcdMsg line2 "   step-2-of-4   "
printf "writing image to disk.......... "
xzcat -d $DISK_IMAGE | dd of=/dev/sda bs=1M 1>/dev/null 2>/dev/null
test 0 -eq $? && echo "[OK]" || echo "[FAIL]"
###############################################################################
PrintLcdMsg line2 "   step-3-of-4   "
printf "syncing disk................... "
sync
test 0 -eq $? && echo "[OK]" || echo "[FAIL]"
###############################################################################
PrintLcdMsg line2 "   step-4-of-4   "
printf "unmounting partitions again.... "
for f in `ls /dev/sda*`
do
	umount $f 1>/dev/null 2>/dev/null
done
echo "[OK]"
###############################################################################
PrintLcdMsg line2 "   step-12-of-12   "
printf "done,disk is ready :-)......... "
echo "[OK]"
/opt/fmw/bin/dispclt --dispclear >/dev/null
/opt/fmw/bin/dispclt --printline=line2,"   disk ready :-)  " > /dev/null
###############################################################################
