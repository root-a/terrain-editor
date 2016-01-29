#-------------------------------------------------------------------------------
# Demos
#-------------------------------------------------------------------------------

OPTION(N_BUILD_N3DEMOS "Build N3 demos" ON)

if(N_BUILD_N3DEMOS)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SUBDIRS(
terraineditor
)
ENDIF()
