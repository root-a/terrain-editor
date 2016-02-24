#-------------------------------------------------------------------------------
# terrainqtaddon
#-------------------------------------------------------------------------------

PROJECT(terrainqtaddon)

FILE(GLOB SRC "code/*.cc") 
FILE(GLOB HEAD "code/*.h")
FILE(GLOB FORMS "ui/*.ui")
SOURCE_GROUP("terrainqtaddon" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

SET(QTMOCFILES "code/terrainaddoncontroller.h")
PCH_QT4_WRAP_CPP(terrainqtaddon_HEADERS_MOC ${QTMOCFILES})
QT4_WRAP_UI(terrainqtaddon_FORMS_HEADERS ${FORMS})

SET(PCH_files	
    ../stdneb.cc
)
SOURCE_GROUP("Precompiled" FILES ${PCH_files})


LIST(APPEND ALLSRCS ${CODE} ${PCH_files} ${terrainqtaddon_HEADERS_MOC} ${terrainqtaddon_FORMS_HEADERS})
ADD_LIBRARY(terrainqtaddon STATIC ${ALLSRCS})
TARGET_INCLUDE_DIRECTORIES(terrainqtaddon PUBLIC ${Nebula3_SOURCE_DIR}/projects/terrain-editor/code/terrainqtaddon/code PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
TARGET_LINK_LIBRARIES(terrainqtaddon application toolkitutil)
SET_TARGET_PROPERTIES(terrainqtaddon PROPERTIES FOLDER "DTerrainTools")
N_SET_COMPILER_DEFAULTS()		