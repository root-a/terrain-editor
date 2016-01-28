#-------------------------------------------------------------------------------
# demoproject
#-------------------------------------------------------------------------------

# glob the source files and add them to folders in visual studio
FILE(GLOB SRC "gamestates/*.cc")
FILE(GLOB HEAD "gamestates/*.h")
LIST(APPEND SRCSGAMESTATES ${SRC} ${HEAD})
SOURCE_GROUP("demoproject\\gamestates" FILES ${SRCSGAMESTATES})

FILE(GLOB SRC "*.cc")
FILE(GLOB HEAD "*.h")
LIST(APPEND SRCSBASE ${SRC} ${HEAD})
SOURCE_GROUP("demoproject" FILES ${SRCSBASE})

# define precompiled headers
SET(PCH_files	
    ../stdneb.cc
)
SOURCE_GROUP("Precompiled" FILES ${PCH_files})

LIST(APPEND ALLSRCS ${SRCSBASE} ${SRCSGAMESTATES} ${PCH_files})

ADD_EXECUTABLE(demoproject ${ALLSRCS})
SET_TARGET_PROPERTIES(demoproject PROPERTIES FOLDER "Demos")

# link to application project, will pull in all dependencies and includes required
TARGET_LINK_LIBRARIES(demoproject application)
ADD_DEPENDENCIES(demoproject application)

# apply default compiler settings, has to be done to be consistent throughout
N_SET_COMPILER_DEFAULTS()
N_SET_EXE_LINKER_DEFAULTS(demoproject)
N_SET_EXE_SUBSYSTEM(demoproject Windows)

# setup precompiled headers if chosen at project level
IF(N_USE_PRECOMPILED_HEADERS)
	N_SET_PCH(demoproject  stdneb.h stdneb.cc ../)
ENDIF()