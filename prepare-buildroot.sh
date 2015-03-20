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
popd
####################check if download folder exists########
[ ! -d  "$BR_DOWNLOAD_FOLDER/"  ] && echo "Warning: BR-download folder $BR_DOWNLOAD_FOLDER not found, will take longer time to build the disk!"  
echo "Success: Buildroot preperation done!!"
