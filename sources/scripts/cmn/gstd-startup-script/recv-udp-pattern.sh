#!/bin/sh
#./recv-udp-pattern.sh -g <gstd_ip> -p <stream_port> -w <pattern-width> -h <pattern-height> -f <pattern-fps>
USAGE="./recv-udp-pattern.sh -g <gstd_ip> -p <stream_port> -w <pattern-width> -h <pattern-height> -f <pattern-fps>"
#TARGET_IP=127.0.0.1
STREAM_PORT=6000
GSTD_IP=127.0.0.1  #gstreamer-daemon ip
GSTD_CLIENT_BIN=gst-client-1.0
WIDTH=1920
HEIGHT=1080
#FPS=30

while getopts p:g:w:h:f: m
do
    case $m in
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
RES=$($GSTD_CLIENT_BIN --tcp-address=$GSTD_IP --tcp-port=5000 pipeline_delete p1)

RES=$($GSTD_CLIENT_BIN --tcp-address=$GSTD_IP --tcp-port=5000  create /pipelines p1 udpsrc port=$STREAM_PORT caps=application/x-rtp,clock-rate=90000,encoding-name=RAW,sampling="(string)BGRA",width="(string)$WIDTH",height="(string)$HEIGHT" buffer-size=1000000000 ! queue ! rtpvrawdepay ! videoconvert ! xvimagesink)

RES=$($GSTD_CLIENT_BIN --tcp-address=$GSTD_IP --tcp-port=5000 pipeline_play p1)
