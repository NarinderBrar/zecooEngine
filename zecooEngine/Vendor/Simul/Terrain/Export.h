#ifndef SIMUL_TERRAIN_EXPORT_H
#define SIMUL_TERRAIN_EXPORT_H

#include "Platform/Core/DebugMemory.h"

#if defined(_MSC_VER)
//  Microsoft
#define SIMUL_EXPORT __declspec(dllexport)
#define SIMUL_IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
//  GCC or Clang
#define SIMUL_EXPORT __attribute__((visibility("default")))
#define SIMUL_IMPORT
#else
//  do nothing and hope for the best?
#define SIMUL_EXPORT
#define SIMUL_IMPORT
#pragma warning Unknown dynamic link import/export semantics.
#endif



#if defined(_MSC_VER) && !defined(SIMUL_TERRAIN_DLL)
	#ifdef _DEBUG
		#ifdef _DLL
			#pragma comment(lib,"SimulTerrain_MDd")
		#else
			#pragma comment(lib,"SimulTerrain_MTd")
		#endif
	#else
		#ifdef _DLL
			#pragma comment(lib,"SimulTerrain_MD")
		#else
			#pragma comment(lib,"SimulTerrain_MT")
		#endif
	#endif
#endif

#if defined(SIMUL_DYNAMIC_LINK) && !defined(DOXYGEN)
	#if !defined(SIMUL_TERRAIN_DLL) 
	// If we're building dll libraries but not in this library IMPORT the classes
		#define SIMUL_TERRAIN_EXPORT SIMUL_IMPORT
	#else
	// In ALL OTHER CASES we EXPORT the classes!
		#define SIMUL_TERRAIN_EXPORT SIMUL_EXPORT
	#endif
#else
	#define SIMUL_TERRAIN_EXPORT
#endif

#define SIMUL_TERRAIN_EXPORT_CLASS class SIMUL_TERRAIN_EXPORT
#define SIMUL_TERRAIN_EXPORT_STRUCT struct SIMUL_TERRAIN_EXPORT
#endif
