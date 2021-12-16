#ifndef SIMULSKY_TEXTURE1D4_H
#define SIMULSKY_TEXTURE1D4_H
#include "Simul/Sky/Float4.h"
#include "Simul/Sky/Export.h"
#include "Platform/Core/MemoryInterface.h"
SIMUL_SKY_EXPORT_CLASS Texture1D4
{
	simul::sky::float4 *tex;
	int size;
	bool Wrap;
	bool Border;
	simul::sky::float4 Edge;
public:
	Texture1D4(simul::base::MemoryInterface *mem);
	~Texture1D4();
	void setMemoryInterface(simul::base::MemoryInterface *mem);
	const Texture1D4 &operator=(const Texture1D4 &t);
	simul::sky::float4 Get(float u) const;
	simul::sky::float4 *Setup(int x,bool wrap,bool border,simul::sky::float4 edge=simul::sky::float4(0.f,0.f,0.f,0.f));
	simul::sky::float4 *GetDataPointer();
	void Zero();
	simul::base::MemoryInterface *memoryInterface;
};
#endif
