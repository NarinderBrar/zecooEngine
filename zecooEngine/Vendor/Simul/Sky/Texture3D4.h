#ifndef SIMULSKY_TEXTURE3D4_H
#define SIMULSKY_TEXTURE3D4_H
#include "Simul/Sky/Float4.h"
#include "Simul/Sky/Export.h"
#include "Platform/Core/MemoryInterface.h"
#include "Platform/Shaders/SL/CppSl.sl"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4324)
#else
#define CACHE_ALIGN
#endif
namespace simul
{
	namespace sky
	{
		class Texture2D4;
		/// A CPU 3D texture.
		SIMUL_SKY_EXPORT_CLASS Texture3D4
		{
			simul::sky::float4 *tex;
			simul::sky::float4 Edge;
			int X;
			int Y;
			int Z;
			bool Wrap;
			bool Border;
			int texsize;
			mutable unsigned checksum;
			bool stretch_to_edges;
			simul::base::MemoryInterface *memoryInterface;
			void CalcChecksum() const;
		public:
			Texture3D4(simul::base::MemoryInterface *mem=NULL,bool stretch_to_edges=false);
			~Texture3D4();
			//! Returns a checksum that changes whenever the texture is modified.
			unsigned GetChecksum() const;
			void setMemoryInterface(simul::base::MemoryInterface *mem);
			const Texture3D4 &operator=(const Texture3D4 &t);
			void GetSizes(int &x,int &y,int &z) const
			{
				x=X;
				y=Y;
				z=Z;
			}
			int3 GetSize() const
			{
				return int3(X, Y, Z);
			}
			simul::sky::float4 Get(float u,float v,float w) const;
			void SetValue(int x,int y,int z,simul::sky::float4 clr);
			void SetLayer(int z,const Texture2D4 *tex);
			simul::sky::float4 *Setup(int x,int y,int z,bool wrap,bool border,simul::sky::float4 edge=simul::sky::float4(0.f,0.f,0.f,0.f));
			simul::sky::float4 *GetDataPointer();
			const simul::sky::float4 *GetDataPointer() const;
			simul::sky::float4 *GetLayerPointer(int z);
			const simul::sky::float4 *GetLayerPointer(int z) const;
			void Zero();
		};
	}
}
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif
