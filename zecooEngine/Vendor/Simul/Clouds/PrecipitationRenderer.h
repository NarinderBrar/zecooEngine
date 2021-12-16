#ifndef SIMUL_CLOUDS_BASEPRECIPITATIONRENDERER_H
#define SIMUL_CLOUDS_BASEPRECIPITATIONRENDERER_H
#include "Simul/Clouds/Export.h"

#include "Platform/Math/Matrix4x4.h"
#include "Simul/Sky/Float4.h"
#include "Platform/Shaders/SL/CppSl.sl"
#include "Simul/Shaders/SL/rain_constants.sl"
#include "Simul/Shaders/SL/cloud_constants.sl"
#include "Platform/CrossPlatform/BaseRenderer.h"
#include "Platform/CrossPlatform/Effect.h"
#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4251)
#endif

struct CloudShadowStruct;
namespace simul
{
	namespace sky
	{
		struct float4;
		class BaseSkyInterface;
	}
	namespace crossplatform
	{
		struct DeviceContext;
		class RenderPlatform;
		class Texture;
		class Effect;
		class Buffer;
		class Layout;
	}
	namespace clouds
	{
		//! A simple struct encapsulating a depth texture, a window defining the part of the texture covered in this view, and the projection properties
		//! that were used to generate it.
		struct DepthTextureStruct
		{
			const void *depth_tex;
			simul::sky::float4 viewportXYWH;
		};

		enum struct PrecipitationOptions
		{
			None=0
			,VelocityStreaks=1
			,SimulationTime=2
			,Paused=4
		};

		inline PrecipitationOptions operator|(PrecipitationOptions a, PrecipitationOptions b)
		{
			return static_cast<PrecipitationOptions>(static_cast<int>(a) | static_cast<int>(b));
		}

		inline PrecipitationOptions operator&(PrecipitationOptions a, PrecipitationOptions b)
		{
			return static_cast<PrecipitationOptions>(static_cast<int>(a) & static_cast<int>(b));
		}
		enum struct RainMethod
		{
			None=0
			,Colour
			,Cubemap
			,Normal
		};
		//! The class that renders rain and snow.
		/* http://www-das.uwyo.edu/~geerts/cwx/notes/chap09/hydrometeor.html
			Rain falls at 6-7 m/s.
			Snow falls at about 1 m/s, slightly faster aloft because of the density
			correction [exp(z/20)].
			Hail can fall much faster,its speed is approximately 1.4 D^0.8 (D is in mm)
			at sea level, the exact relationship depends on hail density and shape.
			
			The surface area of a snowflake increases about as rapidly as its weight,
			so the speed of snow increases only slightly with flake size : .
			Fall speed relation for unrimed snow is (3):
				V = a D^b (0.7 < a < 1.2, 0.11 < b < 0.16)
			where V in m/s, D in mm
		*/
		SIMUL_CLOUDS_EXPORT_CLASS PrecipitationRenderer
		{
		public:
			//! Constructor
			PrecipitationRenderer();
			//! Destructor
			virtual ~PrecipitationRenderer();
			//! Platform-dependent function called when initializing the precipitation renderer.
			virtual void RestoreDeviceObjects(crossplatform::RenderPlatform *renderPlatform);
			//! Platform-dependent function called when uninitializing the precipitation renderer.
			virtual void InvalidateDeviceObjects();
			//! Call this to draw precipitation.
			virtual void Render(crossplatform::GraphicsDeviceContext &deviceContext
				,crossplatform::Texture *depth_tex
				,crossplatform::Texture *cubemapTexture
				,float max_fade_distance_metres
				,vec4 viewportTextureRegionXYWH
				,vec3 sunlight,vec3 moonlight
				,RainMethod rainMethod
				,float time
			,bool write_blend_alpha);
			void SetPrecipitationVolume(const CloudShadowStruct &str);
			//! Show the textures onscreen for debugging.
			void RenderOverlay(crossplatform::GraphicsDeviceContext &deviceContext, int x0, int y0, int dx, int dy);
			static void EnsureEffectsAreBuilt(crossplatform::RenderPlatform *r);
			//! Platform-dependent function to reload the shaders - only use this for debug purposes.
			virtual void RecompileShaders();
			void SetCubemapTransform(const float *m);
			void SetBlurTexture(crossplatform::Texture *);
			//! Provide a random 3D texture. This is set externally so the texture can be shared.
			void SetRandomTexture3D(crossplatform::Texture *texture);
			void SetRainDepthTexture(crossplatform::Texture *texture);
			void SetRainDepthTransform(const float *m);
			void SetRainDepthProjection(const float *p);
			/// Set the scale of the rain depth texture to normalize it to [0,1]
			void SetRainDepthTextureScale(float s);
			META_BeginProperties
				META_RangeProperty(float			,RainFallSpeedMS	,6.f	,0.f	,100.f					,"Speed of falling rain.")
				META_RangeProperty(float			,SnowFallSpeedMS	,1.f	,0.f	,100.f					,"Speed of falling snow.")
				META_RangeProperty(float			,RainWindEffect		,0.1f	,0.f	,1.f					,"Strength of the effect of wind on precipitation direction.")
				META_RangeProperty(float			,WaverTimescaleS	,1.0f	,0.01f	,10.f					,"Time scale of wavering in seconds.")
				META_RangePropertyWithSetCall(float	,Aspect				,0.01f	,1000.f	,TextureRepeatChanged	,"How much precipitation texture is stretched vertically.")
				META_PropertyWithSetCall(int		,RainTextureRepeat	,TextureRepeatChanged					,"How many times the texture is repeated around 360 degrees.")
				META_RangeProperty(float			,Waver				,0.f,0.f,10.f							,"How much precipitation layers waver as they fall.")
				META_Property(bool					,ReverseDepth												,"Whether the projection matrix reverses the z-order, so 1 is near and 0 is far.")
				META_RangeProperty(float			,Intensity			,0.f,0.f,1.f							,"How much rain/snow.")
				META_Property(int					,MaxParticles												,"The maximum number of particles to render.")
				META_RangeProperty(float			,RainToSnow			,0.f,0.f,1.f							,"What proportion of snow relative to rain, from 0 to 1.")
				META_Property(float					,RadiusMetres												,"Size of the particle zone.")
				META_Property(float					,RainDropSizeMm												,"Size of a single raindrop.")
				META_Property(float					,SnowFlakeSizeMm											,"Size of a single snowflake.")
				META_Property(float					,MinDistanceMetres											,"Minimum distance: particles nearer will not be shown.")
				META_Property(PrecipitationOptions	,Options													,"Rendering option flags.")
			META_EndProperties

			bool GetUseSimulationTime() const
			{
				return (Options&PrecipitationOptions::SimulationTime)==PrecipitationOptions::SimulationTime;
			}
			//! Once per-frame update. Do this before any rendering each frame.
			virtual void PreRenderUpdate(crossplatform::GraphicsDeviceContext &deviceContext,float dt_seconds);
			//! Set the wind velocity.
			void SetWind(float speed,float heading_degrees)
			{
				wind_heading=heading_degrees*3.14159f/180.f;
				wind_speed=speed;
			}
			//! Set the wind velocity.
			void SetWind(const vec3& wind_direction)
			{
				wind_heading = atan2(wind_direction.x, wind_direction.y);
				wind_speed = sqrt(wind_direction.x * wind_direction.x + wind_direction.y * wind_direction.y);
			}
			//! Set the sky and atmospherics interface.
			void SetBaseSkyInterface(simul::sky::BaseSkyInterface *s);
			//! Clear the data()
			void New();
		protected:
			void InitParticles(crossplatform::DeviceContext& deviceContext);
			void Recompile(crossplatform::GraphicsDeviceContext&deviceContext);
			vec3 rainMapOrigin;
			vec2 rainMapExtents;

			crossplatform::RenderPlatform *renderPlatform;
			simul::sky::BaseSkyInterface *baseSkyInterface;
			crossplatform::Texture*					blurTexture;
			crossplatform::Texture*					randomTexture3D;
			crossplatform::Effect*						effect;
			crossplatform::Texture*						rain_texture;
			crossplatform::Texture*						precipitation_volume;
			virtual void TextureRepeatChanged();
			static void SetMoistureConstants(MoisturePerViewConstants &c,const DepthTextureStruct &depthStruct,const crossplatform::ViewStruct &viewStruct);
			float wind_heading,wind_speed;
			simul::sky::float4 offs;
			simul::sky::float4 meanVelocity;
			float time_step_seconds;
			int allocParticles;
			math::Matrix4x4 rainMapMatrix;
			crossplatform::Texture*					rainDepthTexture;
			math::Matrix4x4 rainDepthTransform;
			math::Matrix4x4 rainDepthProjection;
			float rainDepthTextureScale;
			void RenderParticles(crossplatform::GraphicsDeviceContext &deviceContext,crossplatform::Texture *cubemapTexture,crossplatform::Texture *depthTexture,RainMethod rainMethod,bool write_blend_alpha);
			crossplatform::Layout  			*layout;
			crossplatform::Texture			*previousCubemapSkylight; //We don't own this object; don't allocate or free.
		
			crossplatform::EffectTechnique	*techniqueOverlay;
			crossplatform::EffectTechnique	*m_hTechniqueSnowParticles_mask;
			crossplatform::EffectTechnique	*m_hTechniqueRainParticles_mask;
			crossplatform::EffectTechnique	*m_hTechniqueSnowParticles_nomask;
			crossplatform::EffectTechnique	*m_hTechniqueRainParticles_nomask;
			crossplatform::ConstantBuffer<RainConstants>			rainConstants;
			crossplatform::ConstantBuffer<RainPerViewConstants>		perViewConstants;
			crossplatform::StructuredBuffer<vec4> particleBuffer;
		
			math::Matrix4x4 cubemapTransform;
			bool recompile_shaders;
			
			// Different views in different positions may show different weather.
			struct ViewStore
			{
				ViewStore()
					:view_initialized(false)
					,viewPos(0.0,0.0,0.0)
					,offset(0,0,0)
					,overallOffset(0,0,0)
					,motionOffset(0,0,0) 
					,intensity(0.0f)
					,last_time(0.0f)
					,time_step(0.0f)
					,phase0(0)
					,phase1(0)
					,last_frame_number(0)
					{}
				bool view_initialized;
				mat4 worldViewProj;
				mat4 worldView;
				mat4 proj;
				vec3d viewPos;
				vec3 offset;
				vec3 overallOffset;
				vec3 motionOffset;
				float intensity;
				float last_time;
				float time_step;
				float phase0,phase1;
				int last_frame_number;
			};
			std::map<int,ViewStore*> viewStores;
			long long last_frame_number;
			
		};
	}
}
#ifdef _MSC_VER
	#pragma warning(pop)
#endif
#endif