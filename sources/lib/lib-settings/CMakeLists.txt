cmake_minimum_required(VERSION 2.8.9)

#set the name of the library here
set(STTNGS_LIBNAME AdSttngs)

#set the version of the library here
set(STTNGS_LIBVER_MAJOR 0)
set(STTNGS_LIBVER_MINOR 1)
set(STTNGS_LIBVER_PATCH 1)
set(STTNGS_LIBVER_STRING ${STTNGS_LIBVER_MAJOR}.${STTNGS_LIBVER_MINOR}.${STTNGS_LIBVER_PATCH})

project(${STTNGS_LIBNAME})
set(CMAKE_BUILD_TYPE Release)

SET(OUTBIN "./" CACHE STRING "./")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUT_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTBIN})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTBIN})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTBIN})


#Bring the headers into project
include_directories(include)
 
#GLOB allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")


#Generate the shared library from the sources
add_library(${STTNGS_LIBNAME} SHARED ${SOURCES})
 
#Set the location for library installation -- i.e., /usr/lib in this case
# Use "sudo make install" to apply
install(TARGETS ${STTNGS_LIBNAME} DESTINATION /usr/lib)

set_target_properties(${STTNGS_LIBNAME} PROPERTIES VERSION ${STTNGS_LIBVER_STRING}
                                           SOVERSION ${STTNGS_LIBVER_MAJOR})



