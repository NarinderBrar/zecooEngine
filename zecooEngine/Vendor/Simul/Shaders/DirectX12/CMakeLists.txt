cmake_minimum_required(VERSION 3.14)

file(GLOB SHADERS
	"${CMAKE_SOURCE_DIR}/Shaders/SFX/*.sfx"
)

file(GLOB CORE_SHADER_INCLUDES
	"${CMAKE_SOURCE_DIR}/Platform/Shaders/SL/*.sl"
	"${SIMUL_PLATFORM_DIR}/DirectX12/HLSL/*.sl"
)

file(GLOB SHADER_INCLUDES
	"${CMAKE_SOURCE_DIR}/Shaders/SL/*.sl"
)

#can't include this in SOURCES as Visual Studio will try to build GFX.hlsl with Fxc.
file(GLOB SHADER_DEFS
	"${CMAKE_SOURCE_DIR}/Platform/DirectX12/HLSL/*.*" 
)

if(XBOXONE)
	set( DX12_JSON_FILE "${SIMUL_PLATFORM_DIR}/XboxOne/HLSL/HLSL12X.json" )
	set( XDK_OPT -ESIMUL_XDK_VERSION=${XDK_TOOLCHAIN_VERSION})
elseif(PLATFORM_SPECTRUM)
	set( DX12_JSON_FILE "${SIMUL_PLATFORM_DIR}/Spectrum/Sfx/Spectrum.json" )
elseif(GDK AND NOT PLATFORM_WINGDK)
	set( DX12_JSON_FILE "${SIMUL_PLATFORM_DIR}/XboxOne/HLSL/HLSL12GDK.json" )
	set( XDK_OPT -ESIMUL_XDK_VERSION=${XDK_TOOLCHAIN_VERSION})
else()
	set( DX12_JSON_FILE "${SIMUL_PLATFORM_DIR}/DirectX12/Sfx/DirectX12.json" )
	set( XDK_OPT "")
endif()

source_group("Shaders" FILES  ${SHADERS} )
source_group("Shader Includes" FILES ${SHADER_INCLUDES} )
source_group("Core Includes" FILES ${CORE_SHADER_INCLUDES} )

if(SIMUL_SUPPORT_D3D12)
	add_sfx_shader_project( DirectX12Shaders ${DX12_JSON_FILE} INCLUDES "${CMAKE_SOURCE_DIR}/Platform/DirectX12/Sfx" "${CMAKE_SOURCE_DIR}/Shaders/SL" "${CMAKE_SOURCE_DIR}/Platform/Shaders/SL" "${CMAKE_SOURCE_DIR}/Platform/CrossPlatform/SL" SOURCES  ${SHADERS} ${SHADER_INCLUDES} ${CORE_SHADER_INCLUDES} OPTIONS -w ${XDK_OPT} OUTPUT "${CMAKE_BINARY_DIR}/Platform/DirectX12/shaderbin"  INTERMEDIATE "${CMAKE_BINARY_DIR}/Platform/DirectX12/sfx_intermediate" DEFINES SIMUL_XDK_VERSION=${XDK_TOOLCHAIN_VERSION} GDK_BIN_PATH=${GDK_BIN_PATH})
endif()