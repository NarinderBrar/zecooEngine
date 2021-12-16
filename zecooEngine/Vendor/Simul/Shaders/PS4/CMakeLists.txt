cmake_minimum_required(VERSION 3.14)

file(GLOB SHADERS
	"${CMAKE_SOURCE_DIR}/Shaders/SFX/*.sfx"
)

file(GLOB CORE_SHADER_INCLUDES
	"${CMAKE_SOURCE_DIR}/Platform/Shaders/SL/*.sl"
)

file(GLOB SHADER_INCLUDES
	"${CMAKE_SOURCE_DIR}/Shaders/SL/*.sl"
)

set( PS4_JSON_FILE "${CMAKE_SOURCE_DIR}/Platform/PS4/Render/PSSL/PSSL.json" )

source_group("Shaders" FILES  ${SHADERS} )
source_group("Shader Includes" FILES ${SHADER_INCLUDES} )
source_group("Core Includes" FILES ${CORE_SHADER_INCLUDES} )

if(PLATFORM_PS4)
	#message(ps4 shaders)
	add_sfx_shader_project( PS4Shaders ${PS4_JSON_FILE} INCLUDES "${CMAKE_SOURCE_DIR}/Platform/PS4/Render/PSSL" "${CMAKE_SOURCE_DIR}/Shaders/SL" "${CMAKE_SOURCE_DIR}/Platform/Shaders/SL" "${CMAKE_SOURCE_DIR}/Platform/CrossPlatform/SL"  SOURCES ${SHADERS} ${SHADER_INCLUDES} ${CMAKE} ${SHADER_DEFS} OPTIONS -w OUTPUT "${CMAKE_BINARY_DIR}/Platform/PS4/shaderbin" )

	add_dependencies(SimulRender PS4Shaders)
endif()