#include "FramebufferGL.h"
#include <iostream>
#include <string>
#include "Platform/Core/RuntimeError.h"
#include "Platform/CrossPlatform/DeviceContext.h"
#include "Platform/CrossPlatform/Macros.h"
#include "Platform/OpenGL/RenderPlatform.h"

#ifdef _MSC_VER
    #include <windows.h>
#endif

using namespace simul;
using namespace opengl;

FramebufferGL::FramebufferGL(const char* name):
    BaseFramebuffer(name),
    mFBOId(0)
{
    if (name)
        this->name = name;
}

FramebufferGL::~FramebufferGL()
{
    InvalidateDeviceObjects();
}

void FramebufferGL::SetWidthAndHeight(int w, int h, int m)
{
    if (Width != w || Height != h || mips != m)
    {
        if(buffer_texture)
			buffer_texture->InvalidateDeviceObjects();
		if(buffer_depth_texture)
			buffer_depth_texture->InvalidateDeviceObjects();
        Width   = w;
        Height  = h;
        mips    = m;
    }
}

void FramebufferGL::SetAsCubemap(int w, int num_mips, crossplatform::PixelFormat f)
{
    SetWidthAndHeight(w, w, num_mips);
    SetFormat(f);
    is_cubemap = true;
}

void FramebufferGL::SetFormat(crossplatform::PixelFormat f)
{
    if (target_format == f)
    {
        return;
    }
    target_format = f;
	if(buffer_texture&&!external_texture)
		buffer_texture->InvalidateDeviceObjects();
}

void FramebufferGL::SetDepthFormat(crossplatform::PixelFormat f)
{
    if ((int)depth_format == f)
    {
        return;
    }
    depth_format = f;
	if(buffer_depth_texture&&!external_depth_texture)
		buffer_depth_texture->InvalidateDeviceObjects();
}

bool FramebufferGL::IsValid() const
{
    return ((buffer_texture != nullptr && buffer_texture->IsValid()) || (buffer_depth_texture != nullptr && buffer_depth_texture->IsValid()));
}

void FramebufferGL::RestoreDeviceObjects(crossplatform::RenderPlatform *r)
{
    BaseFramebuffer::RestoreDeviceObjects(r);
}

void FramebufferGL::ActivateDepth(crossplatform::GraphicsDeviceContext &deviceContext)
{
}

void FramebufferGL::SetAntialiasing(int s)
{
    numAntialiasingSamples = s;
}

bool FramebufferGL::CreateBuffers()
{
    if (!Width || !Height)
    {
        return false;
    }
    if (!renderPlatform)
    {
        SIMUL_BREAK("renderPlatform should not be NULL here");
        return false;
    }
    if ((buffer_texture && buffer_texture->IsValid()))
    {
        return true;
    }
    if (buffer_depth_texture && buffer_depth_texture->IsValid())
    {
        return true;
    }
    if (buffer_texture)
    {
        buffer_texture->InvalidateDeviceObjects();
    }
    if (buffer_depth_texture)
    {
        buffer_depth_texture->InvalidateDeviceObjects();
    }
    if (!buffer_texture)
    {
        std::string sn = "BaseFramebuffer_" + name;
        buffer_texture = renderPlatform->CreateTexture(sn.c_str());
    }
    if (!buffer_depth_texture)
    {
        std::string sn = "BaseFramebufferDepth_" + name;
        buffer_depth_texture = renderPlatform->CreateTexture(sn.c_str());
    }
    static int quality = 0;
    if (!external_texture && target_format != crossplatform::UNKNOWN)
    {
        if (!is_cubemap)
        {
            buffer_texture->ensureTexture2DSizeAndFormat(renderPlatform, Width, Height, 1, target_format, false, true, false, numAntialiasingSamples, quality);
        }
        else
        {
            buffer_texture->ensureTextureArraySizeAndFormat(renderPlatform, Width, Height, 1, mips, target_format, false, true, true);
        }
    }
    if (!external_depth_texture && depth_format != crossplatform::UNKNOWN)
    {
        buffer_depth_texture->ensureTexture2DSizeAndFormat(renderPlatform, Width, Height, 1,depth_format, false, false, true, numAntialiasingSamples, quality);
    }

	glDeleteFramebuffers((GLsizei)mFBOId.size(),mFBOId.data());
	
	int faces=is_cubemap?6:1;
	current_face = 0;
	mFBOId.resize((size_t)mips*(size_t)faces);
    // Generate GL FBO:
    glGenFramebuffers((GLsizei)mFBOId.size(), mFBOId.data());
	auto &f=mFBOId.begin();
	for(int i=0;i<mips;i++)
	for(int j=0;j<faces;j++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER,*f);
		{
			GLenum target = numAntialiasingSamples == 1 ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE;
			crossplatform::ShaderResourceType srt = numAntialiasingSamples == 1 ? crossplatform::ShaderResourceType::TEXTURE_2D : (crossplatform::ShaderResourceType::TEXTURE_2D|crossplatform::ShaderResourceType::MS);

			auto glcolour = (opengl::Texture*)buffer_texture;
			auto gldepth = (opengl::Texture*)buffer_depth_texture;
			
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target, glcolour->AsOpenGLView(srt, j, i), 0);
			if (depth_format != crossplatform::UNKNOWN)
			{
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, target, gldepth->AsOpenGLView(srt, j, i), 0);
			}
		}
		f++;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    return true;
}

void FramebufferGL::InvalidateDeviceObjects()
{
	glDeleteFramebuffers((GLsizei)mFBOId.size(),mFBOId.data());
    mFBOId.clear();
	BaseFramebuffer::InvalidateDeviceObjects();
}

void FramebufferGL::Activate(crossplatform::GraphicsDeviceContext& deviceContext)
{
	if((!buffer_texture||!buffer_texture->IsValid())&&(!buffer_depth_texture||!buffer_depth_texture->IsValid()))
		CreateBuffers();
	int fb=0;
	if(is_cubemap)
		fb=current_face;
    glBindFramebuffer(GL_FRAMEBUFFER, mFBOId[fb]);
	colour_active   = true;
	GLenum target = numAntialiasingSamples == 1 ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE;
    opengl::Texture* glcol      = (opengl::Texture*)buffer_texture;
    opengl::Texture* gldepth    = nullptr;
    if (buffer_depth_texture)
    {
        gldepth         = (opengl::Texture*)buffer_depth_texture;
        // Re-attach depth (we dont really to do this every time, only if we called deactivate depth)
        if (depth_active == false)
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, target, gldepth->GetGLMainView(), 0);
        }
        depth_active    = true;
    }
    
    // We need to attach the requested face:
    if (is_cubemap)
    {
       // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, glcol->AsOpenGLView(crossplatform::ShaderResourceType::TEXTURE_2D, current_face), 0);
    }

    // Construct targets and viewport:
    targetsAndViewport.num              = 1;
	GLuint fboid						= mFBOId[fb];
	unsigned long long	fboid_ull		=(unsigned long long)fboid;
    targetsAndViewport.m_rt[0]          = (void*)(fboid_ull);
    targetsAndViewport.m_dt             = 0;
    targetsAndViewport.viewport.x       = 0;
    targetsAndViewport.viewport.y       = 0;
    targetsAndViewport.viewport.w       = Width;
    targetsAndViewport.viewport.h       = Height;

    deviceContext.renderPlatform->SetViewports(deviceContext, 1, &targetsAndViewport.viewport);

    // Cache it:
    deviceContext.GetFrameBufferStack().push(&targetsAndViewport);
}

void FramebufferGL::SetExternalTextures(crossplatform::Texture* colour, crossplatform::Texture* depth)
{
    BaseFramebuffer::SetExternalTextures(colour, depth);
}

void FramebufferGL::Clear(crossplatform::GraphicsDeviceContext &deviceContext, float r, float g, float b, float a, float d, int mask)
{
    // This call must be made within a Activate - Deactivate block!
    bool changed = false;
    if (!colour_active)
    {
        changed = true;
        Activate(deviceContext);
    }
    glClearColor(r, g, b, a);
    GLenum settings = GL_COLOR_BUFFER_BIT;
    if (buffer_depth_texture)
    {
        glDepthMask(GL_TRUE);
        glClearDepth(d);
        settings |= GL_DEPTH_BUFFER_BIT;
    }
    glClear(settings);

    // Leave it as it was:
    if (changed)
    {
        Deactivate(deviceContext);
    }
}

void FramebufferGL::ClearColour(crossplatform::GraphicsDeviceContext &deviceContext, float r, float g, float b, float a)
{
    SIMUL_BREAK("");
}

void FramebufferGL::Deactivate(crossplatform::GraphicsDeviceContext& deviceContext)
{
    deviceContext.renderPlatform->DeactivateRenderTargets(deviceContext);
    colour_active   = false;
    depth_active    = false;
}

void FramebufferGL::DeactivateDepth(crossplatform::GraphicsDeviceContext &deviceContext)
{
    // This call must be made inside Activate - Deactivate block!
	GLenum target = numAntialiasingSamples == 1 ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE;
    if (depth_active)
    {
        depth_active = false;
		glBindFramebuffer(GL_FRAMEBUFFER, mFBOId[is_cubemap?current_face:0]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, target, 0, 0);
    }
}
