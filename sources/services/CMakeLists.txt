cmake_minimum_required(VERSION 2.8.9)
set(PROJECT_NAME BrBoxServices)
project(${PROJECT_NAME})
set(MINIMUM_BRBOX_SYSTEM    OFF CACHE BOOL OFF)
if(${MINIMUM_BRBOX_SYSTEM})
add_subdirectory(sysmgr)
add_subdirectory(dispsrv)
add_subdirectory(xmproxy)
else()
add_subdirectory(sysmgr)
#add_subdirectory(smarteye)
add_subdirectory(gpioctl)
add_subdirectory(hotwire)
#add_subdirectory(bboxsms)
add_subdirectory(xmproxy)
add_subdirectory(apisrv)
add_subdirectory(i2csrv)
add_subdirectory(dispsrv)
#add_subdirectory(lightsense)
add_subdirectory(modbussrv)
add_subdirectory(monitsrv)
endif()

#for cross compiling
#cmake -H. -BOutput -DCMAKE_TOOLCHAIN_FILE=/path/to/toolchain-arm-backup.cmake -DCMAKE_INSTALL_PREFIX=/home/user/tmp/
#cmake --build Output -- all
#cmake --build Output -- install

#for local compiling
#cmake -H. -BOutput -DAUTO_SVN_VERSION=OFF
#cmake --build Output -- all -j$(nproc)

#for cleaning make generated output
#rm -rf Output

