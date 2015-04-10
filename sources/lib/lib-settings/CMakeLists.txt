cmake_minimum_required(VERSION 2.8.9)

#set the name of the library here
set(STTNGS_LIBNAME AdSttngs)

#set the version of the library here
set(STTNGS_LIBVER_MAJOR 0)
set(STTNGS_LIBVER_MINOR 1)
set(STTNGS_LIBVER_PATCH 1)
set(STTNGS_LIBVER_STRING ${STTNGS_LIBVER_MAJOR}.${STTNGS_LIBVER_MINOR}.${STTNGS_LIBVER_PATCH})

#set(CMAKE_INSTALL_PREFIX /home/adav/usr/lib)
project(${STTNGS_LIBNAME})
set(CMAKE_BUILD_TYPE Release)

#SET(OUTBIN "./" CACHE STRING "./") #this value can be passed via cmdline as "cmake . -DOUTBIN=/tmp/"
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTBIN}/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTBIN}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTBIN}/lib)


#Bring the headers into project
include_directories(include)
 
#GLOB allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")


#Generate the shared library from the sources
add_library(${STTNGS_LIBNAME} SHARED ${SOURCES})
 
#Set the location for library installation -- i.e., /usr/local/lib by default or override with CMAKE_INSTALL_PREFIX
# Use "sudo make install" to apply
file(GLOB  INCFILES "include/*.h")
install(TARGETS ${STTNGS_LIBNAME} DESTINATION lib)
install(FILES ${INCFILES} DESTINATION include)

set_target_properties(${STTNGS_LIBNAME} PROPERTIES VERSION ${STTNGS_LIBVER_STRING}
                                           SOVERSION ${STTNGS_LIBVER_MAJOR})

#for cross compiling
#cmake -H. -BOutput -DCMAKE_TOOLCHAIN_FILE=/path/to/toolchain-arm-backup.cmake -DCMAKE_INSTALL_PREFIX=/home/adav/tmp/
#cmake --build Output -- all
#cmake --build Output -- install

#for local compiling
#cmake -H. -BOutput
#cmake --build Output -- all

#for cleaning make generated output
#rm -rf Output

