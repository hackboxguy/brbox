#!/bin/sh
#brbox signed update image creation tool
###############################################################################
#./create-signed-brbox-proj.sh
#	--type=BrBoxGl300N
#	--version=00.01.0084
#	--infile=/tmp/gl-mt300nv2-remote-kit.bin
#	--private=/tmp/private_key.pem
#	--outfile=/tmp/mipsProject.uimg
#	--mkimg=/usr/bin/brbox-mkimage.sh
#	--mkuimg=/usr/bin/brbox-mkuimg.sh
USAGE="usage: $0 --type=BrBoxGl300N --version=00.01.1234 --infile=/tmp/img.bin --private=priv.pem --outfile=out.uimg --mkimage=/sbin/mkimage --mkuimg=/sbin/mkuimg.sh"
IMGTYPE="none"
VERSION="none"
PRIVATEKEY="none"
INFILE="none"
OUTFILE="none"
MKIMAGE="none"
MKUIMG="none"
NOARGS="yes"
##############parse the arguments##############################################
optspec=":h-:"
while getopts "$optspec" optchar; do
    case "${optchar}" in
        -)
            case "${OPTARG}" in
                    type=*) #image-type [BrBoxGl300N]
                        val=${OPTARG#*=}
                        opt=${OPTARG%=$val}
                        #IMGTYPE=${val}
                        [ ! -z $IMGTYPE ] && IMGTYPE=${val} #IMGTYPE="BrBoxGl300N"
                        NOARGS="no"
                        ;;
                    version=*) #image-version
                        val=${OPTARG#*=}
                        opt=${OPTARG%=$val}
                        #VERSION=${val}
                        [ ! -z $VERSION ] && VERSION=${val} #VERSION="00.01.0000"
                        NOARGS="no"
                        ;;
                    private=*) #private-key
                        val=${OPTARG#*=}
                        opt=${OPTARG%=$val}
                        #PRIVATEKEY=${val}
                        [ ! -z $PRIVATEKEY ] && PRIVATEKEY=${val} #PRIVATEKEY="none"
                        NOARGS="no"
                        ;;
                    infile=*) #input-binary-file
                        val=${OPTARG#*=}
                        opt=${OPTARG%=$val}
                        #INFILE=${val}
                        [ ! -z $INFILE ] && INFILE=${val} #INFILE="none"
                        NOARGS="no"
                        ;;
                    outfile=*) #output-uimage-file
                        val=${OPTARG#*=}
                        opt=${OPTARG%=$val}
                        #OUTFILE=${val}
                        [ ! -z $OUTFILE ] && OUTFILE=${val} #OUTFILE="none"
                        NOARGS="no"
                        ;;
                    mkimage=*) #mkimage-binary-path
                        val=${OPTARG#*=}
                        opt=${OPTARG%=$val}
                        MKIMAGE=${val}
                        [ ! -z $MKIMAGE ] && MKIMAGE=${val} #MKIMAGE="none"
                        NOARGS="no"
                        ;;
                    mkuimg=*) #mkuimg-script
                        val=${OPTARG#*=}
                        opt=${OPTARG%=$val}
                        #MKUIMG=${val}
                        [ ! -z $MKUIMG ] && MKUIMG=${val} #MKUIMG="none"
                        NOARGS="no"
                        ;;
                *)
                    if [ "$OPTERR" = 1 ] && [ "${optspec:0:1}" != ":" ]; then
                        echo "Unknown option:"
                        echo -n "${USAGE}";echo ""
                        exit 1
                    fi
                    ;;
            esac;;
        h)
                echo -n "${USAGE}";echo ""
                exit 0
            ;;
        *)
                echo "Unknown option:"
                echo -n "${USAGE}";echo ""
                exit 1
            ;;
    esac
done

if [ ${NOARGS} = "yes" ] ; then
    echo -n "${USAGE}";echo ""
    exit 0
fi

#echo "IMGTYPE=$IMGTYPE"
#echo "VERSION=$VERSION"
#echo "PRIVATEKEY=$PRIVATEKEY"
#echo "INFILE=$INFILE"
#echo "OUTFILE=$OUTFILE"
#echo "MKIMAGE=$MKIMAGE"
#echo "MKUIMG=$MKUIMG"
[ $IMGTYPE = "none" ] && { echo "missing --type=<value> !!!" ; return 1;}
[ $VERSION = "none" ] && { echo "missing --version=<value> !!!" ; return 1;}
[ $PRIVATEKEY = "none" ] && { echo "missing --private=<filepath> !!!" ; return 1;}
[ $INFILE = "none" ] && { echo "missing --infile=<filepath> !!!" ; return 1;}
[ $OUTFILE = "none" ] && { echo "missing --outfile=<filepath> !!!" ; return 1;}
[ $MKIMAGE = "none" ] && { echo "missing --mkimage=<binary-file-path> !!!" ; return 1;}
[ $MKUIMG = "none" ] && { echo "missing --mkuimg=<script-path> !!!" ; return 1;}

[ ! -f  "$PRIVATEKEY"  ] && { echo "Error: --private=key-file not found!!!"; return 1; }
[ ! -f  "$INFILE"  ] && { echo "Error: --infile=file not found!!!"; return 1; }
[ ! -f  "$MKIMAGE"  ] && { echo "Error: --mkimage=utility not found!!!"; return 1; }
[ ! -f  "$MKUIMG"  ] && { echo "Error: --mkuimg=script not found!!!"; return 1; }

if [ $IMGTYPE != "BrBoxGl300N" ]; then
	echo "unknown --type=<value> !!!"
	return 1
fi

#sign a file with priv_key
SIGNATURE_FILE=$(mktemp)
openssl dgst -sign $PRIVATEKEY -keyform PEM -sha256 -out $SIGNATURE_FILE -binary $INFILE > /dev/null
if [ $? != "0" ]; then
	echo "Unable to create signature file!!!"
	rm -rf SIGNATURE_FILE
	return 1
fi

SIGNATURE_UIMAGE=$(mktemp)
#brbox-mkuimg.sh -r /tmp/gl-mt300nv2-remote-kit.sign -t BrBoxSign -v 00.01.0084 -o /tmp/gl-mt300nv2-remote-kit.sign.uimg -m /usr/sbin/brbox-mkimage
$MKUIMG -r $SIGNATURE_FILE -t BrBoxSign -v $VERSION -o $SIGNATURE_UIMAGE -m $MKIMAGE 1>/dev/null 2>/dev/null

if [ $? != "0" ]; then
	echo "Unable to create signature.uimg file!!!"
	rm -rf SIGNATURE_FILE
	rm -rf SIGNATURE_UIMAGE
	return 1
fi

FMW_UIMAGE=$(mktemp)
#/tmp/brbox-mkuimg.sh -r /tmp/gl-mt300nv2-remote-kit.bin -t BrBoxGl300N -v 00.01.0084 -o /tmp/gl-mt300nv2-remote-kit.bin.uimg -m /usr/sbin/brbox-mkimage
$MKUIMG -r $INFILE -t $IMGTYPE -v $VERSION -o $FMW_UIMAGE -m $MKIMAGE 1>/dev/null 2>/dev/null
if [ $? != "0" ]; then
	echo "Unable to create Firmware.uimg file!!!"
	rm -rf SIGNATURE_FILE
	rm -rf SIGNATURE_UIMAGE
	rm -rf FMW_UIMAGE
	return 1
fi

#/tmp/brbox-mkuimg.sh -r /tmp/gl-mt300nv2-remote-kit.sign.uimg:/tmp/gl-mt300nv2-remote-kit.bin.uimg -t BrBoxProj -v 00.01.0084 -o /tmp/mipsProject.uimg -m /usr/sbin/brbox-mkimage
$MKUIMG -r "$FMW_UIMAGE:$SIGNATURE_UIMAGE" -t BrBoxProj -v $VERSION -o $OUTFILE -m $MKIMAGE 1>/dev/null 2>/dev/null
if [ $? != "0" ]; then
	echo "Unable to create final project.uimg file!!!"
	rm -rf SIGNATURE_FILE
	rm -rf SIGNATURE_UIMAGE
	rm -rf FMW_UIMAGE
	return 1
fi

rm -rf SIGNATURE_FILE
rm -rf SIGNATURE_UIMAGE
rm -rf FMW_UIMAGE

echo "Image creation successfull :-)"
return 0
