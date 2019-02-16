#!/bin/bash
#./play-multicast-stream.sh -g <gstd_ip> -p <stream_port> -i <multicast_ip>
USAGE="./play-multicast-stream.sh -g <gstd_ip> -p <stream_port> -i <multicast_ip>"
STREAM_PORT=5004
MULTICAST_IP=239.255.42.42
GSTD_IP=127.0.0.1  #gstreamer-daemon ip
GSTD_CLIENT_BIN=gstd-client
MULTICAST_IFACE=eth0
#caps for lenkeng-hdmi-capture and stream as rtp-x264 compressed-stream
CAPS="caps=video/mpegts,media=(string)video"

while getopts p:g:i: f
do
    case $f in
	p) STREAM_PORT=$OPTARG ;;
	g) GSTD_IP=$OPTARG ;;
	i) MULTICAST_IP=$OPTARG ;;
    esac
done

if [ $# -lt 2  ]; then 
	echo $USAGE
	exit 1
fi

RES=$($GSTD_CLIENT_BIN --address=$GSTD_IP --port=5000 pipeline_delete p1)

RES=$($GSTD_CLIENT_BIN --address=$GSTD_IP --port=5000 create /pipelines p1 udpsrc port=$STREAM_PORT multicast-iface=$MULTICAST_IFACE multicast-group=$MULTICAST_IP auto-multicast=true $CAPS ! queue ! tsdemux ! queue ! decodebin ! queue ! videoconvert ! queue ! xvimagesink sync=false async=false)

RES=$($GSTD_CLIENT_BIN --address=$GSTD_IP --port=5000 pipeline_play p1)

