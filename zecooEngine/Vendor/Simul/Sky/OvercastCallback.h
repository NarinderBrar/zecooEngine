#ifndef SIMUL_OVERCASTCALLBACK_H
#define SIMUL_OVERCASTCALLBACK_H

#include "Simul/Sky/Export.h"
#include "Simul/Sky/SkyInterface.h"

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4251)
#endif

namespace simul
{
	namespace sky
	{
		//! A virtual callback class for skies to get overcast information from cloud classes.
		class OvercastCallback
		{
		public:
			//! For the specified time, get how overcast the sky will be, where the transition starts (e.g. the cloudbase), and what altitude it occurs over (e.g. how tall the clouds are).
			virtual OvercastStruct GetOvercastData(double time) const=0;
		};
	}
}
#ifdef _MSC_VER
	#pragma warning(pop)
#endif
#endif