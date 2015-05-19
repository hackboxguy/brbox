interesting raspi addon:
http://blog.koalo.de/2013/03/8-tft-for-raspberry-pi.html
#####################################################
#######how to blink ACT LED on raspi?################
echo none > /sys/class/leds/led0/trigger  #this will ensure that no other process will access this led
echo 1 > /sys/class/leds/led0/brightness  #switch ON led
echo 0 > /sys/class/leds/led0/brightness  #switch OFF led
#####################################################

sudo ./mkcard.txt /dev/sdd
sudo mount /dev/sdd1 /mnt/rpboot
sudo mount /dev/sdd2 /mnt/rproot
sudo cp images/rpi-firmware/* /mnt/rpboot
sudo cp images/zImage /mnt/rpboot

sudo tar xf ./images/rootfs.tar -C /mnt/rproot



mount /dev/mmcblk0p1 /mnt/boot

################kernel args#######################
vi /mnt/boot/cmdline.txt
ro root=/dev/mmcblk0p2 rootwait brboxsystem=bebox1
or
ro root=/dev/mmcblk0p3 rootwait brboxsystem=bebox2
################boot args#########################
vi /mnt/boot/config.txt
kernel=zImage1
or
kernel=zImage2


###############5sec delay in S40Network script###




sudo dd if=/home/adav/tmp/rp/images/rp-sd.img of=/dev/sdd bs=1M




###########################different approach#########################
1 cylinder ~=8MB, so 16=131MB
0 - 2  (2-16M)
2   12 (10-80M)
12  22 (10-80M)

dd if=/dev/zero of=~/rp.img bs=1024 count=0 seek=$[1000*256]
sudo losetup /dev/loop0 ~/rp.img 
sudo parted -s /dev/loop0 mklabel msdos
sudo parted -s /dev/loop0 unit cyl mkpart primary fat32 -- 0 16  
sudo parted -s /dev/loop0 set 1 boot on  
sudo parted -s /dev/loop0 unit cyl mkpart primary ext2 -- 16 -2  
sudo mkfs.vfat -n System /dev/loop0p1  
sudo mkfs.ext4 -L Storage /dev/loop0p2  

##############################################
sudo fdisk -l ~/rp.img
##############################################
adav@adav-private-linux:~/sandbox/brbox/trunk/scripts$ sudo parted /dev/loop0
GNU Parted 2.3
Using /dev/loop0
Welcome to GNU Parted! Type 'help' to view a list of commands.
(parted) p                                                                
Model: Loopback device (loop)
Disk /dev/loop0: 262MB
Sector size (logical/physical): 512B/512B
Partition Table: msdos

Number  Start   End    Size   Type     File system  Flags
 1      1049kB  132MB  131MB  primary  fat16        boot, lba
 2      132MB   245MB  113MB  primary  ext4
#################################################################
sudo mount /dev/loop0p1 /mnt/rpboot
sudo mount /dev/loop0p2 /mnt/rproot
sudo cp /home/adav/tmp/rp/images/rpi-firmware/* /mnt/rpboot/
sudo cp /home/adav/tmp/rp/images/zImage /mnt/rpboot/
sudo tar -C /mnt/rproot/ -Jxf /home/adav/tmp/rp/images/rootfs.tar.xz
sync
sudo umount /mnt/rpboot
sudo umount /mnt/rproot
sudo losetup -d /dev/loop0

sudo dd if=tmp/raspi.img of=/dev/sdd bs=1M



##########partition sizes of newly created image using script####
sudo parted /dev/sdd

GNU Parted 2.3
Using /dev/sdd
Welcome to GNU Parted! Type 'help' to view a list of commands.
(parted) print                                                            
Model: Generic STORAGE DEVICE (scsi)
Disk /dev/sdd: 7948MB
Sector size (logical/physical): 512B/512B
Partition Table: msdos

Number  Start   End     Size    Type     File system  Flags
 1      1049kB  16,8MB  15,7MB  primary  fat16        boot, lba
 2      16,8MB  98,6MB  81,8MB  primary  ext3
 3      98,6MB  181MB   82,8MB  primary  ext3
 4      181MB   252MB   70,3MB  primary  ext3

