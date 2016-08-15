#!/bin/sh
#this script generates a jpg file consisting of logo + ip/version overlay
#feh application will use this jpg and show as desktop.
#on ip address change event, /usr/share/udhcpc/default.script will trigger new desktop-screen.jpg creation, and
#and sends "r"(reload key) to feh image viewer through xdotool
BKGND_IMAGE=/etc/X11/logo.png #/tmp/bkgnd.jpg # /etc/X11/logo.png
OUTPUT_IMAGE=/tmp/desktop-screen.jpg
INFO_COLOR=lime #black
#export DISPLAY=:0.0

# Create white background image
#convert -size 100x60 xc:white $BKGND_IMAGE #base.jpg

# Create IP image
#convert $BKGND_IMAGE -pointsize 30 -fill black -draw "text 0,30 'IPv4: $(ip -4 a s eth0 | grep -Eo 'inet [0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}' | awk '{print $2}')'" -fill black -draw "text -0,60 'Hostname: $(uname -n)'" -pointsize 30 -draw "text -0,90 'Date $(date)'" $OUTPUT_IMAGE

convert $BKGND_IMAGE -pointsize 30 -fill $INFO_COLOR -draw "text 0,30 'IPv4: $(ip -4 a s eth0 | grep -Eo 'inet [0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}' | awk '{print $2}')'" -fill $INFO_COLOR -draw "text -0,60 'Version: $(cat /boot/version-num.txt)'" $OUTPUT_IMAGE

#feh $OUTPUT_IMAGE  

