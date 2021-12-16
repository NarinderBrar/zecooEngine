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

add_static_library( SimulSky SOURCES ${SOURCES} ${HEADERS} ${CMAKE})

add_dynamic_library(SimulSky SOURCES ${SOURCES} ${HEADERS} ${CMAKE} DEFINITIONS SIMUL_SKY_DLL=1
	LINK SimulMath_MD SimulCrossPlatform_MD SimulBase_MD)
