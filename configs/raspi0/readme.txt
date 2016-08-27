cd /home/adav/sandbox/;svn co https://github.com/hackboxguy/brbox;cd brbox/trunk;ln -s /home/adav/dl dl;./prepare-buildroot.sh -v 2016.08-rc2
cd /home/adav/sandbox/brbox/trunk
cp configs/raspi0/raspberrypi0_defconfig buildroot/configs/
cp configs/raspi0/busybox.config buildroot/package/busybox/
cd buildroot
make O=/mnt/buildramdisk/00.01 raspberrypi0_defconfig BRBOX_RELVERSION=00.01 BRBOX_BUILDNUM=12345
make O=/mnt/buildramdisk/00.01 menuconfig BRBOX_RELVERSION=00.01 BRBOX_BUILDNUM=12345
sudo dd if=/mnt/buildramdisk/00.01/images/sdcard.img of=/dev/sdX
