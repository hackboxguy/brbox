cmake_minimum_required(VERSION 2.8.9)
set(COMPONENT_NAME monitclt) #set the name of the binary
set(LIB_ADSTTNG AdSttngs)   #settings library
set(LIB_ADCOMMN AdCmn)      #common library






project(${COMPONENT_NAME})
set(AUTO_SVN_VERSION     ON CACHE BOOL ON)  #by default include svn revision numbers
file(GLOB SOURCES "src/*.cpp")
add_executable(${COMPONENT_NAME} ${SOURCES})
set(INC_ADSTTNG  "${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/lib-settings/include")
set(INC_ADCOMMN  "${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/lib-adav-old/include")
set(INC_SRVCOMMN "${CMAKE_CURRENT_SOURCE_DIR}/../include") #rpc server's cmn header file


# Add #include path of all dependent components
include_directories(${INC_ADSTTNG} ${INC_ADCOMMN} ${INC_SRVCOMMN} ${CMAKE_CURRENT_BINARY_DIR})

# Specify component's dependency.
target_link_libraries(${COMPONENT_NAME} ${LIB_ADCOMMN} ${LIB_ADSTTNG} json-c pthread rt)

#install the binary and server-rpc-headers to CMAKE_INSTALL_PREFIX destination
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

