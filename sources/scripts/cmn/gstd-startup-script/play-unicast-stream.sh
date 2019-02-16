#!/bin/bash
#./play-unicast-stream.sh -g <gstd_ip> -p <stream_port>
USAGE="./play-unicast-stream.sh -g <gstd_ip> -p <stream_port>"
STREAM_PORT=6000
GSTD_IP=127.0.0.1  #gstreamer-daemon ip
GSTD_CLIENT_BIN=gstd-client
#caps for aver-media-BU110-hdmi-capture and stream-over rtp
CAPS="caps=application/x-rtp,media=(string)video,clock-rate=(int)90000,\
encoding-name=(string)RAW,sampling=(string)YCbCr-4:2:2,depth=(string)8,\
width=(string)1920,height=(string)1080,colorimetry=(string)BT709-2,\
payload=(int)96,ssrc=(uint)2748897661,timestamp-offset=(uint)619409570,\
seqnum-offset=(uint)22814,a-framerate=(string)60 buffer-size=1000000000"

while getopts p:g: f
do
    case $f in
	p) STREAM_PORT=$OPTARG ;;
	g) GSTD_IP=$OPTARG ;;
    esac
done

if [ $# -lt 2  ]; then 
	echo $USAGE
	exit 1
fi

RES=$($GSTD_CLIENT_BIN --address=$GSTD_IP --port=5000 pipeline_delete p1)

RES=$($GSTD_CLIENT_BIN --address=$GSTD_IP --port=5000 create /pipelines p1 udpsrc port=$STREAM_PORT $CAPS ! queue ! rtpvrawdepay ! queue ! videoconvert ! xvimagesink sync=false async=false)

RES=$($GSTD_CLIENT_BIN --address=$GSTD_IP --port=5000 pipeline_play p1)


