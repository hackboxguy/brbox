#!/bin/bash
#this script prepares the buildroot and dependencies
BR_LAST_STABLE_VERSION=2015.02
BR_DOWNLOAD_FOLDER=dl
####################checkout buildroot#####################
git clone git://git.buildroot.net/buildroot
pushd .
cd buildroot
git checkout $BR_LAST_STABLE_VERSION
git pull . $BR_LAST_STABLE_VERSION

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
