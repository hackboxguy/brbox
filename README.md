# brbox
buildroot based distribution targeted for intel mini-boxes based on J1900

###How to prepare fresh ubuntu 14.04 for brbox
1. ```sudo mkdir -p /mnt/buildramdisk```
2. ```sudo chown user:user /mnt/buildramdisk``` (replace user with correct username)
3. ```sudo apt-get update```
4. ```sudo apt-get install subversion git ckermit build-essential gcc-multilib libncurses5-dev expect```
5. ```sudo vi /etc/fstab``` (add last line as "tmpfs   /mnt/buildramdisk    tmpfs   defaults,gid=100,size=6G 0 0"

###How to start brbox from fresh?
1. ```mkdir -p /home/user/sandbox```
2. ```cd /home/user/sandbox```
3. ```svn co http://github.com/hackboxguy/brbox```      (when asked, enter your git username and password)
4. ```cd /home/user/sandbox/brbox/trunk```
5. ```./prepare-buildroot.sh```
6. ```./build-image.sh -o /mnt/buildramdisk -v 01.00    (-o is the output folder -v is the version number of the build)```

For the first time, this will take about 1 to 2 hours (because packages are to be downloaded fresh to download directory)
after ./build-image.sh is complete, you will see following files under /mnt/buildramdisk/01.00/output/images
  1. /mnt/buildramdisk/01.00/images/out.disk.img (this is the image to be copied to usb-flash-memory)
  2. /mnt/buildramdisk/01.00/images/rootfs.tar   (root file system for future use)

use ```sudo dd if=/mnt/buildramdisk/01.00/images/out.disk.img of=/dev/sdX bs=1M``` (replace /dev/sdX with correct device node)
