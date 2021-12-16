#pragma once

#ifndef _WATER_PARTICLES_H
#define _WATER_PARTICLES_H

#include "Simul/Terrain/Export.h"

#include "WaterWaveletsSimulator.h"
#include "Platform/CrossPlatform/RenderPlatform.h"
#include "Platform/CrossPlatform/Texture.h"
#include "Platform/CrossPlatform/Effect.h"
#include "Platform/Shaders/SL/CppSl.sl"
#include "Simul/Shaders/SL/water_particles_constants.sl"
#include "Platform/CrossPlatform/Layout.h"
#include "Platform/CrossPlatform/Buffer.h"
#include "Platform/CrossPlatform/Macros.h"

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4251)
#endif

namespace simul
{
	namespace terrain
	{
		enum class particleGeneratorType
		{
			point,
			planeSource,
			planeDynamic
		};

		struct particleGeneratorValues
		{
			int ID;
			bool active;

			vec3 location;
			vec3 scale;
			math::Quaternion rotation;
		};

		struct particleGenerator
		{
			particleGeneratorValues values;
			particleGeneratorType type;
			crossplatform::Texture *planeGenerationTexture;
			vec3 previousLocation;
		};

		SIMUL_TERRAIN_EXPORT_CLASS WaterParticleSimulator
		{
			public:
				WaterParticleSimulator();
				~WaterParticleSimulator();

				//! Platform-dependent function called when initializing the wavelets simulator.
				void RestoreDeviceObjects(crossplatform::RenderPlatform* renderPlatform);
				//! Platform-dependent function called when uninitializing the wavelets simulator.
				void InvalidateDeviceObjects();

				//! Platform-dependent function to reload the shaders - only use this for debug purposes.
				void RecompileShaders();

				void PreRenderUpdate(crossplatform::GraphicsDeviceContext& deviceContext, boundlessOceanProperties* oceanProperties, float time);

				//! Create a particle generator
				bool AddWaterParticleGenerator(const particleGeneratorValues* newGenerator, const particleGeneratorType newGeneratorType, crossplatform::Texture* customPlaneTexture);
				//! Update the values of a specific water particle generator
				void UpdateWaterParticleGeneratorValues(const particleGeneratorValues* generator, const particleGeneratorType generatorType, crossplatform::Texture* customPlaneTexture);
				//! Remove a particle generator
				void RemoveWaterParticleGenerator(int ID);

				void SpawnParticles(crossplatform::DeviceContext& deviceContext, int num, float time);
				void ProcessParticleGrid(WaveGrid* inputWaveGrid);

				void ComputeParticles(crossplatform::DeviceContext& deviceContext, float time);
				void RenderParticles(crossplatform::GraphicsDeviceContext& deviceContext);

				//Delete after debugging
				float timeSinceLastSpawn;

			private:
				float previousTime;
				int maxParticles;

				crossplatform::RenderPlatform* renderPlatform;
				// HLSL shaders
				crossplatform::Effect* effect;

				crossplatform::ConstantBuffer<cbParticleRenderConstants> particleRenderConstants;
				crossplatform::ConstantBuffer<cbParticleGenerationConstants> particleGenerationConstants;
				crossplatform::ConstantBuffer<cbParticleUpdateConstants> particleUpdateConstants;
				crossplatform::ConstantBuffer<cbParticleGenerationPlaneConstants> particleGenerationPlaneConstants;

				crossplatform::StructuredBuffer<particleData>* waveParticleBuffer;
				crossplatform::StructuredBuffer<particleData>* sprayParticleBuffer;

				//Particle Generators
				std::vector<particleGenerator*> particleGenerators;
		};
	}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif