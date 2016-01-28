#-------------------------------------------------------------------------------
# consoletest
#-------------------------------------------------------------------------------


SET(SOURCES consoletest.cc consoletestapp.cc consoletestapp.h)
SOURCE_GROUP("consoletest" FILES ${SOURCES})

set(PCH_files	
    ../stdneb.cc
)
SOURCE_GROUP("Precompiled" FILES ${PCH_files})

LIST(APPEND ALLSRCS ${SOURCES} ${PCH_files})

ADD_EXECUTABLE(consoletest ${ALLSRCS})
SET_TARGET_PROPERTIES(consoletest PROPERTIES FOLDER "Demos")

N_SET_COMPILER_DEFAULTS()
N_SET_EXE_LINKER_DEFAULTS(consoletest)
N_SET_EXE_SUBSYSTEM(consoletest Console)
IF(N_USE_PRECOMPILED_HEADERS)
	N_SET_PCH(consoletest stdneb.h stdneb.cc ../)
ENDIF()

ADD_DEPENDENCIES(consoletest
	foundation	
	)
TARGET_LINK_LIBRARIES(consoletest 
	foundation	
)
