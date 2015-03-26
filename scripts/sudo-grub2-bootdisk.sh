#!/usr/bin/expect -f

#./sudo-grub2-bootdisk.sh /home/adav/sandbox/brbox/trunk/scripts/grub2-bootdisk.sh /mnt/buildramdisk/x86 02.00.23456 /mnt/buildramdisk/x86/images/out.disk.img <sudo_pw>

# set standard arguments to local Variables
set grub2scr      [lrange $argv 0 0]
set outfolder     [lrange $argv 1 1]
set diskversion   [lrange $argv 2 2]
set outimgname    [lrange $argv 3 3]
set sudo_password [lrange $argv 4 4]

set timeout -1
spawn sudo -k $grub2scr -o "$outfolder" -v "$diskversion" -i $outimgname

match_max 100000
expect "*?assword"
send -- "$sudo_password\r"
expect eof

