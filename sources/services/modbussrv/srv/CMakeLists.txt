cmake_minimum_required(VERSION 2.8.9)
set(COMPONENT_NAME modbussrv) #set the name of the binary
set(LIB_ADDISP AdDisp)      #oled display control library
set(LIB_ADSTTNG AdSttngs)   #settings library
set(LIB_ADCOMMN AdCmn)      #common library





project(${COMPONENT_NAME})
set(AUTO_SVN_VERSION    ON CACHE BOOL ON)  #by default include svn revision numbers
set(INSTALL_MY_HEADERS  ON CACHE BOOL ON)  #by default install header files to target-install-directory
set(STARTUP_SCRIPT ModbusSrvStartupScr)    #server startup script
set(WRTSTARTUP_SCRIPT WrtModbusSrvStartupScr)          #server startup script
set(STARTNUM "81" CACHE STRING "81")       #init order number of the service
file(GLOB SOURCES "src/*.cpp")
add_executable(${COMPONENT_NAME} ${SOURCES})
set(INC_ADDISP   "${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/lib-display/include")
set(INC_ADSTTNG  "${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/lib-settings/include")
set(INC_ADCOMMN  "${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/lib-adav-old/include")
set(INC_SRVCOMMN "${CMAKE_CURRENT_SOURCE_DIR}/../include")
#find_package( libi2c-dev REQUIRED ) #core imgproc highgui )
# Add #include path of all dependent components
include_directories(${INC_ADDISP} ${INC_ADSTTNG} ${INC_ADCOMMN} ${INC_SRVCOMMN} ${CMAKE_CURRENT_BINARY_DIR})

# Specify component's dependency.
target_link_libraries(${COMPONENT_NAME} ${LIB_ADCOMMN} ${LIB_ADDISP} ${LIB_ADSTTNG} json-c pthread rt)

#install the binary and server-rpc-headers to CMAKE_INSTALL_PREFIX destination
file(GLOB  INCFILES "${INC_SRVCOMMN}/*.*")
install(TARGETS ${COMPONENT_NAME} DESTINATION bin)
install(PROGRAMS ${STARTUP_SCRIPT} DESTINATION etc/init.d) #copy server's startup script to init.d
install(PROGRAMS ${WRTSTARTUP_SCRIPT} DESTINATION etc/init.d) #copy server's startup script to init.d
if(${INSTALL_MY_HEADERS})
	install(FILES ${INCFILES} DESTINATION include)
endif()
if(CMAKE_HOST_UNIX)
install(CODE "EXECUTE_PROCESS(WORKING_DIRECTORY \$ENV{TARGET_DIR}\${CMAKE_INSTALL_PREFIX}/etc/init.d COMMAND ln -sf ${STARTUP_SCRIPT}  S${STARTNUM}${STARTUP_SCRIPT})")
endif(CMAKE_HOST_UNIX)

#automatically include svn revision to a headerfile which will be included in this component
if(${AUTO_SVN_VERSION})
	find_package(Subversion REQUIRED)
	if(SUBVERSION_FOUND)
	  Subversion_WC_INFO(${PROJECT_SOURCE_DIR} Project)
	  FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/SrcControlVersion.h "#define SRC_CONTROL_VERSION ${Project_WC_LAST_CHANGED_REV}\n")
	else()
		message( FATAL_ERROR "unable to read sub-version Revision number, this is needed for embedding into this component" )
	endif()
else()
	FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/SrcControlVersion.h "#define SRC_CONTROL_VERSION 0001\n")
endif()

#INCLUDE( FindPkgConfig )
#pkg_check_modules( I2CDEV i2c-dev )
#if( NOT I2CDEV_FOUND )
#	message( FATAL_ERROR "ERROR: could not find libi2c-dev development files" )
#endif( NOT I2CDEV_FOUND )
#include_directories( ${I2CDEV_INCLUDE_DIR} )
#include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party )
#include_directories( ${I2CDEV_INCLUDE_DIRS} )

#for cross compiling
#cmake -H. -BOutput -DCMAKE_TOOLCHAIN_FILE=/path/to/toolchain-arm-backup.cmake -DCMAKE_INSTALL_PREFIX=/home/user/tmp/
#cmake --build Output -- all
#cmake --build Output -- install

#for local compiling
#cmake -H. -BOutput
#cmake --build Output -- all

#for cleaning make generated output
#rm -rf Output

