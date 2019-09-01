/*=============================================================================
D3D9IndexBuffer.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D9INDEXBUFFER_H_
#define _SE_D3D9INDEXBUFFER_H_

#include "D3D9RenderSystem.h"

namespace SE_D3D9
{

/** D3D9 hardware buffer. */
class D3D9IndexBuffer : public HardwareBuffer
{
protected:
	IDirect3DIndexBuffer9* _D3DBuffer;
	bool _isMapped;

public:
	D3D9IndexBuffer();
	D3D9IndexBuffer(IDirect3DIndexBuffer9* buffer);
	virtual ~D3D9IndexBuffer();

	virtual bool IsMapped();
	virtual bool Map(HardwareBufferMode mode, void** data);
	virtual void Unmap();

	IDirect3DIndexBuffer9* GetBuffer() const { return _D3DBuffer; }
	void SetBuffer(IDirect3DIndexBuffer9* buffer) { _D3DBuffer = buffer; }
};

}

#endif
