cmake_minimum_required(VERSION 2.8.9)
set(PROJECT_NAME AdLibraries)
project(${PROJECT_NAME})
add_subdirectory(lib-display)
add_subdirectory(lib-settings)
add_subdirectory(lib-adav-old)

#include(ExternalProject)
#ExternalProject_Add(libstrophe
#    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libstrophe
#    UPDATE_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/libstrophe/bootstrap.sh
#    CONFIGURE_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/libstrophe/configure --prefix=${CMAKE_INSTALL_PREFIX}
#    BUILD_COMMAND ${MAKE})

#add_subdirectory(lib-onion)
#add_subdirectory(lib-ilclient)
#for cross compiling
#cmake -H. -BOutput -DCMAKE_TOOLCHAIN_FILE=toolchain-arm-backup.cmake
#cmake --build Output -- all

#for local compiling
#cmake -H. -BOutput
#cmake --build Output -- all

#for cleaning make generated output
#rm -rf Output

#specifying generated output binary folder
#cmake -H. -BOutput or #cmake -H. -BOutput -DOUTBIN=/mnt/buildramdisk/lib-output/

