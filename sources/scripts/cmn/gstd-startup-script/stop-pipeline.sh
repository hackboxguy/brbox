#!/bin/bash
#./stop-pipeline.sh -g <gstd_ip> -l <pipelineID>
USAGE="./stop-pipeline.sh -g <gstd_ip> -l <pipelineID>"
PIPELINE_ID=p1
GSTD_IP=127.0.0.1  #gstreamer-daemon ip
GSTD_CLIENT_BIN=gstd-client

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

RES=$($GSTD_CLIENT_BIN --address=$GSTD_IP --port=5000 pipeline_delete $PIPELINE_ID)

