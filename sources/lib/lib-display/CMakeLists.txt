cmake_minimum_required(VERSION 2.8.9)

#set the name of the library here
set(DISP_LIBNAME AdDisp)

#set the version of the library here
set(DISP_LIBVER_MAJOR 0)
set(DISP_LIBVER_MINOR 1)
set(DISP_LIBVER_PATCH 1)
set(DISP_LIBVER_STRING ${DISP_LIBVER_MAJOR}.${DISP_LIBVER_MINOR}.${DISP_LIBVER_PATCH})

#set(CMAKE_INSTALL_PREFIX /home/adav/usr/lib)
project(${DISP_LIBNAME})
set(CMAKE_BUILD_TYPE Release)

#SET(OUTBIN "./" CACHE STRING "./") #this value can be passed via cmdline as "cmake . -DOUTBIN=/tmp/"
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTBIN}/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTBIN}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTBIN}/lib)


#Bring the headers into project
include_directories(include)
 
#GLOB allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp" "src/*.c")


#Generate the shared library from the sources
add_library(${DISP_LIBNAME} SHARED ${SOURCES})
 
#Set the location for library installation -- i.e., /usr/local/lib by default or override with CMAKE_INSTALL_PREFIX
# Use "sudo make install" to apply
file(GLOB  INCFILES "include/*.h")
install(TARGETS ${DISP_LIBNAME} DESTINATION lib)
install(FILES ${INCFILES} DESTINATION include)

set_target_properties(${DISP_LIBNAME} PROPERTIES VERSION ${DISP_LIBVER_STRING}
                                           SOVERSION ${DISP_LIBVER_MAJOR})

#for cross compiling
#cmake -H. -BOutput -DCMAKE_TOOLCHAIN_FILE=/path/to/toolchain-arm-backup.cmake -DCMAKE_INSTALL_PREFIX=/home/adav/tmp/
#cmake --build Output -- all
#cmake --build Output -- install

#for local compiling
#cmake -H. -BOutput
#cmake --build Output -- all

#for cleaning make generated output
#rm -rf Output

