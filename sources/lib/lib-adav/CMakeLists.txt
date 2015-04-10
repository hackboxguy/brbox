cmake_minimum_required(VERSION 2.8.9)

#set the name of the library as libAdCmn
set(ADCMN_LIBNAME AdCmn)
#set(STTNGS_LIBNAME AdSttngs)
set(LIBADSTTNG_SOURCE_DIR ../lib-settings/include)

#set the version of the library here
set(ADCMN_LIBVER_MAJOR 0)
set(ADCMN_LIBVER_MINOR 1)
set(ADCMN_LIBVER_PATCH 1)
set(ADCMN_LIBVER_STRING ${ADCMN_LIBVER_MAJOR}.${ADCMN_LIBVER_MINOR}.${ADCMN_LIBVER_PATCH})

project(${ADCMN_LIBNAME})
set(CMAKE_BUILD_TYPE Release)

#SET(OUTBIN "./" CACHE STRING "./") #this value can be passed via cmdline as "cmake . -DOUTBIN=/tmp/"
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTBIN}/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTBIN}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTBIN}/lib)


#Bring the headers into project (note that lib-adav depends on lib-settings, hence including lib-settings headers)
include_directories(include ${LIBADSTTNG_SOURCE_DIR})
 
#GLOB allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")


#Generate the shared library from the sources
add_library(${ADCMN_LIBNAME} SHARED ${SOURCES})

#Set the location for library installation -- i.e., /usr/local/lib by default or override with CMAKE_INSTALL_PREFIX
# Use "sudo make install" to apply
file(GLOB  INCFILES "include/*.h")
install(TARGETS ${ADCMN_LIBNAME} DESTINATION lib)
install(FILES ${INCFILES} DESTINATION include)



set_target_properties(${ADCMN_LIBNAME} PROPERTIES VERSION ${ADCMN_LIBVER_STRING}
                                           SOVERSION ${ADCMN_LIBVER_MAJOR})

# Add libAdSttng to #include search path
#include_directories(${AdSttngs_SOURCE_DIR}/include)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")
find_package (JSON)  #REQUIRED
if (JSON_FOUND)
  include_directories(${JSON_INCLUDE_DIRS})
else()
	message( FATAL_ERROR "Error!!! Missing json-c library, try sudo apt-get install libjson-c-dev" )
endif (JSON_FOUND)

#for cross compiling
#cmake -H. -BOutput -DCMAKE_TOOLCHAIN_FILE=/path/to/toolchain-arm-backup.cmake
#cmake --build Output -- all

#for local compiling
#cmake -H. -BOutput
#cmake --build Output -- all

#for cleaning make generated output
#rm -rf Output

