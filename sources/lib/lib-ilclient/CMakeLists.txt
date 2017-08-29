cmake_minimum_required(VERSION 2.8.9)

#set the name of the library here
set(LIBILCLT_LIBNAME AdIlClt)

#set the version of the library here
set(LIBILCLT_LIBVER_MAJOR 0)
set(LIBILCLT_LIBVER_MINOR 1)
set(LIBILCLT_LIBVER_PATCH 1)
set(LIBILCLT_LIBVER_STRING ${LIBILCLT_LIBVER_MAJOR}.${LIBILCLT_LIBVER_MINOR}.${LIBILCLT_LIBVER_PATCH})

#set(CMAKE_INSTALL_PREFIX /home/adav/usr/lib)
project(${LIBILCLT_LIBNAME})
set(CMAKE_BUILD_TYPE Release)



#Bring the headers into project
include_directories(include)
 
#GLOB allows for wildcard additions:
file(GLOB SOURCES "src/*.c")

SET(CMAKE_C_FLAGS "-DSTANDALONE -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -DTARGET_POSIX -D_LINUX -fPIC -DPIC -D_REENTRANT -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -U_FORTIFY_SOURCE -Wall -g -DHAVE_LIBOPENMAX=2 -DOMX -DOMX_SKIP64BIT -ftree-vectorize -pipe -DUSE_EXTERNAL_OMX -DHAVE_LIBBCM_HOST -DUSE_EXTERNAL_LIBBCM_HOST -DUSE_VCHIQ_ARM -Wno-psabi ${CMAKE_C_FLAGS}")
SET(CMAKE_CXX_FLAGS "-DSTANDALONE -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -DTARGET_POSIX -D_LINUX -fPIC -DPIC -D_REENTRANT -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -U_FORTIFY_SOURCE -Wall -g -DHAVE_LIBOPENMAX=2 -DOMX -DOMX_SKIP64BIT -ftree-vectorize -pipe -DUSE_EXTERNAL_OMX -DHAVE_LIBBCM_HOST -DUSE_EXTERNAL_LIBBCM_HOST -DUSE_VCHIQ_ARM -Wno-psabi ${CMAKE_CXX_FLAGS}")

#Generate the shared library from the sources
add_library(${LIBILCLT_LIBNAME} SHARED ${SOURCES})
 
#Set the location for library installation -- i.e., /usr/local/lib by default or override with CMAKE_INSTALL_PREFIX
# Use "sudo make install" to apply
file(GLOB  INCFILES "include/*.h")
install(TARGETS ${LIBILCLT_LIBNAME} DESTINATION lib)
install(FILES ${INCFILES} DESTINATION include)

set_target_properties(${LIBILCLT_LIBNAME} PROPERTIES VERSION ${LIBILCLT_LIBVER_STRING}
                                           SOVERSION ${LIBILCLT_LIBVER_MAJOR})

#for cross compiling
#cmake -H. -BOutput -DCMAKE_TOOLCHAIN_FILE=/path/to/toolchain-arm-backup.cmake -DCMAKE_INSTALL_PREFIX=/home/adav/tmp/
#cmake --build Output -- all
#cmake --build Output -- install

#for local compiling
#cmake -H. -BOutput
#cmake --build Output -- all

#for cleaning make generated output
#rm -rf Output

