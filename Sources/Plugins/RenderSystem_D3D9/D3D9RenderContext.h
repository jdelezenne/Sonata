/*=============================================================================
D3D9RenderContext.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D9RENDERCONTEXT_H_
#define _SE_D3D9RENDERCONTEXT_H_

#include "D3D9RenderSystem.h"

namespace SE_D3D9
{

/** D3D9 rendering context. */
class D3D9RenderContext : public RenderContext
{
protected:
	bool _isValid;
	IDirect3DDevice9* _D3DDevice;
	IDirect3DSwapChain9* _D3DSwapChain;

public:
	D3D9RenderContext();
	virtual ~D3D9RenderContext();

	virtual bool IsValid() const;

	virtual bool Create(Window* window, const RenderContextDescription& desc);
	virtual bool Destroy();

	virtual void SwapBuffers(WindowHandle handle = NULL);

	virtual RectangleInt GetSafeArea() const;

	IDirect3DDevice9* GetDevice() const { return _D3DDevice; }
	void SetDevice(IDirect3DDevice9* value) { _D3DDevice = value; }
	IDirect3DSwapChain9* GetSwapChain() const { return _D3DSwapChain; }
};

}

#endif 
