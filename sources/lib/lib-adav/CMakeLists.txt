cmake_minimum_required(VERSION 2.8.9)

#set the name of the library as libAdCmn
set(ADCMN_LIBNAME AdCmn)

#set the version of the library here
set(ADCMN_LIBVER_MAJOR 0)
set(ADCMN_LIBVER_MINOR 1)
set(ADCMN_LIBVER_PATCH 1)
set(ADCMN_LIBVER_STRING ${ADCMN_LIBVER_MAJOR}.${ADCMN_LIBVER_MINOR}.${ADCMN_LIBVER_PATCH})

project(${ADCMN_LIBNAME})
set(CMAKE_BUILD_TYPE Release)

SET(OUTBIN "./" CACHE STRING "./") #this value can be passed via cmdline as "cmake . -DOUTBIN=/tmp/"
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTBIN}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTBIN}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTBIN}/lib)


#Bring the headers into project
include_directories(include)
 
#GLOB allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")


#Generate the shared library from the sources
add_library(${ADCMN_LIBNAME} SHARED ${SOURCES})
 
#Set the location for library installation -- i.e., /usr/lib in this case
# Use "sudo make install" to apply
install(TARGETS ${ADCMN_LIBNAME} DESTINATION /usr/lib)

set_target_properties(${ADCMN_LIBNAME} PROPERTIES VERSION ${ADCMN_LIBVER_STRING}
                                           SOVERSION ${ADCMN_LIBVER_MAJOR})

#cmake -H. -BOutput or #cmake -H. -BOutput -DOUTBIN=/mnt/buildramdisk/lib-output/
#cmake --build Output -- all
#cmake --build Output -- clean
#rm -rf Output
