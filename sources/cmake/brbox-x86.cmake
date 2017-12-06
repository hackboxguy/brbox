# this one is important
SET(CMAKE_SYSTEM_NAME Linux)
#this one not so much
#SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
SET(CMAKE_C_COMPILER   /opt/brbox-x86/usr/bin/x86_64-buildroot-linux-gnu-gcc)
SET(CMAKE_CXX_COMPILER /opt/brbox-x86/usr/bin/x86_64-buildroot-linux-gnu-g++)

# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH  /opt/brbox-x86 /opt/brbox-x86/usr/x86_64-buildroot-linux-gnu/sysroot)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
