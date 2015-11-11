#this cmake is only used for installing installing scripts to target)
cmake_minimum_required(VERSION 2.8.9)
#set(COMPONENT_NAME baytrail-upd)    #set the name of the binary
#set(BRDISKNAME brdskmgr)           #generic script name for all h/w
#set(UPDATENAME update)             #generic script name for all h/w
#set(BRDS_SCR baytrail-brdskmgr)  #script to be copied to target
set(USBMODE_UDEVRULE 41-usb_modeswitch.rules)    #/etc/fstab file

#project(${COMPONENT_NAME})
#file(GLOB SOURCES "*.c")

#dummy include and build( we are only interested in installing scripts to target)
#include_directories(${CMAKE_CURRENT_BINARY_DIR})
#add_executable(${COMPONENT_NAME} ${SOURCES})

file(GLOB  UDEV_RULE ${USBMODE_UDEVRULE})
install(PROGRAMS ${UDEV_RULE} DESTINATION etc/udev/rules.d)

#if(CMAKE_HOST_UNIX)
#install(CODE "EXECUTE_PROCESS(WORKING_DIRECTORY \$ENV{TARGET_DIR}\${CMAKE_INSTALL_PREFIX}/sbin COMMAND ln -sf ${BRDS_SCR} ${BRDISKNAME})")
#install(CODE "EXECUTE_PROCESS(WORKING_DIRECTORY \$ENV{TARGET_DIR}\${CMAKE_INSTALL_PREFIX}/sbin COMMAND ln -sf ${UPDT_SCR} ${UPDATENAME})")
#endif(CMAKE_HOST_UNIX)
#install(PROGRAMS ${FSTABFILE} DESTINATION ../etc/init.d/) #copy to /target/usr/../etc/init.d : to be fixed in package file
#MESSAGE( STATUS "OUTPUT_INSTALL_DIR: " ${CMAKE_INSTALL_PREFIX}/sbin )
#DESTDIR

#for cross compiling
#cmake -H. -BOutput -DCMAKE_TOOLCHAIN_FILE=/path/to/toolchain-arm-backup.cmake -DCMAKE_INSTALL_PREFIX=/home/user/tmp/
#cmake --build Output -- all
#cmake --build Output -- install

#for local compiling
#cmake -H. -BOutput
#cmake --build Output -- all

#for cleaning make generated output
#rm -rf Output

