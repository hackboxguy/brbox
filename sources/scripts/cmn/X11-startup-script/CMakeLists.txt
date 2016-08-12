#this cmake is only used for installing installing scripts to target)
cmake_minimum_required(VERSION 2.8.9)
set(X11_STARTUP S25StartX11)    #/etc/fstab file
set(X11_VARS vars.sh)    
set(X11_INITFUNC init-functions)    

file(GLOB  STARTUPFILE ${X11_STARTUP})
file(GLOB  VARSFILE ${X11_VARS})
file(GLOB  INIT_FUNCFILE ${X11_INITFUNC})

install(PROGRAMS ${STARTUPFILE} DESTINATION etc/init.d)
install(PROGRAMS ${VARSFILE} DESTINATION lib/init)
install(PROGRAMS ${INIT_FUNCFILE} DESTINATION lib/lsb)

#for cross compiling
#cmake -H. -BOutput -DCMAKE_TOOLCHAIN_FILE=/path/to/toolchain-arm-backup.cmake -DCMAKE_INSTALL_PREFIX=/home/user/tmp/
#cmake --build Output -- all
#cmake --build Output -- install

#for local compiling
#cmake -H. -BOutput
#cmake --build Output -- all

#for cleaning make generated output
#rm -rf Output

