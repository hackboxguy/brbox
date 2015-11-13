#this cmake is only used for installing installing scripts to target)
cmake_minimum_required(VERSION 2.8.9)
#set(COMPONENT_NAME baytrail-upd)    #set the name of the binary
#set(BRDISKNAME brdskmgr)           #generic script name for all h/w
#set(UPDATENAME update)             #generic script name for all h/w
#set(BRDS_SCR baytrail-brdskmgr)  #script to be copied to target
set(FINDDEV_SCR find-devtype)    #script to be copied to target
set(FINDDEV_STARTUP S01FindDevType)    #/etc/fstab file
set(SRC_HOSTNAME hostname)          #server startup script
set(TARGT_HOSTNAME /mnt/settings/etc/hostname)          #server startup script

#project(${COMPONENT_NAME})
#file(GLOB SOURCES "*.c")

#dummy include and build( we are only interested in installing scripts to target)
#include_directories(${CMAKE_CURRENT_BINARY_DIR})
#add_executable(${COMPONENT_NAME} ${SOURCES})

file(GLOB  SCRIPTFILES ${FINDDEV_SCR})
file(GLOB  STARTUPFILES ${FINDDEV_STARTUP})
install(PROGRAMS ${SCRIPTFILES} DESTINATION usr/sbin)
install(PROGRAMS ${STARTUPFILES} DESTINATION etc/init.d)

if(CMAKE_HOST_UNIX)
install(CODE "EXECUTE_PROCESS(WORKING_DIRECTORY \$ENV{TARGET_DIR}\${CMAKE_INSTALL_PREFIX}/etc COMMAND ln -sf ${TARGT_HOSTNAME} ${SRC_HOSTNAME})")
endif(CMAKE_HOST_UNIX)

#for cross compiling
#cmake -H. -BOutput -DCMAKE_TOOLCHAIN_FILE=/path/to/toolchain-arm-backup.cmake -DCMAKE_INSTALL_PREFIX=/home/user/tmp/
#cmake --build Output -- all
#cmake --build Output -- install

#for local compiling
#cmake -H. -BOutput
#cmake --build Output -- all

#for cleaning make generated output
#rm -rf Output

