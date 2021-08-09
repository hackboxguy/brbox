cmake_minimum_required(VERSION 2.8.9)
set(COMPONENT_NAME xmproxysrv)           #server name
set(STARTUP_SCRIPT XmproxyStartupScr)   #server startup script
set(WRTSTARTUP_SCRIPT WrtXmproxyStartupScr) #server startup script for openwrt based system
set(SYSTEMDSTARTUP_SCRIPT xmproxysrv.service) #server startup script for systemd based system
set(STARTNUM "74" CACHE STRING "74")    #init order number of the service
set(LIB_ADSTTNG AdSttngs)   #settings library
set(LIB_ADCOMMN AdCmn)      #common library

option(WITH_CXMPP_LIB "link with libstrophe xmpp c lib" OFF)
if(WITH_CXMPP_LIB)
  add_definitions(-DUSE_CXMPP_LIB)
endif()




project(${COMPONENT_NAME})
set(CMAKE_CXX_FLAGS "-std=c++11")
set(AUTO_SVN_VERSION    ON CACHE BOOL ON)  #by default include svn revision numbers
set(INSTALL_MY_HEADERS  ON CACHE BOOL ON)  #by default install header files to target-install-directory
file(GLOB SOURCES "src/*.cpp")
if(WITH_CXMPP_LIB)
	LIST(REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/ADXmppProxy.cpp)
else()
	LIST(REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/CXmppProxy.cpp)
endif()


add_executable(${COMPONENT_NAME} ${SOURCES})
set(INC_ADSTTNG  "${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/lib-settings/include")
set(INC_ADCOMMN  "${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/lib-adav-old/include")
set(INC_SRVCOMMN "${CMAKE_CURRENT_SOURCE_DIR}/../include")
set(INC_LIBSTROPHE "${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/libstrophe")
include_directories(${INC_ADSTTNG} ${INC_ADCOMMN} ${INC_SRVCOMMN} ${CMAKE_CURRENT_BINARY_DIR})

# Specify component's dependency.
if(WITH_CXMPP_LIB)
	target_link_libraries(${COMPONENT_NAME} ${LIB_ADCOMMN} ${LIB_ADSTTNG} json-c pthread rt strophe)
else()
	target_link_libraries(${COMPONENT_NAME} ${LIB_ADCOMMN} ${LIB_ADSTTNG} json-c pthread rt gloox)
endif()

#install the binary and server-rpc-headers to CMAKE_INSTALL_PREFIX destination
file(GLOB  INCFILES "${INC_SRVCOMMN}/*.*")
install(TARGETS ${COMPONENT_NAME} DESTINATION bin)
install(PROGRAMS ${STARTUP_SCRIPT} DESTINATION etc/init.d) #copy server's startup script to init.d
install(PROGRAMS ${WRTSTARTUP_SCRIPT} DESTINATION etc/init.d) #copy server's startup script to init.d
install(PROGRAMS ${SYSTEMDSTARTUP_SCRIPT} DESTINATION etc/systemd/system) #copy server's startup script to systemd/system
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

if(WITH_CXMPP_LIB)
	include_directories( ${INC_LIBSTROPHE} )
else()
	INCLUDE( FindPkgConfig )
	pkg_check_modules( GLOOX gloox )
	if( NOT GLOOX_FOUND )
		message( FATAL_ERROR "ERROR: could not find gloox development files" )
	endif( NOT GLOOX_FOUND )
	include_directories( ${GLOOX_INCLUDE_DIR} )
	include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party )
	include_directories( ${GLOOX_INCLUDE_DIRS} )
endif()

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/")
#find_package (GAMMU)  #REQUIRED
#if (GAMMU_FOUND)
 # include_directories(${GAMMU_INCLUDE_DIRS})
#else()
#	message( FATAL_ERROR "Error!!! Missing gammu library, try sudo apt-get install libgammu-dev" )
#endif (GAMMU_FOUND)

#for cross compiling
#cmake -H. -BOutput -DCMAKE_TOOLCHAIN_FILE=/path/to/toolchain-arm-backup.cmake -DCMAKE_INSTALL_PREFIX=/home/user/tmp/
#cmake --build Output -- all
#cmake --build Output -- install

#for local compiling
#cmake -H. -BOutput
#cmake --build Output -- all

#for cleaning make generated output
#rm -rf Output

