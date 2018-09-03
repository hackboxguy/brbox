#!/bin/sh

#./operation.sh -i 10.128.64.164 -p reboot 
PORT=80
IPADDR="127.0.0.1"
#FILEPATH="brbox-operation.jpg"
USAGE1="usage:$0 -i <brbox_ipaddr> -p <on/idle/reboot>"
#return types
SUCCESS=0
FAIL=1
FINALRES=0

#non-standard return values (shifted by 200)
ERR_FILE_OPEN=201
ERR_FILE_READ=202
ERR_FILE_WRITE=203
ERR_ARGC=204
ERR_ARGV=205
ERR_HOST=206
ERR_SAME=207
ERR_DIFF=208
ERR_GENERIC=242
PATTERN="none"
export http_proxy= #by default do not use any http proxy
while getopts i:p: f
do
    case $f in
	i) IPADDR=$OPTARG ;;
	p) PATTERN=$OPTARG ;;
    esac
done

if [ $# -lt 2  ]; then 
	#echo $USAGE
	echo $USAGE1
	exit 1
fi

ping $IPADDR -c 1 -i 0.2 -w 5 >/dev/null
if [ $? != 0 ]; then
	echo "Error: BrBox $IPADDR not reachable!!!"
	exit $ERR_ARGC
fi

if [ $PATTERN = "none" ]; then
	JSON_CMDSTR="{ \"jsonrpc\": \"2.0\", \"method\": \"get_device_operation\", \"params\": { }, \"id\": 1 }"
	RESULT=$(curl -s --data-binary "$JSON_CMDSTR" -H 'content-type: application/json;' http://$IPADDR:$PORT/rpcapi/40001)
	RES=$(echo $RESULT | grep Success)
	[ $? != "0" ] && FINALRES=$FAIL
	RES=$(echo $RESULT | grep -o "operation.*" | awk '{print $2}' | sed -r 's/[""]//g')
	RET_VAL=": result=$RES"
else
	JSON_CMDSTR="{ \"jsonrpc\": \"2.0\", \"method\": \"set_device_operation\", \"params\": { \"operation\": \"$PATTERN\" }, \"id\": 1 }"
	RESULT=$(curl -s --data-binary "$JSON_CMDSTR" -H 'content-type: application/json;' http://$IPADDR:$PORT/rpcapi/40001)
	RES=$(echo $RESULT | grep Success)
	[ $? != "0" ] && FINALRES=$FAIL
	RET_VAL=""
fi

if [ "$FINALRES" = 0 ]; then
	echo "return=Success $RET_VAL"
else
	echo "return=Fail"
fi

exit $FINALRES

