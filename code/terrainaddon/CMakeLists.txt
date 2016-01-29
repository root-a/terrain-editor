#-------------------------------------------------------------------------------
# terrainaddon
#-------------------------------------------------------------------------------

# glob sources and put them in folders
FILE(GLOB SRC "*.cc")
FILE(GLOB HEAD "*.h")
LIST(APPEND SRCS ${SRC} ${HEAD})
SOURCE_GROUP("terrainaddon" FILES ${SRCS})

set(PCH_files	
    ../stdneb.cc
)
SOURCE_GROUP("Precompiled" FILES ${PCH_files})

LIST(APPEND ALLSRCS ${SRCS} ${PCH_files})

ADD_LIBRARY(terrainaddon STATIC ${ALLSRCS})
SET_TARGET_PROPERTIES(terrainaddon PROPERTIES FOLDER "DTerrainTools")
TARGET_INCLUDE_DIRECTORIES(terrainaddon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# link to render, will pull in dependencies and includes
TARGET_LINK_LIBRARIES(terrainaddon render imgui dynui framecapture)
ADD_DEPENDENCIES(terrainaddon render imgui dynui framecapture)

# default nebula compiler settings
N_SET_COMPILER_DEFAULTS()
N_SET_EXE_LINKER_DEFAULTS(terrainaddon)
N_SET_EXE_SUBSYSTEM(terrainaddon Windows)
IF(N_USE_PRECOMPILED_HEADERS)
	N_SET_PCH(terrainaddon stdneb.h stdneb.cc ../)
ENDIF()