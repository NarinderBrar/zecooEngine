#pragma once
#include <glad/glad.h>
#include <glfw3.h>

#include <windows.h>
#include <string.h>

#include "Common.h"
#include "texture.h"

#include "FreeImage.h"

// Class that provides a texture for texture mapping in OpenGL
class Texture
{
public:
	void CreateFromData(BYTE* data, int width, int height, int bpp, GLenum format);
	bool Load(string path);
	void Bind(int textureUnit = 0);

	void SetSamplerObjectParameter(GLenum parameter, GLenum value);
	void SetSamplerObjectParameterf(GLenum parameter, float value);

	int GetWidth();
	int GetHeight();
	int GetBPP();

	void Release();

	Texture();
	~Texture();
private:
	int m_width, m_height, m_bpp; // Texture width, height, and bytes per pixel
	UINT m_textureID; // Texture id
	UINT m_samplerObjectID; // Sampler id

	string m_path;
};

