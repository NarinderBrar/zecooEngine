#pragma once
namespace simul
{
	namespace base
	{
		/// Very simple 32-bit variant class for passing parameters.
		struct Variant32
		{
			Variant32(float f):floatVal(f)
			{
			}
			Variant32(int f=0):intVal(f)
			{
			}
			union
			{
				float floatVal;
				int intVal;
			};
		};
		/// Very simple variant class for passing parameters.
		struct Variant
		{
			Variant(float f):floatVal(f)
			{
			}
			Variant(int f=0):intVal(f)
			{
			}
			union
			{
				float floatVal;
				int intVal;
				double doubleVal;
				float vec3Val[3];
				float vec2Val[2];
				int int3Val[3];
				int int2Val[2];
			};
		};
	}
}