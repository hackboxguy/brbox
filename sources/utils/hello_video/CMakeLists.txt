cmake_minimum_required(VERSION 2.8.9)
set(COMPONENT_NAME hello_video)  #set the name of the binary
set(LIB_ILCLT AdIlClt)      #oled display control library


project(${COMPONENT_NAME})
set(AUTO_SVN_VERSION    ON CACHE BOOL ON)  #by default include svn revision numbers
set(INSTALL_MY_HEADERS  ON CACHE BOOL ON)  #by default install header files to target-install-directory



file(GLOB SOURCES "src/*.c")
add_executable(${COMPONENT_NAME} ${SOURCES})
set(INC_ILCLT   "${CMAKE_CURRENT_SOURCE_DIR}/../../lib/lib-ilclient/include")

#find_package( libi2c-dev REQUIRED ) #core imgproc highgui )
# Add #include path of all dependent components
include_directories(${INC_ILCLT} ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(${COMPONENT_NAME} ${LIB_ILCLT} GLESv2 EGL openmaxil bcm_host vcos vchiq_arm pthread rt m)

#install the binary and server-rpc-headers to CMAKE_INSTALL_PREFIX destination
file(GLOB  INCFILES "${INC_SRVCOMMN}/*.*")
install(TARGETS ${COMPONENT_NAME} DESTINATION bin)


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

#for cross compiling
#cmake -H. -BOutput -DCMAKE_TOOLCHAIN_FILE=/path/to/toolchain-arm-backup.cmake -DCMAKE_INSTALL_PREFIX=/home/user/tmp/
#cmake --build Output -- all
#cmake --build Output -- install

#for local compiling
#cmake -H. -BOutput
#cmake --build Output -- all

#for cleaning make generated output
#rm -rf Output

