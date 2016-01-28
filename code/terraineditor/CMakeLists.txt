#-------------------------------------------------------------------------------
# terraineditor
#-------------------------------------------------------------------------------

# glob sources and put them in folders
FILE(GLOB SRC "*.cc")
FILE(GLOB HEAD "*.h")
LIST(APPEND SRCS ${SRC} ${HEAD})
SOURCE_GROUP("terraineditor" FILES ${SRCS})

set(PCH_files	
    ../stdneb.cc
)
SOURCE_GROUP("Precompiled" FILES ${PCH_files})

LIST(APPEND ALLSRCS ${SRCS} ${PCH_files})

ADD_EXECUTABLE(terraineditor ${ALLSRCS})
SET_TARGET_PROPERTIES(terraineditor PROPERTIES FOLDER "Demos")

# link to render, will pull in dependencies and includes
TARGET_LINK_LIBRARIES(terraineditor render imgui dynui framecapture)
ADD_DEPENDENCIES(terraineditor render imgui dynui framecapture)

# default nebula compiler settings
N_SET_COMPILER_DEFAULTS()
N_SET_EXE_LINKER_DEFAULTS(terraineditor)
N_SET_EXE_SUBSYSTEM(terraineditor Windows)
IF(N_USE_PRECOMPILED_HEADERS)
	N_SET_PCH(terraineditor stdneb.h stdneb.cc ../)
ENDIF()