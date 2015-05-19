cmake_minimum_required(VERSION 2.8.9)

#set the name of the library here
set(ADCMN_LIBNAME AdCmn)
#set the path of dependent library here
set(LIBADSTTNG_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../lib-settings/include")
set(AUTO_SVN_VERSION    ON CACHE BOOL ON)  #by default include svn revision numbers
#set(INSTALL_MY_HEADERS  ON CACHE BOOL ON)  #by default install header files to target-install-directory


#set the version of the library here
set(ADCMN_LIBVER_MAJOR 0)
set(ADCMN_LIBVER_MINOR 1)
set(ADCMN_LIBVER_PATCH 1)
set(ADCMN_LIBVER_STRING ${ADCMN_LIBVER_MAJOR}.${ADCMN_LIBVER_MINOR}.${ADCMN_LIBVER_PATCH})

project(${ADCMN_LIBNAME})
set(CMAKE_BUILD_TYPE Release)

#Bring the myheaders into project + my dependent headers
#(note that lib-adav-old depends on lib-settings, hence including lib-settings headers too)
include_directories(include ${LIBADSTTNG_SOURCE_DIR})
 
#GLOB allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")


#Generate the shared library from the sources
add_library(${ADCMN_LIBNAME} SHARED ${SOURCES})

#Set the location for library installation -- i.e., /usr/local/lib by default or override with CMAKE_INSTALL_PREFIX
# Use "sudo make install" to apply
file(GLOB  INCFILES "include/*.*")
install(TARGETS ${ADCMN_LIBNAME} DESTINATION lib)
#if(${INSTALL_MY_HEADERS})
install(FILES ${INCFILES} DESTINATION include)
#endif()


set_target_properties(${ADCMN_LIBNAME} PROPERTIES VERSION ${ADCMN_LIBVER_STRING}
                                           SOVERSION ${ADCMN_LIBVER_MAJOR})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/")
#list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../../cmake")
find_package (JSON)  #REQUIRED
if (JSON_FOUND)
  include_directories(${JSON_INCLUDE_DIRS})
else()
	message( FATAL_ERROR "Error!!! Missing json-c library, try sudo apt-get install libjson-c-dev" )
endif (JSON_FOUND)

#for cross compiling
#cmake -H. -BOutput -DCMAKE_TOOLCHAIN_FILE=/path/to/toolchain-arm-backup.cmake -DCMAKE_INSTALL_PREFIX=/home/user/tmp/
#cmake --build Output -- all
#cmake --build Output -- install

#for local compiling
#cmake -H. -BOutput
#cmake --build Output -- all

#for cleaning make generated output
#rm -rf Output

