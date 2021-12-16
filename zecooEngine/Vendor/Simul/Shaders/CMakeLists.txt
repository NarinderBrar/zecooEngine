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

source_group("Shaders" FILES  ${SHADERS} )
source_group("Shader Includes" FILES ${SHADER_INCLUDES} )
source_group("Core Includes" FILES ${CORE_SHADER_INCLUDES} )

add_subdirectory(DirectX11)
add_subdirectory(DirectX12)
add_subdirectory(OpenGL)
add_subdirectory(Vulkan)
add_subdirectory(PS4)
add_subdirectory(Commodore)