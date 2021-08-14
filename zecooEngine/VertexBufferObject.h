#pragma once

#include <ctime>
#include <windows.h>

#include <cstring>
#include <vector>
#include <sstream>

#include <glad/glad.h>
#include <glfw3.h>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

// This class provides a wrapper around an OpenGL Vertex Buffer Object
class VertexBufferObject
{
public:
	VertexBufferObject();
	~VertexBufferObject();

	void Create();									// Creates a VBO
	void Bind();									// Binds the VBO
	void Release();									// Releases the VBO

	void AddData(void* ptrData, UINT dataSize);	// Adds data to the VBO
	void UploadDataToGPU(int usageHint);			// Uploads the VBO to the GPU


private:
	UINT m_vbo;									// VBO id
	vector<BYTE> m_data;							// Data to be put in the VBO
	bool m_dataUploaded;							// A flag indicating if the data has been sent to the GPU
};