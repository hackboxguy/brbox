#!/bin/bash
#./capture-and-stream-unicast.sh -g <gstd_ip> -i <target_ip> -p <stream_port> -d <video_device_index>
USAGE="./capture-and-stream-unicast.sh -g <gstd_ip> -i <target_ip> -p <stream_port> -d <video_device_index>"
VIDEO_DEV=/dev/video0
TARGET_IP=127.0.0.1
STREAM_PORT=6000
GSTD_IP=127.0.0.1  #gstreamer-daemon ip
GSTD_CLIENT_BIN=gstd-client

while getopts i:p:g:d: f
do
    case $f in
	i) TARGET_IP=$OPTARG ;;
	p) STREAM_PORT=$OPTARG ;;
	d) VIDEO_DEV=$OPTARG ;;
	g) GSTD_IP=$OPTARG ;;
    esac
done

if [ $# -lt 2  ]; then 
	echo $USAGE
	exit 1
fi

#sysctl -w net.core.rmem_max=98214400
RES=$($GSTD_CLIENT_BIN --address=$GSTD_IP --port=5000 pipeline_delete p1)

RES=$($GSTD_CLIENT_BIN --address=$GSTD_IP --port=5000 create /pipelines p1 v4l2src device=$VIDEO_DEV ! videoconvert ! rtpvrawpay mtu=65000 chunks-per-frame=1 ! queue ! udpsink host=$TARGET_IP buffer-size=100000000 port=$STREAM_PORT sync=false async=false)

RES=$($GSTD_CLIENT_BIN --address=$GSTD_IP --port=5000 pipeline_play p1)
