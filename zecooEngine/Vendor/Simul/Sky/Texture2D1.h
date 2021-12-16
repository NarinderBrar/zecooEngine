#ifndef SIMULSKY_TEXTURE1D1_H
#define SIMULSKY_TEXTURE1D1_H
#include "Platform/Core/MemoryInterface.h"
#include "Simul/Sky/Export.h"

SIMUL_SKY_EXPORT_CLASS Texture2D1
{
	simul::base::MemoryInterface *memoryInterface;
	float *tex;
	int X;
	int Y;
	int texsize;
public:
	Texture2D1(simul::base::MemoryInterface *m);
	~Texture2D1();
	float Get(float u,float v) const;
	void Setup(int x,int y);
	void Set(int x,int y,float z);
	float *GetDataPointer();
};
#endif