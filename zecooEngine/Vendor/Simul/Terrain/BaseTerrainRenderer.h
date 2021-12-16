#pragma once
#include "Export.h"
#include "Simul/Base/Referenced.h"
#include "Platform/Core/PropertyMacros.h"
#include "Simul/Terrain/HeightMapNode.h"
#include "Platform/CrossPlatform/RenderPlatform.h"
#include "Platform/CrossPlatform/DeviceContext.h"
#include "Platform/CrossPlatform/Effect.h"
#include "Platform/Shaders/SL/CppSl.sl"
#include "Simul/Shaders/SL/simul_terrain_constants.sl"
#include "Simul/Shaders/SL/cloud_constants.sl"
#include <vector>
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4251)
#endif

namespace simul
{
	namespace sky
	{
		class BaseSkyInterface;
	}
	namespace sky
	{
		class BaseSkyInterface;
	}
	namespace terrain
	{
		struct LightningIllumination
		{
			vec3 centre;
			float brightness;
			vec3 colour;
		};
		SIMUL_TERRAIN_EXPORT_CLASS BaseTerrainRenderer
		{
		public:
			BaseTerrainRenderer(simul::base::MemoryInterface *m);
			virtual ~BaseTerrainRenderer();
			
			META_BeginProperties
				META_Property(bool	,ThermalErosion	,"Whether to apply thermal erosion per-frame.")
				META_Property(bool	,WaterErosion	,"Whether to apply hydraulic erosion per-frame.")
			META_EndProperties
			
			void SetBaseSkyInterface(simul::sky::BaseSkyInterface *s)
			{
				baseSkyInterface=s;
			}
			void SetLightningProperties(const LightningIllumination &p);
			void SetLighting(const vec3 &sunlight,const vec3 &moonlight,const vec3 &ambient);
			virtual void RestoreDeviceObjects(crossplatform::RenderPlatform *r);
			virtual void InvalidateDeviceObjects();
			virtual void RecompileShaders();
			virtual void Render(crossplatform::GraphicsDeviceContext &deviceContext,float exposure);
			simul::terrain::HeightMapInterface *GetHeightMapInterface()
			{
				return heightMapInterface;
			}
			simul::terrain::HeightMapNode *GetHeightMap()
			{
				return heightmap;
			}
			virtual void SetCloudShadowTexture(const CloudShadowStruct &c)
			{
 				cloudShadowStruct=c;
			}
			vec3 GetOffset() const
			{
				return offset;
			}
			void SetOffset(vec3 o)
			{
				offset=o;
			}
			void MakeTextures(crossplatform::GraphicsDeviceContext &deviceContext);
		protected:
			crossplatform::RenderPlatform		*renderPlatform;
			simul::base::MemoryInterface		*memoryInterface;
			simul::terrain::HeightMapNode		*heightmap;
			simul::terrain::HeightMapInterface	*heightMapInterface;
			simul::sky::BaseSkyInterface		*baseSkyInterface;
			CloudShadowStruct					cloudShadowStruct;
			crossplatform::Effect				*effect;
			crossplatform::Buffer*				vertexBuffer;
			crossplatform::Layout*				layout;
			crossplatform::EffectTechnique*		m_pTechnique;
			crossplatform::ConstantBuffer<TerrainConstants>	terrainConstants;
			LightningIllumination lightningIllumination;
			crossplatform::Texture *texArray;
			int numVertices;
			void MakeVertexBuffer();
			vec3 sunlight;
			vec3 moonlight;
			vec3 ambient;
			vec3 offset;
		};
	}
}
