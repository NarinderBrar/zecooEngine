#pragma once
#ifndef _WATER_WAVELETS_H
#define _WATER_WAVELETS_H

#include "Simul/Terrain/Export.h"
#include "Platform/Shaders/SL/CppSl.sl"
#include "Simul/Shaders/SL/water_constants.sl"
#include "Platform/CrossPlatform/Texture.h"
#include "Platform/CrossPlatform/Effect.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4251)
#endif

namespace simul
{
	namespace terrain
	{
		//! Struct for holding Global water values that affect all water objects and the boundless ocean
		struct globalWaterValues
		{
			int mapSize;
			float scale;
			float timeScale;
			float time;
			bool useGameTime;
			bool enableFoam;
			bool enableReflection;
			bool enableParticles;
			int noOfReflectionSteps;
			int pixelStep;
			int reflectionDistance;
			bool fullResolutionReflections;
			bool reverseFace;
			bool fullResolution;
		};
		//! Struct for holding values for individual water objects and the boundless ocean
		struct localWaterValues
		{
			float beaufortScale; //< Scales the water surface to the observed conditions defined by the beaufort scale. Overrides several other values
			float windSpeed; //< Wind speed 
			float windDirection; //< Wind Direction
			float windDependency; //< Dependancy of the wind on the direction surface waves travel in.
			float waveAmplitude; //< Amplitude of the wave
			float foamStrength; //< Strength of the foam effect
			vec3 scattering; //< Scattering coefficient for water opacity
			vec3 absorption; //< Absorption coefficient for water colour
			bool render; //< Whether to render the water object or not
			bool enableWaveGrid; //< Enable wave grid effects of the water surface (Boundless surfaces only)
			bool resetWaveGrid; //< Boolean to set if the wave grid is to be reset
			int waveGridDistance; //< Distance to calculate wave grid values
			int passNo;
			int probeCallNo;
			bool refraction; //< Apply refraction to the surface
			float maxWaveLength; //< Maximum length of the generated waves
			float minWaveLength; //< Minimum length of the generated waves
			int profileBufferResolution; //< Resolution of the profile buffers
			bool useCustomMesh; //< Use a custom mesh for the water surface (Bounded objects only)
		};
		//! Struct for holding the shore texture values
		struct shoreTexture
		{
			vec3 location;
			float extent;
			float width;
			bool active;
			crossplatform::Texture* depthTexture;
		};
		//! Struct for defining a wavegrid
		struct WaveGrid
		{
			uint2 dimension;
			vec2 location;
			int angleDimension;
			int waveNumberDimension;
			
			bool valuesUpdated;
			bool initialised;
			bool usePrecomputedWaveGrid;
			float lastTime;

			vec2* waveSpectrum;

			simul::crossplatform::Texture* profileBuffers;
			simul::crossplatform::StructuredBuffer<float> groupSpeeds;
			std::vector<simul::crossplatform::Texture*> amplitudeGrid;
		};
		//! Struct for holding a specific Flow Ray buffer
		struct surfaceFlowRayBuffer
		{
			int ID;
			int depth;
			uint2 dimension;
			float baseAmplitude;

			simul::crossplatform::Texture* rayPathBuffer;
		};
		//! Struct for holding the Boundless Ocean properties
		struct boundlessOceanProperties
		{
			uint baseLayerDensity;
			uint baseNoOfLayers;
			uint layerDensity;
			uint noOfLayers;
			uint verticiesPerLayer;
			uint totalVertices;
			int maxDistance;
			float minQuadSize;
			vec3 center;
			localWaterValues waterValues;
			bool enableWaveGrid;
			WaveGrid* waveGrid;
		};
		SIMUL_TERRAIN_EXPORT_CLASS WaterWaveletsSimulator
		{
			public:
				WaterWaveletsSimulator(simul::terrain::globalWaterValues *s);
				~WaterWaveletsSimulator();

				//! Platform-dependent function called when initializing the wavelets simulator.
				void RestoreDeviceObjects(crossplatform::RenderPlatform *renderPlatform);
				//! Platform-dependent function called when uninitializing the wavelets simulator.
				void InvalidateDeviceObjects();

				//! Set the precompiled shader used by the wavelets simulator - debug only
				void SetShader(crossplatform::Effect *e);

				//! Initialise a given wave grid
				void InitialiseWaveGrid(WaveGrid* newWaveGrid, uint2 dimension, int theta, int k);
				//! Set the target wave grid the simulator will be working on.
				void SetTargetWaveGrid(WaveGrid* targetWaveGrid, localWaterValues* waterValues);

				//! Clear the wave amplitude grid of the target wave grid
				void ResetAmplitudeGrid(crossplatform::DeviceContext &deviceContext);
				//! Update the wave amplitude grid of the target wave grid
				void UpdateAmplitudeGrid(crossplatform::DeviceContext &deviceContext, float real_time_seconds, vec4* waterProbePositions, vec4* waterProbeVelocities, vec3 center, vec2 camPos);

				//! Precompute the profile buffers of a given wave grid
				void PrecomputeProfileBuffers(crossplatform::GraphicsDeviceContext &deviceContext, WaveGrid* targetWaveGrid, float real_time_seconds, localWaterValues* waterValues);
				//! Precompute the wave speeds of the target wave grid
				void PrecomputeWaveSpeeds(crossplatform::DeviceContext &deviceContext);

				//! Get a pointer to the contour map
				simul::crossplatform::Texture* getContourMap();
				//! Get a pointer to the base flow ray buffer
				surfaceFlowRayBuffer* getBaseFlowRayBuffer();
				//! Get a pointer to the sub flow ray buffer
				surfaceFlowRayBuffer* getSubFlowRayBuffer();

				//! Generate a contour map that stores the normal of the shore surface
				void GenerateContourMap(crossplatform::GraphicsDeviceContext &deviceContext, simul::terrain::shoreTexture *shoreDepth, float oceanHeight);
				//! Calculate the path of the flow rays
				void GenerateSurfaceFlow(crossplatform::DeviceContext &deviceContext, WaveGrid* surfaceFlowGrid);

			private:
				WaveGrid* targetWaveGrid;
				std::vector<simul::crossplatform::Texture*> internalAmplitudeGrid;

				simul::crossplatform::Texture* contourMap;
				surfaceFlowRayBuffer* baseFlowRayBuffer;
				surfaceFlowRayBuffer* subFlowRayBuffer;

				crossplatform::StructuredBuffer<vec2>* waveSpectrumBuffer;
				crossplatform::StructuredBuffer<vec4>* waterProbePositionsBuffer;
				crossplatform::StructuredBuffer<vec4>* waterProbeVelocitiesBuffer;

				crossplatform::RenderPlatform *renderPlatform;
				crossplatform::Effect *effect;

				crossplatform::ConstantBuffer<cbProfileBuffers>		profileBufferConstants;
				crossplatform::ConstantBuffer<cbWaterProbe>			waterProbeConstants;
				crossplatform::ConstantBuffer<cbContourBuffer>		contourConstants;
				crossplatform::ConstantBuffer<cbFlowRayBuffer>		flowRayConstants;

				globalWaterValues* waterGlobals;
				localWaterValues* currentWaterValues;

				surfaceFlowRayBuffer* createSurfaceFlowRayBuffer(int ID, float baseAmplitude, uint2 dimension);
		};
	}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // _WATER_WAVELETS_H