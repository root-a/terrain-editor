#-------------------------------------------------------------------------------
# terrain
#-------------------------------------------------------------------------------

# glob sources and put them in folders
FILE(GLOB SRC "*.cc")
FILE(GLOB HEAD "*.h")
LIST(APPEND SRCS ${SRC} ${HEAD})
SOURCE_GROUP("terrain" FILES ${SRCS})

set(PCH_files	
    ../stdneb.cc
)
SOURCE_GROUP("Precompiled" FILES ${PCH_files})

LIST(APPEND ALLSRCS ${SRCS} ${PCH_files})

ADD_LIBRARY(terrain STATIC ${ALLSRCS})
SET_TARGET_PROPERTIES(terrain PROPERTIES FOLDER "DTerrainTools")
TARGET_INCLUDE_DIRECTORIES(terrain PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# link to render, will pull in dependencies and includes
TARGET_LINK_LIBRARIES(terrain render imgui dynui framecapture)
ADD_DEPENDENCIES(terrain render imgui dynui framecapture)

# default nebula compiler settings
N_SET_COMPILER_DEFAULTS()
N_SET_EXE_LINKER_DEFAULTS(terrain)
N_SET_EXE_SUBSYSTEM(terrain Windows)
IF(N_USE_PRECOMPILED_HEADERS)
	N_SET_PCH(terrain stdneb.h stdneb.cc ../)
ENDIF()