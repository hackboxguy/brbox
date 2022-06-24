cmake_minimum_required(VERSION 2.8.9)
set(PROJECT_NAME BrBoxUtils)
project(${PROJECT_NAME})
set(BRBOX_MIPS_UPDATE_UTILS    OFF CACHE BOOL OFF)
if(${BRBOX_MIPS_UPDATE_UTILS})
add_subdirectory(brbox-mkimage)
add_subdirectory(mips-update)
endif()
#add_subdirectory(brbox-mkimage)
#add_subdirectory(hello_video)


#for cross compiling
#cmake -H. -BOutput -DCMAKE_TOOLCHAIN_FILE=/path/to/toolchain-arm-backup.cmake -DCMAKE_INSTALL_PREFIX=/home/user/tmp/
#cmake --build Output -- all
#cmake --build Output -- install

#for local compiling
#cmake -H. -BOutput
#cmake --build Output -- all

#for cleaning make generated output
#rm -rf Output
