#this cmake is only used for installing scripts to target)
cmake_minimum_required(VERSION 2.8.9)
#set(MOUNTFILE S02MountSettings)    #/etc/init.d/S02MountSettings file
set(MOUNTFILE fstab)    #/etc/init.d/S02MountSettings file

#file(GLOB  INSTALLFILES ${BRDS_SCR} ${UPDT_SCR})
#install(PROGRAMS ${INSTALLFILES} DESTINATION sbin)
#if(CMAKE_HOST_UNIX)
#install(CODE "EXECUTE_PROCESS(WORKING_DIRECTORY \$ENV{TARGET_DIR}\${CMAKE_INSTALL_PREFIX}/sbin COMMAND ln -sf ${BRDS_SCR} ${BRDISKNAME})")
#install(CODE "EXECUTE_PROCESS(WORKING_DIRECTORY \$ENV{TARGET_DIR}\${CMAKE_INSTALL_PREFIX}/sbin COMMAND ln -sf ${UPDT_SCR} ${UPDATENAME})")
#endif(CMAKE_HOST_UNIX)

#install(PROGRAMS ${MOUNTFILE} DESTINATION init.d) #copy /etc/fstab file
install(PROGRAMS ${MOUNTFILE} DESTINATION etc) #copy /etc/fstab file

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

