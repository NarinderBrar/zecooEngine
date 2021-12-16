namespace simul
{
/*! 
\image html "SkyInheritance.png"

The simul::sky namespace corresponds to the Simul/Sky library.

Static library: Simul/lib/(PLATFORM)/(COMPILER)/(ConfigurationName)/SimulSky_(RUNTIME).lib

Dynamic library: Simul/exe/(PLATFORM)/(COMPILER)/(ConfigurationName)/SimulSky_(RUNTIME).dll

Sky depends on the Base and Math libraries.

Simul Sky calculates the colour gradations of the sky, including the
position of the sun, the colour of sunlight and variations in brightness with azimuth, elevation, altitude,
and weather conditions..

Simul Sky integrates with the Simul Clouds library, which provides realistic volume-
rendering of various types of cloud.

To use Simul Sky directly, create an instance of simul::sky::BaseSkyRenderer.

*/

	namespace sky
	{
	}
}
