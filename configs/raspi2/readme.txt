sudo ./mkcard.txt /dev/sdd
sudo mount /dev/sdd1 /mnt/rpboot
sudo mount /dev/sdd2 /mnt/rproot
sudo cp images/rpi-firmware/* /mnt/rpboot
sudo cp images/zImage /mnt/rpboot

sudo tar xf ./images/rootfs.tar -C /mnt/rproot
