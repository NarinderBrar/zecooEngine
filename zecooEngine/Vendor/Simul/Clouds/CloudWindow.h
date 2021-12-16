#pragma once
#include "Simul/Clouds/Export.h"
#include "Platform/CrossPlatform/Quaterniond.h"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4251)
#endif
namespace simul
{
	namespace clouds
	{
		struct CloudRenderingOptions;

		struct SIMUL_CLOUDS_EXPORT CloudWindow
		{
			CloudWindow();
			/// assignment operator only copies properties, not state.
			const CloudWindow &operator=(const CloudWindow &);
			void			Update(vec3 wind_spd_ms,float dt);
			/// Reset offsets
			void			Reset();
			/// Initialize the window.
			void			InitWindowCentre(double lat_degrees,double long_degrees,double x_heading_degrees);
			/// Returns updated x heading in degrees.
			float			UpdateWindowCentre(double lat_degrees, double long_degrees);
			//! Translate the cloud window by the given x and y
			void			MoveCloudWindow(int x,int y);
			//! Check if the cloud window origin has changed, and update values if it has.
			void			CheckForOriginChange(crossplatform::Quaterniond new_origin,const CloudRenderingOptions &opts);
			void			SetTrueWindOffset();
			static void		GetLatitudeLongitudeHeadingDegrees(crossplatform::Quaterniond new_origin,vec3 pos, double &lat, double &lon, double &head);
			void			GetLatitudeLongitudeHeading(vec3 pos,double &lat,double &lon,double &head) const;		///< Get the latitude/longitude referred to by the specified position value.
			void			GetOriginLatitudeLongitudeHeading(double &lat_deg,double &lon_deg,double &head_deg) const;			///< Get the latitude/longitude referred to by the specified quaternion.
			double			GetOriginLatitudeDegrees() const;			///< Get the latitude referred to by the origin quaternion.
			double			GetOriginLongitudeDegrees() const;			///< Get the longitude referred to by the origin quaternion.
			double			GetOriginHeadingDegrees() const;
			float			MinimumHeightKm;
			int3			VolumeTextureGrid;
			vec3			GlobalQuaternionToLocalOffsetKm(crossplatform::Quaterniond global_quat) const;

			// Convert the WGS84 position to local coordinates.
			vec3			WGS84PositionToLocal(vec3d pos_km) const;
			// Convert the WGS84 direction to local coordinates.
			vec3			WGS84DirectionToLocal(vec3d dir) const;
			// Convert the local direction to WGS84 coordinates.
			vec3d			LocalPositionToWGS84(vec3d pos) const;
			// Convert the local direction to WGS84 coordinates.
			vec3d			LocalDirectionToWGS84(vec3d dir) const;
			static crossplatform::Quaterniond LatLongHeadingDegreesToQuaternion(double lat_deg, double long_deg, double heading_deg);
			static crossplatform::Quaterniond LatLongHeadingToQuaternion(double lat_radians, double long_radians, double heading_radians);

			static crossplatform::Quaterniond XYZToQuaternion(vec3d pos);
			// wind offset
			vec3			windOffsetKm;
			// Properties of cloud window
			crossplatform::Quaterniond		origin;
			// Window
			int				WindowGridX;
			int				WindowGridZ;
			int				PrecipitationGridDivisorX;
			int				PrecipitationGridDivisorZ;
			vec3			WindowScaleKm;					// x and y scale of the window.
			int2			WindowGridCentreTexel;			// integer for the centre texel.
			unsigned		window_checksum;
			int2			GetGridOffset(int2 old_ctr) const;
			// state with respect to CloudRenderingOptions
			crossplatform::Quaterniond last_origin;
			vec3			EdgeNoiseOffsetKm;				//< Offset relative to 
			vec3			EdgeNoiseOffset2Km;
			vec3			WorleyOffsetKm;
			vec3			GridOriginKm;
			vec3			gridWindOffsetKm;
		protected:
			vec3			noiseOffsetKm;
			vec3			cellNoiseOffsetKm;
		};
	}
}
#ifdef _MSC_VER
#pragma warning(pop)
#endif