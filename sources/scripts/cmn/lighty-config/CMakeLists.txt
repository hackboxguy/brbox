#this cmake is only used for installing scripts to target)
cmake_minimum_required(VERSION 2.8.9)
set(LIGHTYCFG_DIR_LISTING dirlisting.conf)
set(LIGHTYCFG_STARTUP S99PostLightyConfig) 

file(GLOB  LIGHTYCFGFILES ${LIGHTYCFG_DIR_LISTING} )
file(GLOB  STARTUPFILES ${LIGHTYCFG_STARTUP})

install(FILES ${LIGHTYCFGFILES} DESTINATION  etc/lighttpd/conf.d)
install(PROGRAMS ${STARTUPFILES} DESTINATION etc/init.d)

if(CMAKE_HOST_UNIX)
install(CODE "EXECUTE_PROCESS(WORKING_DIRECTORY \$ENV{TARGET_DIR}\${CMAKE_INSTALL_PREFIX}/var/www COMMAND ln -sf /tmp/downloads downloads)")
#install(CODE "EXECUTE_PROCESS(WORKING_DIRECTORY \$ENV{TARGET_DIR}\${CMAKE_INSTALL_PREFIX}/tmp COMMAND mkdir -p /tmp/downloads)")
endif(CMAKE_HOST_UNIX)

