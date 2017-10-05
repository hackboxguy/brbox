#!/bin/sh
while true; do
	export DISPLAY=:0;(gst-play-1.0 $1 --videosink=xvimagesink &)
	wait
	if [ -f /tmp/omxplay.stoploop ]; then
		rm -rf /tmp/omxplay.stoploop
		#fbset -depth 8 && fbset -depth 16
		#touch /tmp/omxplay.finished
		break 
	fi
done

