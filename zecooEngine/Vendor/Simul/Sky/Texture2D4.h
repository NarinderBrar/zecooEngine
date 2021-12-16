#ifndef SIMULSKY_TEXTURE2D4_H
#define SIMULSKY_TEXTURE2D4_H

#include "Simul/Sky/Float4.h"
#include "Simul/Sky/Export.h"
#include "Platform/Core/MemoryInterface.h"

#ifdef _MSC_VER
#define CACHE_LINE 16
#define CACHE_ALIGN __declspec(align(CACHE_LINE))
#pragma warning(push)
#pragma warning(disable:4324)
#else
#define CACHE_ALIGN
#endif

namespace simul
{
	namespace sky
	{
		SIMUL_SKY_EXPORT_CLASS CACHE_ALIGN Texture2D4
		{
			simul::sky::float4 *tex;
			CACHE_ALIGN simul::sky::float4 Edge;
			int X;
			int Y;
			bool Wrap;
			bool Border;
			int texsize;
			simul::base::MemoryInterface *memoryInterface;
		public:
			Texture2D4(simul::base::MemoryInterface *mem=NULL);
			~Texture2D4();
			void setMemoryInterface(simul::base::MemoryInterface *mem);
			const Texture2D4 &operator=(const Texture2D4 &t);
			simul::sky::float4 Get(float u,float v) const;
			simul::sky::float4 *Setup(int x,int y,bool wrap,bool border,simul::sky::float4 edge=simul::sky::float4(0.f,0.f,0.f,0.f));
			simul::sky::float4 *GetDataPointer();
			void Zero();
		};
	}
}
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif
