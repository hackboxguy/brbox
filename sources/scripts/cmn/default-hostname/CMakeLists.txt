#this cmake is only used for installing scripts to target)
cmake_minimum_required(VERSION 2.8.9)
set(ACTION_SCR default-hostname)          #action script to be copied to target
set(ACTION_STARTUP S02DefaultHostname)    #startup script to be copied to target
file(GLOB  SCRIPTFILES ${ACTION_SCR})
file(GLOB  STARTUPFILES ${ACTION_STARTUP})
install(PROGRAMS ${SCRIPTFILES} DESTINATION usr/sbin)
install(PROGRAMS ${STARTUPFILES} DESTINATION etc/init.d)

