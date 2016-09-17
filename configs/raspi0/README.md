###How to prepare bootable sdcard image from sources for raspi0-awesome-IoT-box?
1. ```sudo apt-get install subversion git ckermit build-essential gcc-multilib libncurses5-dev expect```
2. ```cd /home/myuser/sandbox/```
3. ```svn co https://github.com/hackboxguy/brbox;cd brbox/trunk```
4. ```ln -s /home/myuser/dl dl```
5. ```./prepare-buildroot.sh```
5. ```./build-image.sh -o /home/myuser/temp -v 00.01 -s myuser_root_pw -b raspi0``` (this will take couple of housrs to build)
6. After build is complete, you will find two binary images(raspi0.uimg and bootable-usb-disk.img) under /home/myuser/temp/00.01/images/
7. Using dd(or Win32DiskImager.exe on windows) command, write bootable-usb-disk.img to the sdcard.
8. Remove the sdcard and re-insert, you will see 4 partitions mounted on your ubuntu pc.
9. Under boot partition of this sdcard, create a file called xmpp-login.txt, fill this file with username/pw of xmpp account and usb-3g configuration.
