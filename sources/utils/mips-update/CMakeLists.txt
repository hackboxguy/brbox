cmake_minimum_required(VERSION 2.8.9)
file(GLOB  UPDATEFILES "*.sh")
install(PROGRAMS ${UPDATEFILES} DESTINATION sbin)
