#this cmake is only used for installing scripts to target)
cmake_minimum_required(VERSION 2.8.9)
set(MISC_BIN_FILES *.png)
set(MISC_MKV_FILES *.mkv)
set(MISC_SCRIPT_FILES *.sh)
file(GLOB  BIN_FILES ${MISC_BIN_FILES} ${MISC_MKV_FILES} ${MISC_SCRIPT_FILES})
install(FILES ${BIN_FILES} DESTINATION opt/fmw/misc_binaries/)

