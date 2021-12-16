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

set (JSON_FILE "${SIMUL_PLATFORM_DIR}/Commodore/Sfx/Commodore.json")

source_group("Shaders" FILES  ${SHADERS} )
source_group("Shader Includes" FILES ${SHADER_INCLUDES} )
source_group("Core Includes" FILES ${CORE_SHADER_INCLUDES} )

if(PLATFORM_COMMODORE)
	add_sfx_shader_project( CommodoreShaders ${JSON_FILE} INCLUDES "${CMAKE_SOURCE_DIR}/Platform/Commodore/Sfx" "${CMAKE_SOURCE_DIR}/Shaders/SL" "${CMAKE_SOURCE_DIR}/Platform/Shaders/SL" SOURCES ${SHADERS} ${SHADER_INCLUDES} OPTIONS -w OUTPUT "${CMAKE_BINARY_DIR}/Platform/Commodore/shaderbin" DEFINES COMMODORE_SDK_DIR=${COMMODORE_SDK_DIR} )
	 
	add_dependencies(TrueSkyPluginRender CommodoreShaders)
endif()