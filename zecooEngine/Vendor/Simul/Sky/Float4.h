#ifndef SIMUL_SKY_FLOAT4_H
#define SIMUL_SKY_FLOAT4_H

#include "Simul/Sky/Export.h"
#include "Simul/Sky/Binding.h"
#include "Platform/Core/PropertyMacros.h"

namespace simul
{
	namespace sky
	{
		SIMUL_SKY_EXPORT_STRUCT float4
		{
			float x,y,z,w;
			float4(){}
			static float dot(const float4 &a,const float4 &b)
			{
				return a.x*b.x+a.y*b.y+a.z*b.z;
			}
			float4(const float4 &f)
			{
				x=f.x;
				y=f.y;
				z=f.z;
				w=f.w;
			}
			float4(const float* f)
			{
				x=f[0];
				y=f[1];
				z=f[2];
				w=f[3];
			}
			const float4 &operator=(const float* f)
			{
				x=f[0];
				y=f[1];
				z=f[2];
				w=f[3];
				return *this;
			}
			const float* operator()() const
			{
				return (const float*)this;
			}
			operator const float*() const
			{
				return (const float*)this;
			}
			operator float*()
			{
				return (float*)this;
			}
			void operator=(const float4 &f)
			{
				x=f.x;
				y=f.y;
				z=f.z;
				w=f.w;
			}
			float4(float X,float Y,float Z,float W)
			{
				x=X;
				y=Y;
				z=Z;
				w=W;
			}
			float4 operator -() const
			{
				float4 r;
				r.x=-x;
				r.y=-y;
				r.z=-z;
				r.w=-w;
				return r;
			}
			float4 operator -(const float4 &f)
			{
				float4 r;
				r.x=x-f.x;
				r.y=y-f.y;
				r.z=z-f.z;
				r.w=w-f.w;
				return r;
			}
			float4 operator +(const float4 &f)
			{
				float4 r;
				r.x=x+f.x;
				r.y=y+f.y;
				r.z=z+f.z;
				r.w=w+f.w;
				return r;
			}
			float4 &operator +=(const float4 &f)
			{
				x+=f.x;
				y+=f.y;
				z+=f.z;
				w+=f.w;
				return *this;
			}
			float4 &operator -=(const float4 &f)
			{
				x-=f.x;
				y-=f.y;
				z-=f.z;
				w-=f.w;
				return *this;
			}
			float4 &operator *=(const float4 &f)
			{
				x*=f.x;
				y*=f.y;
				z*=f.z;
				w*=f.w;
				return *this;
			}
			float4 &operator /=(const float4 &f)
			{
				x/=f.x;
				y/=f.y;
				z/=f.z;
				w/=f.w;
				return *this;
			}
			float4 &operator *=(float f)
			{
				x*=f;
				y*=f;
				z*=f;
				w*=f;
				return *this;
			}
			float4 &operator /=(float f)
			{
				x/=f;
				y/=f;
				z/=f;
				w/=f;
				return *this;
			}
			void Set(float X,float Y,float Z,float W)
			{
				x=X;
				y=Y;
				z=Z;
				w=W;
			}
			friend float brightness(const float4 &l)
			{
				return (0.299f *l.x) + (0.587f * l.y) + (0.114f * l.z);
			}
			friend float4 operator *(float f,const float4 &c)
			{
				float4 r;
				r.x=c.x*f;
				r.y=c.y*f;
				r.z=c.z*f;
				r.w=c.w*f;
				return r;
			}
			static float4 Min(const float4 &c1,const float4 &c2);
			static float4 Max(const float4 &c1,const float4 &c2);
			friend float4 operator *(const float4 &c1,const float4 &c2)
			{
				float4 r;
				r.x=c1.x*c2.x;
				r.y=c1.y*c2.y;
				r.z=c1.z*c2.z;
				r.w=c1.w*c2.w;
				return r;
			}
			friend float4 operator /(const float4 &c1,const float4 &c2)
			{
				float4 r;
				r.x=c1.x/c2.x;
				r.y=c1.y/c2.y;
				r.z=c1.z/c2.z;
				r.w=c1.w/c2.w;
				return r;
			}
			friend float4 operator *(const float4 &c,float f)
			{
				float4 r;
				r.x=c.x*f;
				r.y=c.y*f;
				r.z=c.z*f;
				r.w=c.w*f;
				return r;
			}
			friend float4 operator /(const float4 &c,float f)
			{
				float4 r;
				r.x=c.x/f;
				r.y=c.y/f;
				r.z=c.z/f;
				r.w=c.w/f;
				return r;
			}
			friend float4 operator+(const float4 &c,const float4 &d)
			{
				float4 r;
				r.x=c.x+d.x;
				r.y=c.y+d.y;
				r.z=c.z+d.z;
				r.w=c.w+d.w;
				return r;
			}
			friend float4 operator-(const float4 &c,const float4 &d)
			{
				float4 r;
				r.x=c.x-d.x;
				r.y=c.y-d.y;
				r.z=c.z-d.z;
				r.w=c.w-d.w;
				return r;
			}
			friend float4 operator+(const float4 &c,float d)
			{
				float4 r;
				r.x=c.x+d;
				r.y=c.y+d;
				r.z=c.z+d;
				r.w=c.w+d;
				return r;
			}
			friend float4 operator-(const float4 &c,float d)
			{
				float4 r;
				r.x=c.x-d;
				r.y=c.y-d;
				r.z=c.z-d;
				r.w=c.w-d;
				return r;
			}
			friend float4 operator+(float d,const float4 &c)
			{
				float4 r;
				r.x=c.x+d;
				r.y=c.y+d;
				r.z=c.z+d;
				r.w=c.w+d;
				return r;
			}
			friend float4 operator-(float d,const float4 &c)
			{
				float4 r;
				r.x=d-c.x;
				r.y=d-c.y;
				r.z=d-c.z;
				r.w=d-c.w;
				return r;
			}
		};
		extern float4 SIMUL_SKY_EXPORT lerp(float x,const float4 &a,const float4 &b);
		extern float4 SIMUL_SKY_EXPORT Pow(const float4 &c,float p);
		extern float4 SIMUL_SKY_EXPORT Exp(const float4 &c);
		extern float4 SIMUL_SKY_EXPORT Ln(const float4 &c);
		extern float SIMUL_SKY_EXPORT length(const float4 &f);
		extern void SIMUL_SKY_EXPORT zero(float4 &u);
		extern void SIMUL_SKY_EXPORT unit(float4 &u);
		extern float SIMUL_SKY_EXPORT largest(const float4 &u);
		extern float4 SIMUL_SKY_EXPORT saturate(const float4 &l);
		extern float4 SIMUL_SKY_EXPORT cross(const float4 &a,const float4 &b);
		extern float SIMUL_SKY_EXPORT dot(const float4 &a,const float4 &b);
	}
}
#endif
