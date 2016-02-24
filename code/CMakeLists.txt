#-------------------------------------------------------------------------------
# TerrainTools
#-------------------------------------------------------------------------------

OPTION(N_BUILD_N3TERRAIN "Build N3 TerrainTools" ON)

if(N_BUILD_N3TERRAIN)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SUBDIRS(
terraineditor
terrainaddon
terrainqtaddon
)
ENDIF()
