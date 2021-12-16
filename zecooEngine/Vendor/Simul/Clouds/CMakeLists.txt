cmake_minimum_required(VERSION 3.14)

include_directories( ../.. )

file(GLOB CMAKE
    "*.cmake"
)

file(GLOB SOURCES
    "*.cpp"
)

file(GLOB HEADERS
    "*.h"
)

add_static_library( SimulClouds SOURCES ${SOURCES} ${HEADERS} ${CMAKE} DEFINITIONS -DSIMUL_ALWAYS_PROFILE=$<BOOL:${SIMUL_ALWAYS_PROFILE}>)

add_dynamic_library(SimulClouds SOURCES ${SOURCES} ${HEADERS} ${CMAKE}
	DEFINITIONS -DSIMUL_DYNAMIC_LINK -DSIMUL_CLOUDS_DLL -DSIMUL_ALWAYS_PROFILE=$<BOOL:${SIMUL_ALWAYS_PROFILE}>
	LINK SimulTerrain_MD SimulSky_MD SimulBase_MD SimulMath_MD SimulCrossPlatform_MD)
