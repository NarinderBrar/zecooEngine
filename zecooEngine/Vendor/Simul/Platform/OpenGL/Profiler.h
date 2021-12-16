#pragma once
#include "Platform/OpenGL/Export.h"
#include "Platform/Core/Timer.h"
#include "Platform/CrossPlatform/GpuProfiler.h"

#pragma warning(disable:4251)

namespace simul
{
	namespace opengl
	{
		struct ProfilingQuery
		{
			ProfilingQuery() :timestamp(NULL) {}
			~ProfilingQuery() {}
			unsigned long* timestamp;
		};

		class SIMUL_OPENGL_EXPORT Profiler: public simul::crossplatform::GpuProfiler
		{
		public:
			static Profiler&	GetGlobalProfiler();
								~Profiler();

			void				Begin(crossplatform::DeviceContext& deviceContext,const char* name);
			void				End(crossplatform::DeviceContext& deviceContext);

			void				StartFrame(crossplatform::DeviceContext& deviceContext);
			void				EndFrame(crossplatform::DeviceContext& deviceContext);
		};
	}
}