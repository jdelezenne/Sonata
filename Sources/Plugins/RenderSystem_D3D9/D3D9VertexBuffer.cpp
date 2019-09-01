/*=============================================================================
D3D9VertexBuffer.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D9VertexBuffer.h"

namespace SE_D3D9
{

D3D9VertexBuffer::D3D9VertexBuffer() :
	HardwareBuffer(),
	_D3DBuffer(NULL),
	_isMapped(false)
{
}

D3D9VertexBuffer::D3D9VertexBuffer(IDirect3DVertexBuffer9* buffer) :
	HardwareBuffer(),
	_D3DBuffer(buffer),
	_isMapped(false)
{
}

D3D9VertexBuffer::~D3D9VertexBuffer()
{
	SE_RELEASE(_D3DBuffer);
}

bool D3D9VertexBuffer::IsMapped()
{
	return _isMapped;
}

bool D3D9VertexBuffer::Map(HardwareBufferMode mode, void** data)
{
	if (_D3DBuffer == NULL)
	{
		return false;
	}

	DWORD flags = 0;
	switch (mode)
	{
	case HardwareBufferMode_Normal: flags = 0; break;
	case HardwareBufferMode_ReadOnly: flags = D3DLOCK_READONLY; break;
	case HardwareBufferMode_WriteOnly: flags = D3DLOCK_DISCARD; break;
	case HardwareBufferMode_ReadWrite: flags = D3DLOCK_DISCARD; break;
	}

	_isMapped = SUCCEEDED(_D3DBuffer->Lock(0, 0, data, flags));
	return _isMapped;
}

void D3D9VertexBuffer::Unmap()
{
	if (!_isMapped)
		return;

	_isMapped = !SUCCEEDED(_D3DBuffer->Unlock());
}

}
