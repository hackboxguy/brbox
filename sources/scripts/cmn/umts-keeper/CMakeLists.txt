#this cmake is only used for installing scripts to target)
cmake_minimum_required(VERSION 2.8.9)
set(UMTS_SCR_FILE1 favicon.ico)        #umts script to be copied to target
set(UMTS_SCR_FILE2 robots.txt)         #umts script to be copied to target
set(UMTS_SCR_FILE3 style.css)          #umts script to be copied to target
set(UMTS_SCR_FILE4 umtskeeper)         #umts script to be copied to target
set(UMTS_SCR_FILE5 sakis3g)            #umts script to be copied to target
set(UMTS_SCR_STARTUP S55UmtsKeeper)    #umts script to be copied to target

file(GLOB  UMTSFILES    ${UMTS_SCR_FILE1} ${UMTS_SCR_FILE2} ${UMTS_SCR_FILE3} ${UMTS_SCR_FILE4} ${UMTS_SCR_FILE5})
file(GLOB  UMTS_STARTUP ${UMTS_SCR_STARTUP})
install(PROGRAMS ${UMTSFILES} DESTINATION  etc/umts-keeper)
install(PROGRAMS ${UMTS_STARTUP} DESTINATION etc/init.d)

