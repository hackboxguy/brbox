#!/bin/sh
# Run two video webcam feeds in two different windows
# Script assumes that:
# Microsoft LifeCam Studio is video0
# Logitech c920 is video1
# Both cameras run max 1920x1080 MJPG, but two have both on a USB bus they can run @ 1280x720 max
# MS is made a little smaller
# text overlay and clockoverlay may be added to the pipeline, but render poorly
VELEM="v4l2src device=/dev/video0 do-timestamp=true"
VCAPS="image/jpeg, width=640, height=480, framerate=30/1"
VSOURCE="$VELEM ! $VCAPS"
VIDEO_SINK="videoconvert ! videoscale ! xvimagesink sync=false"
VIDEO_DECODE="jpegparse ! jpegdec"

VELEM1="v4l2src device=/dev/video1 do-timestamp=true"
VCAPS1="image/jpeg, width=1280, height=720, framerate=30/1"
VSOURCE1="$VELEM1 ! $VCAPS1"

TEXT_OVERLAY_MS="textoverlay text=\"MS LifeCam Studio\" shaded-background=true"
TEXT_OVERLAY_LT="textoverlay text=Logitech c920"

CLOCK_OVERLAY="clockoverlay shaded-background=true valignment=bottom time-format=\"%Y/%m/%d %H:%M:%S\""

#echo is just for debugging purposes
echo gst-launch-1.0 -vvv \
	tee name=splitter \
	$VSOURCE \
   	! $VIDEO_DECODE \
   	! $VIDEO_SINK splitter. \
	$VSOURCE1 \
   	! $VIDEO_DECODE \
   	! $VIDEO_SINK splitter.

gst-launch-1.0 -vvv \
	tee name=splitter \
	$VSOURCE \
   	! $VIDEO_DECODE \
   	! $VIDEO_SINK splitter. \
	$VSOURCE1 \
   	! $VIDEO_DECODE \
   	! $VIDEO_SINK splitter.
