#!/usr/bin/expect -f

# set standard arguments to local Variables
set preparescr    [lrange $argv 0 0]
set outfolder     [lrange $argv 1 1]
set diskimage     [lrange $argv 2 2]
set diskversion   [lrange $argv 3 3]
set sudo_password [lrange $argv 4 4]

set timeout -1
spawn sudo -k $preparescr -o "$outfolder" -i "$diskimage" -v "$diskversion"

match_max 100000
expect "*?assword"
send -- "$sudo_password\r"
expect eof

