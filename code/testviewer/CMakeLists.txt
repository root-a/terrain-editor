#-------------------------------------------------------------------------------
# Testviewer
#-------------------------------------------------------------------------------

# glob sources and put them in folders
FILE(GLOB SRC "*.cc")
FILE(GLOB HEAD "*.h")
LIST(APPEND SRCS ${SRC} ${HEAD})
SOURCE_GROUP("testviewer" FILES ${SRCS})

set(PCH_files	
    ../stdneb.cc
)
SOURCE_GROUP("Precompiled" FILES ${PCH_files})

LIST(APPEND ALLSRCS ${SRCS} ${PCH_files})

ADD_EXECUTABLE(testviewer ${ALLSRCS})
SET_TARGET_PROPERTIES(testviewer PROPERTIES FOLDER "Demos")

# link to render, will pull in dependencies and includes
TARGET_LINK_LIBRARIES(testviewer render imgui dynui framecapture)
ADD_DEPENDENCIES(testviewer render imgui dynui framecapture)

# default nebula compiler settings
N_SET_COMPILER_DEFAULTS()
N_SET_EXE_LINKER_DEFAULTS(testviewer)
N_SET_EXE_SUBSYSTEM(testviewer Windows)
IF(N_USE_PRECOMPILED_HEADERS)
	N_SET_PCH(testviewer stdneb.h stdneb.cc ../)
ENDIF()