# brbox
brbox is a bare-metal linux system built with buildroot. brbox provides the basic features for an embedded-linux system which can then be used as a foundation for extending different application on top. Some of the basic features provided by brbox are as follows
  1. Failsafe in-system programmability of the linux system via network(it uses a concept of backup and main rootfs copies)
  2. Includes a basic web-interface for upgrading brbox linux image.
  3. Includes zeroconf announcement on the network(easy to detect ip of headless-embedded-linux boxes running brbox).
  4. Monitoring of system parameters like ip,mac,ram-size,cpu-load etc via web-interface
 

###How to prepare fresh ubuntu 14.04 for brbox
1. ```sudo mkdir -p /mnt/buildramdisk```
2. ```sudo chown user:user /mnt/buildramdisk``` (replace user with correct username)
3. ```sudo apt-get update```
4. ```sudo apt-get install subversion git ckermit build-essential gcc-multilib libncurses5-dev expect```
5. ```sudo vi /etc/fstab``` (add last line as "tmpfs   /mnt/buildramdisk    tmpfs   defaults,gid=100,size=6G 0 0"

###How to start brbox from fresh?
1. ```mkdir -p /home/user/dl``` (this will ensure that your downloads are intact even if you delete whole brbox sandbox)
2. ```mkdir -p /home/user/sandbox```
3. ```cd /home/user/sandbox```
4. ```svn co http://github.com/hackboxguy/brbox```      (if asked, enter your git username and password)
5. ```cd /home/user/sandbox/brbox/trunk```
6. ```ln -s /home/user/dl dl```   (as stated in step-1, this softlink ensure that your downloads are intact)
7. ```./prepare-buildroot.sh```
8. ```./build-image.sh -o /mnt/buildramdisk -v 01.00 -s mypw```    (-o is the output folder -v is the version number of the build, -s is your sudo passwd)

For the first time, this will take about 1 to 2 hours (because packages are to be downloaded fresh to dl - download directory)
after ./build-image.sh is complete, you will see following files under /mnt/buildramdisk/01.00/output/images
  1. /mnt/buildramdisk/01.00/images/bootable-usb-disk.img (this is the image to be copied to usb-flash-memory using dd)
  2. /mnt/buildramdisk/01.00/images/uBrBoxRoot.uimg   (this is the in-system upgrade package - linux can be updated without removing usb-flash)

###what to do once the image is built by build-image.sh script?
1. ```sudo dd if=/mnt/buildramdisk/00.01/images/bootable-usb-disk.img of=/dev/sdX bs=1M``` (use this step for creating bootable usb-flash-drive, replace /dev/sdX with correct device-node)
2. Ensure that your motherboard's bios has set to boot from usb-flash-drive, and its EFI-boot is disabled
3. Insert the usb-flash-drive and power your motherboard, wait for login prompt, and use root to login

###how to upgrade brbox via network without physically removing the usb-flash-drive?
1. ```scp /mnt/buildramdisk/00.01/images/uBrBoxRoot.uimg root@ip-of-brbox:/tmp/```
2. ```ssh root@ip-of-brbox (use root to login)```
3. ```brdskmgr -v``` (shows the version of current brbox)
4. ```update -u /tmp/uBrBoxRoot.uimg```
5. ```reboot``` (this will reboot the system with newly programmed brbox)
6. ```ssh root@ip-of-brbox (use root to login)```
7. ```brdskmgr -v``` (verify if the version is as per the latest updated brbox)

###Simplified steps to create BrBox based bootable sdcard for RaspberryPi-1 
1. ```cd /home/user/sandbox/;svn co https://github.com/hackboxguy/brbox;cd brbox/trunk;ln -s /home/user/dl dl;./prepare-buildroot.sh;time ./build-image.sh -o /mnt/buildramdisk -v 00.01 -s my_sudo_pw -b raspi1```
2. ```sudo sh -c "pv -tpreb /mnt/buildramdisk/00.01/images/bootable-usb-disk.img | dd bs=128k of=/dev/sdX conv=fdatasync"```
3. Insert the sdcard and boot Raspi and login using root/brb0x

