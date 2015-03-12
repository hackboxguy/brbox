#!/usr/bin/expect -f

# set standard arguments to local Variables
set grubin  [lrange $argv 0 0]
set diskimg [lrange $argv 1 1]
#todo scan disk.img file path
set timeout -1

# now run the grub binary
spawn $grubin --device-map=/dev/null
send "device (hd0) $diskimg\r" 
send "geometry (hd0) 4096 16 63\r"
send "root (hd0,0)\r"
send "setup (hd0)\r"
send "quit\r"
expect eof

