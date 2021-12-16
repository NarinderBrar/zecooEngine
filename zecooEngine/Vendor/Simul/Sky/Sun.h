#pragma once

namespace simul
{
	namespace sky
	{
		struct dvec2
		{
			double x,y;
		};
		extern double GetEarthSolarSpectralIrradiance(double wavelength_nm);
		/*! Solar irradiance just outside the Earth's atmosphere, from the Wehrli Standard Extraterrestrial Solar Irradiance Spectrum, 1985.
		http://rredc.nrel.gov/solar/spectra/am0/wehrli1985.new.html
		*/
		extern dvec2 SolarIrradianceEarth_nm_Wm2nm[];
	}
}