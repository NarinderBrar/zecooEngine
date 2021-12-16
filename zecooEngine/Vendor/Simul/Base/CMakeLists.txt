# Copyright 2017 Simul Software Ltd, All rights reserved.

cmake_minimum_required( VERSION 3.2 )
# Grab the includes/source files.
file(GLOB SOURCES "*.cpp" )
file(GLOB HEADERS "*.h" )

# Setup and build the static library.
add_static_library( SimulBase SOURCES ${SOURCES} ${HEADERS})

# Setup and build the dynamic library.
add_dynamic_library( SimulBase SOURCES ${SOURCES} ${HEADERS} DEFINITIONS SIMUL_BASE_DLL=1  LINK Core_MD)
