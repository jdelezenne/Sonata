/*=============================================================================
D3D9VertexBuffer.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D9VERTEXBUFFER_H_
#define _SE_D3D9VERTEXBUFFER_H_

#include "D3D9RenderSystem.h"

namespace SE_D3D9
{

/** D3D9 hardware buffer. */
class D3D9VertexBuffer : public HardwareBuffer
{
protected:
	IDirect3DVertexBuffer9* _D3DBuffer;
	bool _isMapped;

public:
	D3D9VertexBuffer();
	D3D9VertexBuffer(IDirect3DVertexBuffer9* buffer);
	virtual ~D3D9VertexBuffer();

	virtual bool IsMapped();
	virtual bool Map(HardwareBufferMode mode, void** data);
	virtual void Unmap();

	IDirect3DVertexBuffer9* GetBuffer() const { return _D3DBuffer; }
	void SetBuffer(IDirect3DVertexBuffer9* buffer) { _D3DBuffer = buffer; }
};

}

#endif
