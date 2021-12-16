#ifndef SIMUL_TERRAIN_MATERIALINTERFACE_H
#define SIMUL_TERRAIN_MATERIALINTERFACE_H

#include "Simul/Terrain/Export.h"
#include "Platform/Core/PropertyMacros.h"

namespace simul
{
	namespace terrain
	{
		SIMUL_TERRAIN_EXPORT_CLASS MaterialInterface
		{
		public:
			virtual int GetMaterialAt(int x,int y) const=0;
			virtual int GetNumMaterials() const=0;
			virtual const char *GetMaterialName(int mat_num) const=0;
		};
	}
}
#endif

