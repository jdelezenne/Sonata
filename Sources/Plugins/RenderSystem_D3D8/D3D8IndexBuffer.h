/*=============================================================================
D3D8IndexBuffer.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D8INDEXBUFFER_H_
#define _SE_D3D8INDEXBUFFER_H_

#include "D3D8RenderSystem.h"

namespace SE_D3D8
{

/** D3D8 hardware buffer. */
class D3D8IndexBuffer : public HardwareBuffer
{
public:
	D3D8IndexBuffer();
	D3D8IndexBuffer(IDirect3DIndexBuffer8* buffer);
	virtual ~D3D8IndexBuffer();

	virtual bool IsMapped();
	virtual bool Map(HardwareBufferMode mode, void** data);
	virtual void Unmap();

	IDirect3DIndexBuffer8* GetBuffer() const { return _D3DBuffer; }
	void SetBuffer(IDirect3DIndexBuffer8* buffer) { _D3DBuffer = buffer; }

protected:
	bool _Mapped;
	IDirect3DIndexBuffer8* _D3DBuffer;
};

}

#endif
