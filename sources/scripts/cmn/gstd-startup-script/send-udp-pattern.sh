#!/bin/sh
#./send-udp-pattern.sh -g <gstd_ip> -i <target_ip> -p <stream_port> -w <pattern-width> -h <pattern-height> -f <pattern-fps>
USAGE="./send-udp-pattern.sh -g <gstd_ip> -i <target_ip> -p <stream_port> -w <pattern-width> -h <pattern-height> -f <pattern-fps>"
TARGET_IP=127.0.0.1
STREAM_PORT=6000
GSTD_IP=127.0.0.1  #gstreamer-daemon ip
GSTD_CLIENT_BIN=gstd-client
WIDTH=1920
HEIGHT=1080
FPS=30

while getopts i:p:g:w:h:f: m
do
    case $m in
	i) TARGET_IP=$OPTARG ;;
	p) STREAM_PORT=$OPTARG ;;
	g) GSTD_IP=$OPTARG ;;
	w) WIDTH=$OPTARG ;;
	h) HEIGHT=$OPTARG ;;
	f) FPS=$OPTARG ;;

    esac
done

if [ $# -lt 2  ]; then 
	echo $USAGE
	exit 1
fi

#sysctl -w net.core.rmem_max=98214400
RES=$($GSTD_CLIENT_BIN --address=$GSTD_IP --port=5000 pipeline_delete p1)

RES=$($GSTD_CLIENT_BIN --address=$GSTD_IP --port=5000 create /pipelines p1 videotestsrc pattern=ball flip=0 is-live=1 ! video/x-raw,format=BGRA,width=$WIDTH,height=$HEIGHT,pixel-aspect-ratio=1/1,interlace-mode=progressive,framerate=$FPS/1 ! timeoverlay halignment=left valignment=bottom shaded-background=true ! rtpvrawpay mtu=65000 chunks-per-frame=1 ! queue ! udpsink host=$TARGET_IP buffer-size=100000000 port=$STREAM_PORT sync=false async=false)

RES=$($GSTD_CLIENT_BIN --address=$GSTD_IP --port=5000 pipeline_play p1)
