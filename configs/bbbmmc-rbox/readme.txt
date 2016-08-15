how to create bootable sdcard image and upgrade image for bbbmmc-rbox:
cd ~/sandbox/;svn co https://github.com/hackboxguy/brbox;cd brbox/trunk;ln -s ~/dl dl;./prepare-buildroot.sh -v 2016.08-rc1 ;time ./build-image.sh -o /mnt/buildramdisk -v 00.01 -s <myrootpw> -b bbbmmc-rbox
