# brbox
buildroot based distribution targeted for intel mini-boxes based on J1900

###How to start from fresh?
1. ```mkdir -p /home/user/sandbox```
2. ```cd /home/user/sandbox```
3. ```svn co http://github.com/hackboxguy/brbox```      (when asked, enter your git username and password)
4. ```cd /home/user/sandbox/brbox/```
5. ```./prepare-buildroot.sh```
6. ```./build-image.sh -o /mnt/buildramdisk -v 01.00    (-o is the output folder -v is the version number of the build)```

For the first time, this will take about 1 to 2 hours (becase packages are to be downloaded fresh to download directory)
after ./build-image.sh is complete, you will see following three files under /mnt/buildramdisk/01.00/output/images
  1. /home/user/barco-xl4/Mercury-XL4/trunk/src/buildroot/output/images/bootfs.ext2
  2. /home/user/barco-xl4/Mercury-XL4/trunk/src/buildroot/output/images/rootfs.ext2.xz
  3. /home/user/barco-xl4/Mercury-XL4/trunk/src/buildroot/output/images/uImage (needed only for nfs/tftp based development environment)

