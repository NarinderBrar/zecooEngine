#pragma once
#include "Simul/Sky/Export.h"
#include "Simul/Sky/Binding.h"
#include "Simul/Sky/BaseSky.h"
#include "Platform/Core/PropertyMacros.h"
#include "Simul/Sky/Float4.h"
#include "Simul/Sky/SkyInterface.h"

#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable:4275)
#endif


namespace simul
{
	namespace sky
	{
//! A class that calculates the positions of sun, moon and stars.
		SIMUL_SKY_EXPORT_CLASS SiderealSky: public SiderealSkyInterface
		{
		public:
			//! Get the position of the sun in the sky with regards to its Azimuth and Elevation
			void GetSunGeocentricPosition(double &RA,double &decl,double epoch_time) const;
			//! Get the position of the moon in the sky with regards to its Azimuth and Elevation
			void GetMoonGeocentricPosition(double &RA,double &decl,double epoch_time,const Orbit &orbit,double *R=nullptr) const;

			//! Get the position of the sun in the sky with regards to its Azimuth and Elevation
			void GetSunPosition(float &azimuth,float &elevation,double epoch_time,float latitude,float longitude) const;
			//! Get the position of the moon in the sky with regards to its Azimuth and Elevation
			void GetMoonPosition(float &azimuth,float &elevation,double epoch_time,float latitude,float longitude,const Orbit &orbit) const;
			double GetDawn(double epoch_time_noon,float latitude,float longitude) const;
			double GetSunset(double epoch_time_noon,float latitude,float longitude) const;
			//! Get the month and day.
			static void GetDate(int y,int &M,int &D,int day_of_the_year);	//< 1 = January, 12 = December
			static int DateToDayInYear(int Year,int M,int D);
			
			static double GetDayNumberFromDate(int year,int day_OfTheYear,double Time);				// From 12 midday 1st January 2000

			//! Convert year, month and day into a Julian Day Number.
			static double GetDayNumberFromDate(int year,int month,int day);
			static void GetDateFromDayNumber(double d,int &year,int &month,int &day);
			const float *SiderealToEarthMatrix(double epoch_time,float LatitudeRadians,float LongitudeRadians) const;
			//! How many days in the specified year.
			static int DaysInTheYear(int y);
			double GetSiderealTimeDegrees(double epoch_time) const;
		//private:
			double GetSiderealTimeAtGreenwichDegrees(double day_number) const;
			double GetLocalSiderealTimeDegrees(double day_time_decimal,double longitude_degrees) const;
			void AscensionDeclinationToAzimuthElevation(double day_number,double delta,double RA,float &az,float &el,float latitude,float longitude) const;
			void AzimuthElevationToAscensionDeclination(double day_number,double &decl_rad,double &RA_rad,const float az,const float el,float LatitudeRadians,float LongitudeRadians) const;
		};
	}
}

#ifdef _MSC_VER
    #pragma warning(pop)
#endif
