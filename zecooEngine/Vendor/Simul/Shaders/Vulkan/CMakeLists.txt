cmake_minimum_required(VERSION 3.14)

file(GLOB SHADERS
	"${CMAKE_SOURCE_DIR}/Shaders/SFX/*.sfx"
)

file(GLOB CORE_SHADER_INCLUDES
	"${CMAKE_SOURCE_DIR}/Platform/Shaders/SL/*.sl"
	"${SIMUL_PLATFORM_DIR}/Vulkan/Sfx/*.sl"
)

file(GLOB SHADER_INCLUDES
	"${CMAKE_SOURCE_DIR}/Shaders/SL/*.sl"
)

file(GLOB VULKAN_SHADER_DEFS
	"${CMAKE_SOURCE_DIR}/Platform/Vulkan/Sfx/*.*" 
)
set( VK_JSON_FILE "${CMAKE_SOURCE_DIR}/Platform/Vulkan/Sfx/Vulkan.json" )

source_group("Shaders" FILES  ${SHADERS} )
source_group("Shader Includes" FILES ${SHADER_INCLUDES} )
source_group("Core Includes" FILES ${CORE_SHADER_INCLUDES} )


if(SIMUL_SUPPORT_VULKAN)
	add_sfx_shader_project( VulkanShaders ${VK_JSON_FILE} INCLUDES "${CMAKE_SOURCE_DIR}/Platform/Vulkan/Sfx" "${CMAKE_SOURCE_DIR}/Shaders/SL" "${CMAKE_SOURCE_DIR}/Platform/Shaders/SL" "${CMAKE_SOURCE_DIR}/Platform/CrossPlatform/SL" SOURCES ${SHADERS} ${SHADER_INCLUDES} ${CORE_SHADER_INCLUDES} ${CMAKE} ${VULKAN_SHADER_DEFS} OPTIONS -w DEFINES VULKAN_SDK=${VULKAN_SDK_DIR} OUTPUT "${CMAKE_BINARY_DIR}/Platform/Vulkan/shaderbin" )
endif()
