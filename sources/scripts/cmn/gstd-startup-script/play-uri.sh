#!/bin/sh
#./play-uri.sh -g <gstd_ip> -u <stream_port> -s <videosink>
USAGE="./play-uri.sh -g <gstd_ip> -u <stream_port> -s <videosink>"
URI=""
VIDEOSINK="xvimagesink"
GSTD_IP=127.0.0.1  #gstreamer-daemon ip
GSTD_CLIENT_BIN=gst-client-1.0


while getopts u:g:s: f
do
    case $f in
	u) URI=$OPTARG ;;
	g) GSTD_IP=$OPTARG ;;
	s) VIDEOSINK=$OPTARG ;;
    esac
done

if [ $# -lt 2  ]; then 
	echo $USAGE
	exit 1
fi

RES=$($GSTD_CLIENT_BIN --tcp-address=$GSTD_IP --tcp-port=5000 pipeline_delete p1)

RES=$($GSTD_CLIENT_BIN --tcp-address=$GSTD_IP --tcp-port=5000 create /pipelines p1 playbin3 uri="$URI" video-sink=$VIDEOSINK)

RES=$($GSTD_CLIENT_BIN --tcp-address=$GSTD_IP --tcp-port=5000 pipeline_play p1)

