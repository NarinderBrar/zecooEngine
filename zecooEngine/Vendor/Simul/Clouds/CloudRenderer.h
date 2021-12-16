#ifndef SIMUL_CLOUDS_BASECLOUDRENDERER_H
#define SIMUL_CLOUDS_BASECLOUDRENDERER_H
#include "Simul/Clouds/Export.h"

#include "Simul/Clouds/CloudKeyframer.h"
#include "Simul/Clouds/GpuCloudGenerator.h"
#include "Simul/Clouds/CloudRenderingOptions.h"
#include "Simul/Clouds/CloudWindow.h"
#include "Platform/Core/ProfilingInterface.h"
#include "Platform/CrossPlatform/Quaterniond.h"
#include "Platform/CrossPlatform/DeviceContext.h"
#include "Platform/CrossPlatform/BaseRenderer.h"
#include "Platform/CrossPlatform/SphereRenderer.h"
#include "Platform/Shaders/SL/CppSl.sl"
#include "Simul/Shaders/SL/gpu_cloud_constants.sl"
#include "Simul/Shaders/SL/cloud_constants.sl"
#include "Simul/Shaders/SL/noise_constants.sl"
#include "Simul/Shaders/SL/sky_constants.sl"
#include "Platform/CrossPlatform/Effect.h"
#include "WindowUpdater.h"

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4251)
#endif

namespace simul
{
	namespace crossplatform
	{
		struct DeviceContext;
		class Effect;
		struct AmortizationStruct;
	}
	namespace sky
	{
		class BaseSkyInterface;
		class OvercastCallback;
		struct ScatteringVolume;
		struct LocalFadeTextures;
	}
	namespace crossplatform
	{
		class RenderPlatform;
		class Texture;
	}
	namespace clouds
	{
		class CloudInterface;
		class CloudKeyframer;
		class Environment;
		class LightningRenderInterface;
		class BaseGpuCloudGenerator;
		class TwoResFramebuffer;
		class CloudWindowEditor;
		struct CloudLayer
		{
			CloudLayer();
			~CloudLayer();
			void RestoreDeviceObjects(crossplatform::RenderPlatform *r);
			void InvalidateDeviceObjects();
			void ResetChecksums()
			{
				for(int i=0;i<3;i++)
				{	
					FillState &F=fillStates[i];
					F.checksum=0;
				}
				checksum=0;
			}

			sky::uid layer_uid;
			int texture_cycle;
			crossplatform::Texture *map;
			CloudKeyframer *cloudKeyframer;
			// offset from the window centre to the layer centre: this is for repeating layers. For others, we use origin to track an absolute world position.
			vec3 offsetKm;
			// This is for updating offsetKm. When the window's grid centre changes, we modify offsetKm.
			int2 WindowGridCentreTexel;
			crossplatform::Texture *cloud_textures[3];
			FillState fillStates[3];
			unsigned checksum;
			unsigned generation_number=0;
		};
		/// A point light source
		struct PointSource
		{
			vec3 pos_m;
			float min_radius;
			float max_radius;
			vec3 irradiance;
			int last_update_frame;
		};

		//! A helper class
		struct CloudGeometryHelper
		{
			int overrideStepCount;
			float forwardOffsetKm;
			float initialStepKm;
			unsigned stepPos;
			vec3 last_view_pos_km;
			int last_framenumber;
			int2 WindowGridCentreTexel;
			int initialSteps;
			CloudGeometryHelper():overrideStepCount(0)
				,forwardOffsetKm(0.0f)
				,initialStepKm(0.0f)
				,stepPos(0)
				,last_view_pos_km(0.f,0.f,0.f)
				,last_framenumber(0)
				,WindowGridCentreTexel(0,0)
				,initialSteps(7)
				,steps_in_use(0)
				,dist_in_use(0.0f)
			{
			}
			void Update(int,float,float);
		protected:
			int steps_in_use;
			float dist_in_use;
		};
		/*! Class for real-time volumetric cloud rendering.

			There should exist a "trueSKY space", where the origin(0,0,0 Cartesian) is at global mean sea level, and an arbitrary point on the Earth's surface.
			The Z axis points up, the X and Y axes are arbitrary (for reasons described below). To avoid singularities, this point, and the orientation of its axes
			should be represented by a double-precision quaternion, which represents the rotation from (say) latitude and longitude zero with X pointing East and Y pointing North.

			The Volume Window is a deformed cuboid, its upper and lower surfaces matching the Earth's curvature. On the GPU, it is a 3D texture.

			This trueSKY space moves in steps equivalent to one horizontal texel. The function CloudRenderer::MoveCloudWindow(x,y) does this.
			This should be done when the chosen viewpoint (this is up to you) moves more than a texel in any horizontal direction. This way, we need only update the edges as the window moves.
*/
		SIMUL_CLOUDS_EXPORT_CLASS CloudRenderer
		{
		public:
			//! Constructor: An external keyframer is provided, and an optional memory manager.
			CloudRenderer(simul::clouds::Environment *e,simul::base::MemoryInterface *mem);
			//! Destructor
			virtual ~CloudRenderer();
			META_BeginProperties
			//	META_RangePropertyWithSetCall(float,MaxCloudDistanceKm,1.f,1000.f,DetailChanged	,"Furthest distance, in km, to draw clouds.")
				META_Property(bool					,UseAltCloudShadows								,"Whether this cloud renderer should use an alternate method of cloud shadow generation (Unity only).")
			META_EndProperties
			//! Place a point light source.
			void SetPointLight(int id,vec3 pos,float min_radius,float max_radius,vec3 irradiance);
			//! Set, or create a cloud query. This returns the previous results of the same query.
			VolumeQueryResult GetPointQuery(int id,vec3 pos_km);
			//! Get results of a cloud query.
			VolumeQueryResult GetPointQuery(int id) const;
			//! Get the query id for the specified view. If Render() has been called for the view, the query should have a valid set of results for the last frame rendered.
			int GetQueryIdForViewId(int view_id) const;
			//! Set, or create a cloud query. This returns the previous results of the same query.
			LineQueryResult GetLineQuery(int id,vec3 pos1_km,vec3 pos2_km);
			//!	Cloud rendering detail is determined by the number of slices. If not set the value from the CloudKeyframer will be used (GetDefaultNumSlices), but each viewport id can have a separate setting.
			void SetMaxSlices(int viewport_id,int maxs);
			//!	Cloud rendering detail is determined by the number of slices. See SetMaxSlices.
			int GetMaxSlices(int viewport_id) ;
			//! Platform-dependent function to reload the shaders - only use this for debug purposes.
			virtual void RecompileShaders();
			//! If possible, build all shader effect variations.
			static void EnsureEffectsAreBuilt(crossplatform::RenderPlatform *renderPlatform);
			//! Platform-dependent function to generate device objects for the renderer.
			virtual void RestoreDeviceObjects(crossplatform::RenderPlatform *renderPlatform);
			//! Call this Platform-dependent function when the 3D device has been lost.
			virtual void InvalidateDeviceObjects();
			void AsynchronousUpdate(crossplatform::DeviceContext &deviceContext,float real_time);
			//! Once per-frame update. Do this before any rendering each frame. Called by BaseWeatherRenderer.
			virtual void PreRenderUpdate(crossplatform::GraphicsDeviceContext &deviceContext,float real_time);
			//! Platform-dependent render function.
			virtual bool Render(crossplatform::GraphicsDeviceContext &deviceContext,float exposure,TrueSkyRenderMode renderMode,crossplatform::NearFarPass nearFarPass
				,crossplatform::Texture *depth_tex
				,const sky::ScatteringVolume *scatteringVolume,bool write_alpha
				,const vec4& viewportTextureRegionXYWH
				,const crossplatform::AmortizationStruct &amortizationStruct
				,clouds::TwoResFramebuffer *fb
				,sky::LocalFadeTextures *localFadeTextures
				,crossplatform::Texture *ambientCubemap);
			/// The cloud shadow texture:
			/// Centred on the viewer
			/// Aligned to the sun.
			/// Output is km in front of or behind the view pos where shadow starts
			virtual void RenderCloudShadowTexture(crossplatform::GraphicsDeviceContext &deviceContext);
			virtual void RenderPrecipitationVolumeTexture(crossplatform::GraphicsDeviceContext& deviceContext);
			virtual void RenderRainbowLookupTexture(crossplatform::GraphicsDeviceContext& deviceContext);
			//! Draw the queries in 3D
			virtual void RenderQueries(crossplatform::GraphicsDeviceContext &deviceContext);
			//! Render the cloud volumes for debugging purposes
			void RenderCloudVolumes(crossplatform::GraphicsDeviceContext &deviceContext,CloudKeyframer *ck);
			//! Render the cloud grid for debugging purposes
			void RenderCloudGrid(crossplatform::GraphicsDeviceContext &deviceContext,CloudKeyframer *ck);
			void RenderShapeMasks(crossplatform::GraphicsDeviceContext &deviceContext,CloudKeyframer *ck);
			virtual void RenderDebugInfo(crossplatform::GraphicsDeviceContext &deviceContext,int width,int height);
			//! Get some per-frame text information for debugging - usually timing data.
			virtual const char *GetDebugText() const;
			//! Show the cloud volumes onscreen by cross section.
			virtual void RenderCrossSections(crossplatform::GraphicsDeviceContext &context);
			virtual void RenderAuxiliaryTextures(crossplatform::GraphicsDeviceContext &context);
			/// Show the cloud volume window on the lat-long sphere.

			inline void SetLightingQueryResult(const LightingQueryResult& _lightingQueryResult)
			{
				lightingQueryResult = _lightingQueryResult;
			}
			//force a full init of clouds
			void ResetWindow();
#if SIMUL_EDITOR
			// Editor rendering:
			void EditorRenderKeyframe(crossplatform::GraphicsDeviceContext& deviceContext, const CloudKeyframe* K, SelectionIdentifier::Type type_mask, vec4 lineColour, vec4 fillColour, bool volumeEdit = false);
			void EditorRenderCloudWindow(crossplatform::GraphicsDeviceContext &deviceContext,CloudWindowEditor *cloudWindowEditor);
#endif
			void SetIlluminationTexture(crossplatform::Texture *);
			void SetLightTableTexture(crossplatform::Texture*);
			//! Get a pointer to the Cloud Generator
			virtual simul::clouds::BaseGpuCloudGenerator *GetBaseGpuCloudGenerator(){ return gpuCloudGenerator; }
			virtual simul::clouds::BaseGpuCloudGenerator *CreateGpuCloudGenerator(base::MemoryInterface *m);
			//! Set the sky interface.
			void SetEnvironment(simul::clouds::Environment *e);
			//! Returns a new geometry helper.
			CloudGeometryHelper *GetCloudGeometryHelper(int view_id);
			//! Get the random 3D texture.
			virtual crossplatform::Texture *GetRandomTexture3D();
			//! Return true if the camera is above the cloudbase altitude.
			bool IsCameraAboveCloudBase(math::Vector3 cam_pos) const;
			//! Get a value for how much the sun is blocked from the clouds.
			float GetSunOcclusion(crossplatform::GraphicsDeviceContext &deviceContext,math::Vector3 cam_pos) ;
			//! Where supported, enable lightning generation.
			void SetEnableStorms(bool s);
			//! Adjust the noise texture
			void SetNoiseTextureProperties(int size,int freq,int octaves,float persistence);
			//! Get the xyz scales of the clouds in metres.
			virtual const float *GetCloudScales() const;
			//! Get an API-dependent identifier for the cloud's 2D shadow.
			void MakeCloudShadowTexture();
			const CloudShadowStruct &GetCloudShadowStruct();
			//! Distance for fade texture lookups:
			void SetMaxFadeDistanceKm(float dist_km);
			//! Altitude for fade texture lookups:
			void SetMaxFadeAltitudeKm(float ma_km);
			void ExportCloudVolume(unsigned char *target);
			//! Get a pointer to the current cloud rendering options
			CloudRenderingOptions &GetCloudRenderingOptions();
			void SetCloudRenderingOptions(const CloudRenderingOptions &c);
			CloudWindow &GetCloudWindow() ;
			float GetPrecipitation() const;
			float GetRainToSnow() const;
		protected:
			clouds::GpuCloudsParameters gpuCloudsParameters[3];
			float rain_to_snow;
			bool prec_init_low, prec_init_med, prec_init_high = true;
			bool sim;
			float last_sim_time;
			vec3 cloudToolIntercept;
			vec3 cloudToolVelocity;
			float cloudToolPress;
			float realTime;
			typedef std::map<int,VolumeQueryResult> VolumeQueryResults;
			VolumeQueryResults volumeQueries;
			VolumeQueryResults volumeQueryResults;
			typedef std::map<int,LineQueryResult> LineQueryResults;
			LineQueryResults lineQueries;
			LineQueryResults lineQueryResults;
			
			typedef std::map<int,PointSource> PointSources;
			PointSources pointSources;
			
			void SetCloudLightpassConstants(vec3 pos,float min_radius,float max_radius,vec3 irr,vec3 cam_pos);
			void CreateGpuCloudGenerator();
			simul::clouds::BaseGpuCloudGenerator *gpuCloudGenerator;
			void RenderCombinedCloudTexture(crossplatform::DeviceContext &deviceContext);
			virtual void Recompile();
			bool recompile_shaders;
			crossplatform::RenderPlatform	*renderPlatform;
			crossplatform::Effect			*effect;
			crossplatform::Effect			*noiseEffect;

			crossplatform::EffectTechnique	*mixCloudsTechnique;
			crossplatform::EffectTechnique	*mixMapTechnique;
			crossplatform::EffectTechnique	*mixCirrusTechnique;
			crossplatform::EffectTechnique	*mixCumulusTechnique=nullptr;
			crossplatform::EffectTechnique	*mixCumulonimbusTechnique=nullptr;

			crossplatform::EffectTechnique	*lineQueriesTechnique;
			crossplatform::EffectTechnique	*pointQueriesTechnique;

			crossplatform::EffectTechnique	*skylight;
			crossplatform::EffectTechnique	*skybox;
			crossplatform::EffectTechnique	*stat_rain;
			crossplatform::EffectTechnique	*edge_stat_rain;
			crossplatform::EffectTechnique	*stat;
			crossplatform::EffectTechnique	*edge_stat;
			crossplatform::EffectTechnique	*cons_rain;
			crossplatform::EffectTechnique	*edge_cons_rain;
			crossplatform::EffectTechnique	*cons;
			crossplatform::EffectTechnique	*edge_cons;
			crossplatform::EffectTechnique	*edge_rain;
			crossplatform::EffectTechnique	*full_compute;
			crossplatform::EffectTechnique	*edge_norain;
			crossplatform::EffectTechnique	*no_rain_compute;
			
			crossplatform::EffectTechnique	*cloud_shadow;
			crossplatform::EffectTechnique	*alt_cloud_shadow;
			
			crossplatform::EffectTechnique	*experimental_edge_rain;
			crossplatform::EffectTechnique	*experimental_full_compute;
			crossplatform::EffectTechnique	*experimental_edge_norain;
			crossplatform::EffectTechnique	*experimental_no_rain_compute;

			crossplatform::EffectTechnique	*lightpass_draw;
			crossplatform::EffectPass		*pointQueriesPass;
			crossplatform::EffectPass		*lineQueriesPass;
			
			crossplatform::Effect			*editEffect;
			std::vector<int3> amortizeOffsets;
			std::vector<int3> amortizeOffsets2;	//2d offsets.
			std::vector<int3> amortizeOffsetsPrecipitation;
			// Identify the textures etc within the effect:
			crossplatform::ShaderResource _cloudDensity;
			crossplatform::ShaderResource _cloudDensity2;
			crossplatform::ShaderResource _outputTexture3d;			
			crossplatform::ShaderResource _outputTexture1;
			crossplatform::ShaderResource _outputTexture2;
			crossplatform::ShaderResource _outputTexture3;
			crossplatform::ShaderResource _updateTexture;
			crossplatform::ShaderResource _noiseTexture3D;	
			crossplatform::ShaderResource _largeWorleyTexture3D;
			crossplatform::ShaderResource _smallWorleyTexture3D;	
			crossplatform::ShaderResource _lossTexture;
			crossplatform::ShaderResource _inscTexture;
			crossplatform::ShaderResource _skylTexture;
			crossplatform::ShaderResource _depthTexture;
			crossplatform::ShaderResource _ambientCubemapTexture;
			crossplatform::ShaderResource _depthTextureFace;
			crossplatform::ShaderResource _lightTableTexture;		
			crossplatform::ShaderResource _cloudMaskTexture;	
			crossplatform::ShaderResource _precipitationMap;	
			crossplatform::ShaderResource _precipitationVolume;	
			crossplatform::ShaderResource _precipitationVolumeRW;	
			crossplatform::ShaderResource _illuminationTexture;	
			crossplatform::ShaderResource _volumeInscatterTexture;
			crossplatform::ShaderResource _godraysTexture;
			crossplatform::ShaderResource _rainbowLookup;
			crossplatform::ShaderResource _rainbowLookupTexture;
			crossplatform::ShaderResource _coronaLookupTexture;
			crossplatform::ShaderResource _cloudSamplerState;
			crossplatform::ShaderResource _lineQueryResults;

			crossplatform::ShaderResource _queryInputs;
			crossplatform::ShaderResource _precipitationLayerInfos;
			crossplatform::ShaderResource _volumeQueries;

			crossplatform::RenderState*						blendAndWriteAlpha;
			crossplatform::RenderState*						blendAndDontWriteAlpha;
			crossplatform::ConstantBuffer<CloudConstants> cloudConstants;
			crossplatform::ConstantBuffer<CloudStaticConstants> cloudStaticConstants;
			crossplatform::ConstantBuffer<CloudPerViewConstants> cloudPerViewConstants;
			crossplatform::ConstantBuffer<CloudLightpassConstants> cloudLightpassConstants;
			crossplatform::ConstantBuffer<GpuCloudConstants>	gpuCloudConstants;
			crossplatform::ConstantBuffer<RendernoiseConstants> rendernoiseConstants;

			crossplatform::StructuredBuffer<CloudVolume_densityspace> cloudVolumes;
			crossplatform::StructuredBuffer<PrecipitationLayerInfo> precipitationLayerInfos;

			simul::base::MemoryInterface	*memoryInterface;
			void Create3DNoiseTexture(crossplatform::DeviceContext &deviceContext);
			virtual void SetCloudConstants(CloudConstants &cloudConstants,float real_time);
			virtual void SetCloudPerViewConstants(CloudPerViewConstants &cloudPerViewConstants
													,const crossplatform::ViewStruct &viewStruct
													,float exposure
													,const vec4& viewportTextureRegionXYWH
													, const mat4 *worldToScatteringVolumeMatrix
													,const crossplatform::AmortizationStruct *amortizationStruct);
			virtual void SetCloudShadowPerViewConstants(CloudPerViewConstants &c);
			void FillInQueries(crossplatform::DeviceContext &deviceContext);
			void ClearIterators();
			virtual void EnsureCorrectTextureSizes(crossplatform::DeviceContext &deviceContext);
			virtual void EnsureTexturesAreUpToDate(crossplatform::DeviceContext &deviceContext);
			virtual void EnsureCorrectIlluminationTextureSizes();
			virtual void EnsureIlluminationTexturesAreUpToDate();
			virtual void EnsureTextureCycle(CloudLayer *L);
			//LayerConstants layerConstants;
			float max_fade_distance_metres;
			float max_fade_altitude_metres;
			unsigned noise_checksum;
			int realtime_3d_octaves;
			
			unsigned illumination_texel_index[4];
			void DetailChanged();
			simul::clouds::LightningRenderInterface* lightningRenderInterface;
			simul::clouds::Environment* environment;
			size_t cloudKeyframersSize = 0;
			int cloud_tex_width_x,cloud_tex_length_y,cloud_tex_depth_z;
			int illum_tex_width_x,illum_tex_length_y,illum_tex_depth_z;
			float thunder_volume;
			bool enable_lightning;
			float last_time;
			int last_update_frame;
			bool FailedNoiseChecksum();

			bool composite_rainbow = false;
			void SetRainbowPosition(CloudRenderingOptions& opt);
			
			std::vector<CloudLayer*> cloudLayers;
			crossplatform::Texture* shadow_fb;
			// A texture whose x-axis represents azimuth, and whose y-axis represents distance
			// as a proportion of shadow range. The texels represent how much illumination accumulates between the viewer
			// and that distance.

			CloudVolumeTextures cloudVolumeTextures[4];
			crossplatform::Texture	*light_valid=nullptr;	// invalidate light values.
			bool external_cloud_texture;
			
			crossplatform::Texture	*cloud_mask;
			crossplatform::Texture	*precipitation_volume;
			crossplatform::Texture	*rainbow_lookup;
			crossplatform::Texture	*largeWorleyTexture3D;
			crossplatform::Texture	*smallWorleyTexture3D;
			crossplatform::Texture	*noise_texture_3D;
			crossplatform::Texture	*random_3d;
			
			crossplatform::Texture	*illuminationTexture;
			crossplatform::Texture	*lightTableTexture;
			crossplatform::Texture	*rainbowLookupTexture;
			crossplatform::Texture	*coronaLookupTexture;
			crossplatform::SamplerState*	m_pWrapSamplerState;
			crossplatform::SamplerState*	m_pClampSamplerState;

			crossplatform::StructuredBuffer<VolumeQueryResult> volumeQueriesSB;
			crossplatform::StructuredBuffer<vec4> queryInputs;
			crossplatform::StructuredBuffer<LineQueryResult> lineQueryResultsSB;

		protected:
			void EnsureLayersAreInitialized();
			typedef std::map<int,simul::clouds::CloudGeometryHelper*> HelperMap;
			HelperMap helpers;
			/// A checksum to see if the interpolated cloud volume is out of date. This may fail every frame if time is moving continuously - that's ok.
			unsigned last_subdivision_checksum;
			void RenderPointSource(crossplatform::GraphicsDeviceContext &deviceContext,clouds::TwoResFramebuffer *fb
				,float exposure,const vec4& viewportTextureRegionXYWH);
			float layered_precipitiation_value;// that was used to render precip map
			int framenumber;
			int merge_framenumber;
			float time_step;
			unsigned last_true_checksum;
			crossplatform::PixelFormat cloudTextureFormat;
			CloudRenderingOptions cloudRenderingOptions;
			CloudWindow cloudWindow;
			unsigned setupChecksum;
			CloudShadowStruct cloudShadowStruct;
			crossplatform::ViewStruct mainCameraViewStruct;
			long long lastMainCameraViewStoredFrame=0;
			//h_FOV and v_FOV are returned in radians.
			void ExtractMainCameraData(vec3& cam_pos, vec3& cam_view, float& h_FOV, float& v_FOV);

			LightingQueryResult lightingQueryResult;
			bool initializedCombinedTexture=false;
#if SIMUL_EDITOR
			crossplatform::SphereRenderer* sphereRenderer;
#endif
			DetailLevel high,med,low;
			WindowUpdater windowUpdater;
			bool cloud_volume_empty=false;
			bool precipitationVolumeCleared = false;
		};
	}
}

#ifdef _MSC_VER
	#pragma warning(pop)
#endif
#endif