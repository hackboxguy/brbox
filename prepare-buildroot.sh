#!/bin/bash
#this script prepares the buildroot and dependencies
BR_LAST_STABLE_VERSION=2016.08-rc2 #2016.02 #2016.05-rc3 #2016.02 #2015.11.1 #2015.08.1 #2015.05 #2015.02
BR_DOWNLOAD_FOLDER=dl

while getopts v: f
do
    case $f in
	v) BR_LAST_STABLE_VERSION=$OPTARG ;; #override incase if this script is called with -v version
    esac
done

####################checkout buildroot#####################
git clone git://git.buildroot.net/buildroot
[ $? != 0 ] && echo "git clone failed!!!" && exit -1
pushd .
cd buildroot
git checkout $BR_LAST_STABLE_VERSION
[ $? != 0 ] && echo "git checkout failed!!!" && exit -1
git pull . $BR_LAST_STABLE_VERSION
[ $? != 0 ] && echo "git pull failed!!!" && exit -1

#add brbox package menu to Config.in of buildroot/package
#and create a package folder softlink of brbox
head --lines=-1 package/Config.in > package/Config.in.new
echo "source \"package/brbox/Config.in\"" >> package/Config.in.new
echo "endmenu" >> package/Config.in.new
rm package/Config.in
mv package/Config.in.new package/Config.in
ln -s ../../packages/ package/brbox
popd
####################check if download folder exists########
[ ! -d  "$BR_DOWNLOAD_FOLDER/"  ] && echo "Warning: BR-download folder $BR_DOWNLOAD_FOLDER not found, will take longer time to build the disk!"  
echo "Success: Buildroot preperation done!!"
