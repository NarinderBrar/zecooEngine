#pragma once
#include "Export.h"
#include "Platform/Math/Quaternion.h"
#include "Simul/Base/Referenced.h"
#include "Platform/Core/PropertyMacros.h"
#include "Platform/CrossPlatform/DeviceContext.h"
#include "Platform/Shaders/SL/CppSl.sl"
#include "Simul/Shaders/SL/water_constants.sl"
#include "Simul/Terrain/WaterWaveletsSimulator.h"
#include "Simul/Terrain/WaterParticleSimulator.h"
#include "Simul/Terrain/BoundedWaterObject.h"
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
		class AtmosphericScatteringInterface;
	}
	namespace crossplatform
	{
		class Texture;
	}
	namespace terrain
	{
		enum maskObjectType
		{
			plane,
			cube,
			custom
		};
		//! Verticies for plane masking
		static float planeVertices[] = {
			 0.5f,  0.5f,  0.f,
			 0.5f, -0.5f,  0.f,
			-0.5f, -0.5f,  0.f,
			-0.5f, -0.5f,  0.f,
			-0.5f,  0.5f,  0.f,
			 0.5f,  0.5f,  0.f,

			 -0.0f,  0.0f, -0.0f
		};
		//! Verticies for box masking
		static float boxVertices[] = {
				-0.5f, -0.5f, -0.5f,
				 0.5f, -0.5f, -0.5f,
				 0.5f,  0.5f, -0.5f,
				 0.5f,  0.5f, -0.5f,
				-0.5f,  0.5f, -0.5f,
				-0.5f, -0.5f, -0.5f,

				 0.5f,  0.5f,  0.5f,
				 0.5f, -0.5f,  0.5f,
				-0.5f, -0.5f,  0.5f,
				-0.5f, -0.5f,  0.5f,
				-0.5f,  0.5f,  0.5f,
				 0.5f,  0.5f,  0.5f,

				-0.5f, -0.5f, -0.5f,
				-0.5f,  0.5f, -0.5f,
				-0.5f,  0.5f,  0.5f,
				-0.5f,  0.5f,  0.5f,
				-0.5f, -0.5f,  0.5f,
				-0.5f, -0.5f, -0.5f,

				 0.5f,  0.5f,  0.5f,
				 0.5f,  0.5f, -0.5f,
				 0.5f, -0.5f, -0.5f,
				 0.5f, -0.5f, -0.5f,
				 0.5f, -0.5f,  0.5f,
				 0.5f,  0.5f,  0.5f,

				 0.5f, -0.5f,  0.5f,
				 0.5f, -0.5f, -0.5f,
				-0.5f, -0.5f, -0.5f,
				-0.5f, -0.5f, -0.5f,
				-0.5f, -0.5f,  0.5f,
				 0.5f, -0.5f,  0.5f,

				-0.5f,  0.5f, -0.5f,
				 0.5f,  0.5f, -0.5f,
				 0.5f,  0.5f,  0.5f,
				 0.5f,  0.5f,  0.5f,
				-0.5f,  0.5f,  0.5f,
				-0.5f,  0.5f, -0.5f,

				-0.0f,  0.0f, -0.0f
		};
		//! Struct for holding the beaufort values of a water object
		struct beaufortConstants
		{
			float windSpeed;
			float waveAmplitude;
			float choppyScale;
			float foamStrength;
			float maxWaveLength;
			float minWaveLength;
		};
		//! Values of a water probe
		struct WaterProbeValues
		{
			int ID;
			float radius;
			float dEnergy;
			vec3 location;
			vec3 velocity;
		};
		//! Struct for water probes
		struct waterProbe
		{
			WaterProbeValues values;
			int waterObjectID;
			int UAVID;
			bool active;
		};	
		//! Struct for water buoyancy objects
		struct waterBuoyancyObject
		{
			WaterMeshObjectValues values;
			bool active;
			int bufferSize;
			float* waterBuoyancyObjectOutputs;
			crossplatform::StructuredBuffer<vec3>* positionsBuffer;
			crossplatform::StructuredBuffer<float>* outputsBuffer;
			int callNo;
		};
		//! Struct for water masking objects
		struct waterMaskingObject
		{
			WaterMeshObjectValues values;
			bool active;
			bool totalMask;
			maskObjectType objectType;
		};
		/*
		struct detailMap
		{
			vec3 center;
			float snappingDistance;
			float width;
			bool active;
			crossplatform::Texture* detailMapDisplacementTexture;
		};
		*/
		SIMUL_TERRAIN_EXPORT_CLASS BaseWaterRenderer
		{
			
		public:
			//! Constructor
			BaseWaterRenderer();
			//! Destructor
			virtual ~BaseWaterRenderer();
			//! Platform-dependent function called when initializing the water renderer.
			virtual void RestoreDeviceObjects(crossplatform::RenderPlatform *r);
			//! Platform-dependent function called when uninitializing the water renderer.
			void InvalidateDeviceObjects();
			//! Set the sky and atmospherics interface to allow the renderer to use the correct lighting values.
			void SetBaseSkyInterface(simul::sky::BaseSkyInterface *si, simul::sky::AtmosphericScatteringInterface *ai);
			//! Static function to set the default values for the beaufort scale.
			void fillBeaufortScaleConstants();
			//! Set the water parameters according the beaufort scale.
			void updateBeaufortValues(localWaterValues* valuesToUpdate, float scale);
			//! Set the time.
			void updateTime(float time);
			//! Once per-frame update. Do this before any rendering each frame.
			void PreRenderUpdate(crossplatform::GraphicsDeviceContext &deviceContext,float real_time_seconds);
			//! Main Render function.
			void Render(crossplatform::GraphicsDeviceContext &deviceContext, const crossplatform::Viewport *depthViewports, crossplatform::Texture* depthTexture, crossplatform::Texture* colourTexture, crossplatform::Texture* cubemap, int passNo, int vr = 0);
			//! Platform-dependent function to reload the shaders - only use this for debug purposes.
			void RecompileShaders();
			//! Render debug textures.
			void RenderTextures(crossplatform::GraphicsDeviceContext &deviceContext,int width,int depth);
			//! Visualise the path of flow rays.
			void RenderFlowRays(crossplatform::GraphicsDeviceContext &deviceContext, int width, int height);
			//! Update the ocean surface detail according to how far away from the surface you are.
			void updateBoundlessOceanDensity(float height);
			//! Enable the boundless ocean with a certain maximum detail
			void enableBoundlessOcean(uint layerDensity, uint noOfLayers, int maxDistance);
			//! Disable boundless ocean
			void disableBoundlessOcean();

			//! Create a bounded water object
			void createBoundedWaterObject(uint ID, vec3 dimension, vec3 location);
			//! Add an existing water object to the renderer
			void addBoundedWaterObject(BoundedWaterObject* newWaterObject);
			//! Get a pointer to a bounded water object.
			simul::terrain::BoundedWaterObject* getBoundedWaterObject(uint ID);
			//! Remove a bounded water object
			void removeBoundedWaterObject(uint ID);

			//! Get the water wavelets simulator object
			simul::terrain::WaterWaveletsSimulator* getWaterWaveletsSimulator();

			//! Create a water probe from the given values
			bool addWaterProbe(WaterProbeValues* values);
			//! Update the values of a specific water probe
			void updateWaterProbeValues(WaterProbeValues *values);
			//! Get the results of a water probe
			vec4 getWaterProbeValues(int ID);
			//! Caluculate the water probe results
			void updateWaterProbes(crossplatform::DeviceContext &deviceContext);
			//! Remove a water probe
			void removeWaterProbe(int ID);

			//! Create a water buoyancy object
			bool addWaterBuoyancyObject(WaterMeshObjectValues *newObject);
			//! Update the values of a specific water buoyancy object
			void updateWaterBuoyancyObjectValues(WaterMeshObjectValues *values);
			//! Get the results of a water buoyancy object
			float* getWaterBuoyancyObjectResults(int ID);
			//! Caluculate water buoyancy object vaues
			void updateWaterBuoyancyObjects(crossplatform::DeviceContext &deviceContext);
			//! Remove a water buoyancy object
			void removeWaterBuoyancyObject(int ID);

			//! Create a water masking object
			bool addWaterMaskObject(const waterMaskingObject*newObject);
			//! Update the values of a specific water buoyancy object
			void updateWaterMaskObjectValues(const waterMaskingObject *values);
			//! Remove a water masking object
			void removeWaterMaskObject(int ID);

			//! Set the shore depth texture
			void setShoreDepthTexture(crossplatform::Texture *texture);
			//! Get the shore texture paramaters
			shoreTexture* getShoreTextureParams();

			//Global water values
			globalWaterValues			*waterGlobals;
			bool boundlessOceanEnabled;
			bool debugFlowRays;

			//! Get Boundless ocean properties
			boundlessOceanProperties* getBoundlessOceanProperties();
			//! Get a pointer to the wave grid used by the boundless ocean
			WaveGrid* getBoundlessWaveGrid();
			//! Get a pointer to the particle generator
			WaterParticleSimulator* getWaterParticleSimulator();

			//! Get the enums of water properties. Used mainly for plugin interfacing.
			static long long GetEnum(const char *n);

		protected:
			simul::sky::BaseSkyInterface	*skyInterface;
			simul::sky::AtmosphericScatteringInterface *atmosphericInterface;
			crossplatform::RenderPlatform	*renderPlatform;
			vec3 cam_pos;
			
			void FillPerCallConstants(crossplatform::GraphicsDeviceContext &deviceContext);
			simul::terrain::WaterWaveletsSimulator			*waterWaveletsSimulator;
			beaufortConstants beaufortStruct[5];

			simul::terrain::WaterParticleSimulator* waterParticleSimulator;

			// HLSL shaders
			crossplatform::Effect		*effect;

			// Distant perlin wave
			crossplatform::Texture		*perlinNoise;

			// Constant Buffers
			crossplatform::ConstantBuffer<cbShading>		shadingConstants;
			crossplatform::ConstantBuffer<cbChangePerCall>	changePerCallConstants;
			//crossplatform::ConstantBuffer<OsdConstants>		osdConstants;
			crossplatform::ConstantBuffer<cbWaterProbe>		waterProbeConstants;
			//crossplatform::ConstantBuffer<cbWaterFoam>	waterFoamConstants;
			crossplatform::ConstantBuffer<cbMeshObjectBuffers> waterMeshConstants;

			//Boundless Ocean
			boundlessOceanProperties *boundlessOcean;

			//Bounded water volumes
			std::vector<BoundedWaterObject*> boundedWaterObjects;

			//Stencil to maintain depth
			crossplatform::Texture *waterStencil;
            crossplatform::Texture *waterMainDepthBuffer;

			//Mask to render blocking objects to
			crossplatform::Texture *waterFrontfaceMask;
			crossplatform::Texture *waterMask;

			// Textures to output water values to
			crossplatform::Texture *waterDepth;
			crossplatform::Texture *waterScattering;//4th value Salinity
			crossplatform::Texture *waterAbsorption;//4th value Temperature
			crossplatform::Texture *waterNormals; //Render target to handle water normals
			crossplatform::Texture *waterRefractColour;//Render target to handle the colour sampled from the render target

			// Seperate texture to output reflection values to.
			crossplatform::Texture *waterReflectColour;
			
			crossplatform::ShaderResource	profileRes;

			//Foam Textures
			crossplatform::Texture *foamTexture;
			crossplatform::Texture *surfaceFoamTexture;
			//crossplatform::Texture *boundlessFoamAlpha;

			//Depth textures for shoreline effects
			shoreTexture *shoreDepthTexture;

			//Detail map texture
			//detailMap *detailMapTexture;

			//Water Probes
			std::vector<simul::terrain::waterProbe*> waterProbes;
			vec4* boundlessWaterProbePositions;
			vec4* boundlessWaterProbeVelocities;
			vec4* boundlessWaterProbeOutputs;
			int callNo;
			crossplatform::StructuredBuffer<vec4>* waterProbePositionsBuffer;
			crossplatform::StructuredBuffer<vec4>* waterProbeOutputsBuffer;

			// Water Buoyancy Objects
			std::vector<simul::terrain::waterBuoyancyObject*> waterBuoyancyObjects;
			float* waterBuoyancyObjectOutputs;

			// Water Mask Objects
			std::vector<simul::terrain::waterMaskingObject*> waterMaskingObjects;

			// Create perlin noise texture for far-sight rendering
			//void loadTextures();

		};
	}
}
#ifdef _MSC_VER
#pragma warning(pop)
#endif
