#ifndef SIMUL_FADETABLECALLBACK_H
#define SIMUL_FADETABLECALLBACK_H
namespace simul
{
	namespace sky
	{
//! A callback virtual base class that fade tables use.

//! A callback virtual base class that fade tables use. A fade table is any class derived from FadeTableInterface, and it
//! uses these callback functions to tell your platform-specific renderer how to create and modify the sky and fade textures.
//! Usually you would derive your sky-rendering class from SkyTexturesCallback, and implement these functions. See, for example,
//! the DirectX implementation of SimulSkyRenderer.
/*
		A class that implements this interface must have the following functions:
	\code
	virtual void SetSkyTextureSize(unsigned size);
	virtual void SetFadeTextureSize(unsigned width,unsigned height,unsigned num_altitudes);
	virtual void FillFadeTextures(int alt_index,int texture_index,int texel_index,int num_texels,const float *loss_float4_array,const float *inscatter_float4_array);
	virtual void FillSkyTexture(int alt_index,int texture_index,int texel_index,int num_texels,const float *float4_array);
	virtual void CycleTexturesForward();
	\endcode
*/
		class SkyTexturesCallback
		{
		public:
			//! Inform the derived class of the table or texture size. All three tables should be sized to these parameters.
			virtual void SetSkyTextureSize(unsigned size)=0;
			//! Inform the derived class of the table or texture size. All three tables should be sized to these parameters.
			//! If a sunlight texture is used (see FillSunlightTexture), make this a 1D texture of size \p num_altitudes.
			virtual void SetFadeTextureSize(unsigned width_num_distances,unsigned height_num_elevations,unsigned num_altitudes)=0;
			//! Inform the derived class that the texture \p texture_index, either 0, 1 or 2, at altitude
			//! index \p alt_index, (not used if not using multiple altitudes) should have \p num_texels texels
			//! filled starting at \p texel_index, with the values in the arrays \p loss_float4_array and \p
			//! inscatter_float4_array, each of which contains four times \p num_texels floats. The first number is
			//! the value for \p texel_index, rather than the start of the whole table.
			//! Each group of four represents the r,g,b and a of a colour value, and the values may be greater
			//! than one, so floating point textures are preferred.
			//! Only one of FillFadeTexturesSequentially and FillFadeTextureBlocks need be implemented, depending on the graphics API.
			//! Some API's permit transfer of data sequentially in memory to textures (e.g. DirectX 9), some expect cuboid blocks of
			//! texels to be filled (e.g. OpenGL). The unwanted function should be implemented as a stub, with an assert() or some other
			//! fail condition, to make sure it is never called.
			virtual void FillFadeTexturesSequentially(int texture_index,int texel_index,int num_texels,const float *loss_float4_array,const float *inscatter_float4_array)=0;


			//! Inform the derived class that the textures should be cycled forward - i.e. texture 2 becomes 1, 3 becomes 2 and 1 becomes 3.
			//! This is done when interpolation has crossed over a step boundary.
			virtual void CycleTexturesForward()=0;
		};
	}
}
#endif