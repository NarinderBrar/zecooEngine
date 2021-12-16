#pragma once

#if defined(_MSC_VER)
#pragma warning(disable : 4251) //Disables '<typeid> needs to have dll-interface to be used by clients of class <_class>' warning C4251
#endif

#include "Simul/Platform/Math/Pi.h"
#include "Simul/Platform/CrossPlatform/DeviceContext.h"
#include "Simul/Platform/CrossPlatform/RenderPlatform.h"
#include "Simul/Platform/CrossPlatform/Effect.h"
#include "Simul/Sky/Export.h"
#include "Simul/Sky/SkyKeyframer.h"

#include "Simul/Shaders/SL/aurora_constants.sl"

#include <map>
#include <vector>
#include <algorithm>

#define SIMUL_H_F 6.62607015e-34f
#define SIMUL_C_F 299792458.f

namespace simul
{
	namespace sky
	{
		struct AuroralGlobalCoverage
		{
			float highestLatitude;		//! Highest Latitude Aurora can occur, in degrees decimal centred about the geomagnetic north pole.
			float lowestLatitide;		//! Lowest Latitude Aurora can occur, in degrees decimal centred about the geomagnetic north pole.
			float maxAuroralBand;		//! Arc Thickness of the thicknest part of the auroral oval, in degrees decimal.
			float minAuroralBand;		//! Arc Thickness of the thinnest part of the auroral oval, in degrees decimal.

			AuroralGlobalCoverage()
				:highestLatitude(80.0f), lowestLatitide(60.0f), 
				maxAuroralBand(10.0f), minAuroralBand(3.0f) {};
		};
		
		SIMUL_SKY_EXPORT_CLASS Aurora
		{
		private:
			//! Atomic Oxygen Auroral Spectral Emissions in nm.
			const float aoase_red = 630.0f, aoase_green = 557.7f;
			//! Atomic Nitrogen Auroral Spectral Emissions in nm.
			const float anase_cyan = 471.0f;
			//! Diatomic Nitrogen Auroral Spectral Emissions in nm.
			const float dnase_red = 670.0f;
			//! Ionized Diatomic Nitrogen Auroral Spectral Emissions in nm.
			const float idnase_blue = 427.8f, idnase_purple = 391.4f;

			//! Layers within the Aurora
			std::vector<AuroralLayer> m_AuroralLayers;
			//! Intensity of the Auroral Layers
			float m_AuroralLayerIntensity = 1.0f;
			//! Intensity of the Auroral Layers
			uint32_t m_AuroraIntensityMapSize = 1024;
			//! Orientation of CloudWindow
			crossplatform::Quaterniond m_CloudWindowOrientation;
			//! Orientation of Geomagnetic North Pole
			crossplatform::Quaterniond m_GeomagneticNorthPole;
			//! SkyKeyframer for reference
			SkyKeyframer* m_pSkyKeyframer;
			//!	Dirction to the Sun, based on planetary scale.
			vec4 m_GlobalDirectionToSun;
			//! Position of the Aurora Oval about the Geomagnetic North Pole
			AuroralGlobalCoverage m_AuroralGlobalCoverage;
			//! Field Aligned Current checksum
			uint64_t m_FACchecksum;

			uint32_t m_IntensityMapDrawNumber = 0;
			bool m_DrawIntensityMap = false;

			crossplatform::RenderPlatform* m_RenderPlatform = nullptr;
			crossplatform::Effect* m_Effect = nullptr;
			crossplatform::ConstantBuffer<AuroraConstants> m_AuroraConstants;
			crossplatform::ConstantBuffer<AuroraPerViewConstants> m_AuroraPerViewConstants;

			crossplatform::ShaderResource m_SR_FACmap;
			crossplatform::ShaderResource m_SR_IntensityMap;
			crossplatform::ShaderResource m_SR_RWIntensityMap;
			crossplatform::ShaderResource m_SR_AuroralColours;
			crossplatform::ShaderResource m_SR_SB_AuroralLayers;
			crossplatform::ShaderResource m_SR_DepthTexture;
			crossplatform::ShaderResource m_SR_DepthTextureMS;
		
			crossplatform::Texture* m_FACmap = nullptr;
			crossplatform::Texture* m_IntensityMap = nullptr;
			crossplatform::Texture* m_AuroralColours = nullptr;
			crossplatform::StructuredBuffer<AuroralLayer> m_SB_AuroralLayers;

		public:
			bool m_ShowAuroralOvalInCloudWindow = true;

		public:
			///Default class methods

			Aurora(SkyKeyframer* pSkyKeyframer);
			virtual ~Aurora();
			void RestoreDeviceObjects(crossplatform::RenderPlatform* r);
			void InvalidateDeviceObjects();
			void RecompileShaders();
			void EnsureTextures();

			///Get and Set Auroral Global Coverage

			inline void SetDefaultAuroralGlobalCoverage() { m_AuroralGlobalCoverage = AuroralGlobalCoverage(); };
			inline void SetAuroralGlobalCoverage(const AuroralGlobalCoverage& auroralGlobalCoverage) { m_AuroralGlobalCoverage = auroralGlobalCoverage; };
			inline const AuroralGlobalCoverage& GetAuroralGlobalCoverage() const { return m_AuroralGlobalCoverage;}
			inline AuroralGlobalCoverage& GetAuroralGlobalCoverage() { return m_AuroralGlobalCoverage;}

			///Get and Set Auroral Layers

			void SetDefaultAuroralLayers();
			void SetAuroralLayers(const std::vector<AuroralLayer>& auroralLayers);
			inline const std::vector<AuroralLayer>& GetAuroralLayers() const { return m_AuroralLayers; }
			inline std::vector<AuroralLayer>& GetAuroralLayers() { return m_AuroralLayers; }

			///Get and Set Auroral Layers Intensity

			inline void SetAuroralLayerIntensity(float value) { m_AuroralLayerIntensity = value; }
			inline const float& GetAuroralLayerIntensity() const  { return m_AuroralLayerIntensity; }
			inline float GetAuroralLayerIntensity() { return m_AuroralLayerIntensity; }

			///Get and Set Aurora Intensity Map Size

			inline void SetAuroraIntensityMapSize(uint32_t value) { m_AuroraIntensityMapSize = value; }
			inline const uint32_t& GetAuroraIntensityMapSize() const { return m_AuroraIntensityMapSize; }
			inline uint32_t GetAuroraIntensityMapSize() { return m_AuroraIntensityMapSize; }

			///Get and Set Cloud Window Orientation

			inline void SetCloudWindowOrientation(const crossplatform::Quaterniond& q) { m_CloudWindowOrientation = q; }
			inline const crossplatform::Quaterniond& GetCloudWindowOrientation() const { return m_CloudWindowOrientation; }

			///Get and Set Geomagnetic North Pole

			inline void SetGeomagneticNorthPole(const crossplatform::Quaterniond& q) { m_GeomagneticNorthPole = q; }
			inline const crossplatform::Quaterniond& GetGeomagneticNorthPole() const { return m_GeomagneticNorthPole; }

			///Others

			void SetDefaultFACs();
			const vec4& GetGlobalDirectionToSun();
			crossplatform::ConstantBuffer<AuroraConstants>& GetAuroraConstants() { return m_AuroraConstants; }
			crossplatform::Texture* GetIntensityMap() const { return m_IntensityMap; }

			///Rendering

			bool CheckRenderOncePerFrame(crossplatform::GraphicsDeviceContext& deviceContext); //false means that Render() has bee called more than once.
			void PreRenderUpdate(crossplatform::GraphicsDeviceContext& deviceContext);
			void Render(crossplatform::GraphicsDeviceContext& deviceContext, const crossplatform::ViewStruct& viewStruct, crossplatform::Texture* depthTexture, vec4 viewportTextureRegionXYWH, float exposure);
			void ShowAuroraTextures(crossplatform::GraphicsDeviceContext& deviceContext, int x0, int y0, int dx, int dy);

			///Conversion functions

			static float RadianceToRayleigh(float L, float wavelength) //L = W * sr-1 * m-2
			{
				float Phi_e = 4.0f * SIMUL_PI_F * L; //W where 1(m-2)
				float photonicPower = (SIMUL_H_F * SIMUL_C_F) / (1e9f * wavelength) / 1.0f; //W
				float photonCount = Phi_e / photonicPower;
				return photonCount / 1e10f; //1e10 Photons, where 1(s-1) * 1(m-2)
			}
			static float RayleighToRadiance(float I, float wavelength) //I = 1e10 Photons * s-1 * m-2 
			{
				float photonCount = I * 1e10f; //Where 1(s-1) * 1(m-2)
				float photonicPower = (SIMUL_H_F * SIMUL_C_F) / (1e9f * wavelength) / 1.0f; //W
				float Phi_e = photonCount * photonicPower; //W where 1(m-2)
				return Phi_e / (4 * SIMUL_PI_F); //W * sr-1 * m-2
			}

			static void QuaternionToLatLongHeading(crossplatform::Quaterniond q, double& latitude, double& longitude, double& heading);
			static void LatLongHeadingToQuaternion(crossplatform::Quaterniond& q, double latitude, double longitude, double heading);

			///Load/Save

			void SaveToText(crossplatform::TextOutput& output) const;
			void LoadFromText(crossplatform::TextInput& input);

		private:
			void UploadAuroralLayers(crossplatform::DeviceContext& deviceContext);
			float GetTime();
			long long last_frame_number = 0;
			bool FACchecksum(); //Returns true if checksum matches.
			void CalculateViewMatrixAndViewPosition(const crossplatform::ViewStruct& viewStruct);
			bool CheckGlobalPosition();
		};
	}
}

#if defined(_MSC_VER)
#pragma warning(default : 4251)
#endif