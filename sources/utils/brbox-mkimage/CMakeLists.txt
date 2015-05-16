cmake_minimum_required(VERSION 2.8.9)
set(COMPONENT_NAME brbox-mkimage)  #set the name of the binary


project(${COMPONENT_NAME})
set(MKIMG_HOST_BUILD ON CACHE BOOL ON) #by default switch-off local-x86 host build
file(GLOB SOURCES "*.c")
# for local x86 host, build as 32bit binary
if(${MKIMG_HOST_BUILD})
	set(CMAKE_C_COMPILER gcc)
	set(CMAKE_C_FLAGS -m32)
	set(CMAKE_CXX_COMPILER g++)
	set(CMAKE_CXX_FLAGS -m32)
endif()
set(CMAKE_EXE_LINKER_FLAGS -static)

# Add #include path of all dependent components
include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(${COMPONENT_NAME} ${SOURCES})
add_definitions( -DUSE_HOSTCC )

install(TARGETS ${COMPONENT_NAME} DESTINATION sbin)

#automatically include svn revision to a headerfile which will be included in this component
#find_package(Subversion REQUIRED)
#if(SUBVERSION_FOUND)
#  Subversion_WC_INFO(${PROJECT_SOURCE_DIR} Project)
#  FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/SrcControlVersion.h "#define SRC_CONTROL_VERSION ${Project_WC_LAST_CHANGED_REV}\n")
#else()
#	message( FATAL_ERROR "unable to read sub-version Revision number, this is needed for embedding into this component" )
#endif()

FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/SrcControlVersion.h "#define SRC_CONTROL_VERSION 0001\n")

#for cross compiling
#cmake -H. -BOutput -DCMAKE_TOOLCHAIN_FILE=/path/to/toolchain-arm-backup.cmake -DCMAKE_INSTALL_PREFIX=/home/user/tmp/
#cmake --build Output -- all
#cmake --build Output -- install

#for local compiling
#cmake -H. -BOutput
#cmake --build Output -- all

#for cleaning make generated output
#rm -rf Output

