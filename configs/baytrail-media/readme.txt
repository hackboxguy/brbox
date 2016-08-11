last working config:
cd /home/adav/sandbox/;svn co https://github.com/hackboxguy/brbox;cd brbox/trunk;ln -s /home/adav/dl dl;./prepare-buildroot.sh -v 2016.08-rc1 ;time ./build-image.sh -o /mnt/buildramdisk -v 00.01 -s <myrootpw> -b baytrail-media
