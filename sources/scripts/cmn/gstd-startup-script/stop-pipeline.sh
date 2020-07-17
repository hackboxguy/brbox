#!/bin/sh
#./stop-pipeline.sh -g <gstd_ip> -l <pipelineID>
USAGE="./stop-pipeline.sh -g <gstd_ip> -l <pipelineID>"
PIPELINE_ID=p1
GSTD_IP=127.0.0.1  #gstreamer-daemon ip
GSTD_CLIENT_BIN=gst-client-1.0

while getopts g:l: f
do
    case $f in
	l) PIPELINE_ID=$OPTARG ;;
	g) GSTD_IP=$OPTARG ;;
    esac
done

if [ $# -lt 2  ]; then 
	echo $USAGE
	exit 1
fi

RES=$($GSTD_CLIENT_BIN --tcp-address=$GSTD_IP --tcp-port=5000 pipeline_delete $PIPELINE_ID)

