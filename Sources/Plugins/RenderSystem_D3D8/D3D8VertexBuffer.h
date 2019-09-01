/*=============================================================================
D3D8VertexBuffer.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D8VERTEXBUFFER_H_
#define _SE_D3D8VERTEXBUFFER_H_

#include "D3D8RenderSystem.h"

namespace SE_D3D8
{

/** D3D8 hardware buffer. */
class D3D8VertexBuffer : public HardwareBuffer
{
public:
	D3D8VertexBuffer();
	D3D8VertexBuffer(IDirect3DVertexBuffer8* buffer);
	virtual ~D3D8VertexBuffer();

	virtual bool IsMapped();
	virtual bool Map(HardwareBufferMode mode, void** data);
	virtual void Unmap();

	IDirect3DVertexBuffer8* GetBuffer() const { return _D3DBuffer; }
	void SetBuffer(IDirect3DVertexBuffer8* buffer) { _D3DBuffer = buffer; }

protected:
	bool _Mapped;
	IDirect3DVertexBuffer8* _D3DBuffer;
};

}

#endif
