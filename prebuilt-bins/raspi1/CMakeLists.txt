#this cmake is only used for installing scripts to target)
cmake_minimum_required(VERSION 2.8.9)
set(ZBARIMG_BIN_FILE zbarimg)

file(GLOB  BIN_FILES ${ZBARIMG_BIN_FILE})
install(PROGRAMS ${BIN_FILES} DESTINATION usr/bin/)

