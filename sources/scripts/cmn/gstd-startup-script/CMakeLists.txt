#this cmake is only used for installing scripts to target)
cmake_minimum_required(VERSION 2.8.9)
set(GSTD_STARTUP S26StartGstd)

file(GLOB  STARTUPFILE ${GSTD_STARTUP})
file(GLOB  GSTDSCRIPTS "*.sh")
install(PROGRAMS ${STARTUPFILE} DESTINATION etc/init.d)
install(PROGRAMS ${GSTDSCRIPTS} DESTINATION usr/bin)


#for cross compiling
#cmake -H. -BOutput -DCMAKE_TOOLCHAIN_FILE=/path/to/toolchain-arm-backup.cmake -DCMAKE_INSTALL_PREFIX=/home/user/tmp/
#cmake --build Output -- all
#cmake --build Output -- install

#for local compiling
#cmake -H. -BOutput
#cmake --build Output -- all

#for cleaning make generated output
#rm -rf Output

