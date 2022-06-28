cmake_minimum_required(VERSION 2.8.9)
set(PROJECT_NAME BrBoxComponents)
project(${PROJECT_NAME})
option(SDSRV_USE_MUSL "link with musl c library" OFF)
if(${SDSRV_USE_MUSL})
add_definitions(-DSDSRV_USE_MUSL_LIB)
endif()
add_subdirectory(lib)
add_subdirectory(services)
add_subdirectory(utils)
#add_subdirectory(kmods)

#for cross compiling
#cmake -H. -BOutput -DCMAKE_TOOLCHAIN_FILE=/path/to/toolchain-arm-backup.cmake -DCMAKE_INSTALL_PREFIX=/home/user/tmp/
#cmake --build Output -- all
#cmake --build Output -- install -j$(nproc)

#for local compiling
#cmake -H. -BOutput -DCMAKE_INSTALL_PREFIX=/home/adav/tmp/brbox -DAUTO_SVN_VERSION=OFF -DINSTALL_CLIENT=ON
#cmake -H. -BOutput
#cmake --build Output -- all -j$(nproc)

#for cleaning make generated output
#rm -rf Output

