#this cmake is only used for installing scripts to target)
cmake_minimum_required(VERSION 2.8.9)
set(PREPARE_BOOT_DISK prepare-boot-disk.sh) 
set(PREPARE_INTEL_DISK prepare-boot-disk-intel-usb.sh) 
file(GLOB  BOOTDISK_SCRIPT ${PREPARE_BOOT_DISK} ${PREPARE_INTEL_DISK})
install(PROGRAMS ${BOOTDISK_SCRIPT} DESTINATION usr/bin/)

