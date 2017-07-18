#this cmake is only used for installing scripts to target)
cmake_minimum_required(VERSION 2.8.9)
#set(LIGHTYCFG_DIR_LISTING dirlisting.conf)
set(TFTPDCFG_STARTUP S80tftpd-hpa) 

#file(GLOB  LIGHTYCFGFILES ${LIGHTYCFG_DIR_LISTING} )
file(GLOB  STARTUPFILES ${TFTPDCFG_STARTUP})

#install(FILES ${LIGHTYCFGFILES} DESTINATION  etc/lighttpd/conf.d)
install(PROGRAMS ${STARTUPFILES} DESTINATION etc/init.d)

#if(CMAKE_HOST_UNIX)
#install(CODE "EXECUTE_PROCESS(WORKING_DIRECTORY \$ENV{TARGET_DIR}\${CMAKE_INSTALL_PREFIX}/var/www COMMAND ln -sf /tmp/downloads downloads)")
#endif(CMAKE_HOST_UNIX)

