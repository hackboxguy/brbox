#!/bin/sh
while true; do
        /usr/bin/omxplayer -b --layer 2 -r -o both $1 < /tmp/omxplay.fifo &
	echo . > /tmp/omxplay.fifo
	wait
	if [ -f /tmp/omxplay.stoploop ]; then
		rm -rf /tmp/omxplay.stoploop
		fbset -depth 8 && fbset -depth 16
		touch /tmp/omxplay.finished
		break 
	fi
done

