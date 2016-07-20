#!/bin/sh

#sudo ./testeeprom.sh -d /dev/i2c-0 -l 5 -s 1024 -a 0x50
#sudo ./testeeprom.sh -d /dev/i2c-0 -l 5 -s 1024 -a 0x52
#sudo ./testeeprom.sh -d /dev/i2c-0 -l 5 -s 1024 -a 0x54
#sudo ./testeeprom.sh -d /dev/i2c-0 -l 5 -s 1024 -a 0x56
#y=10000;x=0; while [ $x -lt $y ]; do ./bin/dispclt --printline=line1,"$(date +"%T")"; let x=$x+1; done

SUCCESS=0
ERR_FILE_OPEN=1
ERR_FILE_READ=2
ERR_FILE_WRITE=3
ERR_ARGC=4
ERR_ARGV=5
ERR_HOST=6
ERR_SAME=7
ERR_DIFF=8
ERR_FILE_DOESNT_EXIST=9
USAGE="script for testing i2c eeprom read/write/verify"
USAGE1="usage:./$0 -d </dev/i2c-x node> -a <i2c_dev_addr> -s <testsize> -l <loopcount>"
DEVNODE="none"
LOOPCOUNT=1
FILESIZE=64
DEVADDR="none"
LOOP_DELAY=1
EEPROGBIN=./eeprog

while getopts d:s:l:a: f
do
    case $f in
	d) DEVNODE=$OPTARG ;;
	s) FILESIZE=$OPTARG ;;
	l) LOOPCOUNT=$OPTARG ;;
	a) DEVADDR=$OPTARG ;;
    esac
done

if [ $DEVNODE = "none" ]; then
	echo "Error:  i2c device node not specified!"
	echo $USAGE1
	exit $ERR_ARGC
fi

if [ $DEVADDR = "none" ]; then
	echo "Error:  i2c device address not specified!"
	echo $USAGE1
	exit $ERR_ARGC
fi

#echo "devnode  =$DEVNODE"
#echo "loopcount=$LOOPCOUNT"
#echo "filesize =$FILESIZE"
#echo "devaddr =$DEVADDR"

WRITEFILE=$(mktemp)
READFILE=$(mktemp)
dd if=/dev/urandom bs=1 count=$FILESIZE of=$WRITEFILE 2>/dev/null
x=0
while [ $x -lt $LOOPCOUNT ]; do
	RET=$($EEPROGBIN -f -16 -w 0x00 -t 5 $DEVNODE $DEVADDR 2>/dev/null < $WRITEFILE)
	[ $? != "0" ] && { echo "file write failed  :loop_count=$x" ; break; }
	RET=$($EEPROGBIN -f -16 $DEVNODE $DEVADDR -r 0:$FILESIZE 2>/dev/null > $READFILE)
	[ $? != "0" ] && { echo "file read failed   :loop_count=$x" ; break; }
	RET=$(cmp $WRITEFILE $READFILE)
	[ $? != "0" ] && { echo "file compare failed:loop_count=$x" ; break; }
	#sleep $LOOP_DELAY
	x=$(($x+1))
	echo "loop count = $x"
done

rm -rf $WRITEFILE $READFILE
