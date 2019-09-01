/*=============================================================================
D3D8RenderContext.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D8RENDERCONTEXT_H_
#define _SE_D3D8RENDERCONTEXT_H_

#include "D3D8RenderSystem.h"

namespace SE_D3D8
{

/** D3D8 rendering context. */
class D3D8RenderContext : public RenderContext
{
public:
	D3D8RenderContext();
	virtual ~D3D8RenderContext();

	virtual bool IsValid() const;

	virtual bool Create(Window* window, const RenderContextDescription& desc);
	virtual bool Destroy();

	virtual void SwapBuffers();

	IDirect3DDevice8* GetDevice() const { return _D3DDevice; }
	void SetDevice(IDirect3DDevice8* value) { _D3DDevice = value; }
	IDirect3DSwapChain8* GetSwapChain() const { return _D3DSwapChain; }

protected:
	bool _Valid;
	IDirect3DDevice8* _D3DDevice;
	IDirect3DSwapChain8* _D3DSwapChain;
};

}

#endif 
