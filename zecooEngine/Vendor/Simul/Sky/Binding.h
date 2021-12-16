#ifndef SIMUL_SKY_BINDING_H
#define SIMUL_SKY_BINDING_H
#include "Simul/Sky/Export.h"

namespace simul
{
	namespace sky
	{
		struct float4;
	}
}

SIMUL_SKY_EXPORT const char *ConvertToString(const simul::sky::float4 *V);
SIMUL_SKY_EXPORT void ConvertFromString(simul::sky::float4 *V,const char *str);
SIMUL_SKY_EXPORT const simul::sky::float4 &ConstFromString(const char *str);
#endif
