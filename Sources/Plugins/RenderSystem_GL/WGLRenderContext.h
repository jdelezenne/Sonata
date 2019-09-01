/*=============================================================================
WGLRenderContext.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_WGLRENDERCONTEXT_H_
#define _SE_WGLRENDERCONTEXT_H_

#include "GLRenderSystem.h"

namespace SE_GL
{

/** WGL rendering context. */
class WGLRenderContext : public GLRenderContext
{
public:
	WGLRenderContext();
	virtual ~WGLRenderContext();

	virtual bool IsValid() const;

	virtual bool Create(Window* window, const RenderContextDescription& desc);
	virtual bool Destroy();

	virtual void SwapBuffers();

protected:
	bool _Valid;
	HWND _hWnd;
	HDC _hDC;
	HGLRC _hGLRC;
};

}

#endif 
