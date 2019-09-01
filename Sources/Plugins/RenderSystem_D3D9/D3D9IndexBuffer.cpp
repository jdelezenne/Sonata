/*=============================================================================
D3D9IndexBuffer.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D9IndexBuffer.h"

namespace SE_D3D9
{

D3D9IndexBuffer::D3D9IndexBuffer() :
	HardwareBuffer(),
	_D3DBuffer(NULL),
	_isMapped(false)
{
}

D3D9IndexBuffer::D3D9IndexBuffer(IDirect3DIndexBuffer9* buffer) :
	HardwareBuffer(),
	_D3DBuffer(buffer),
	_isMapped(false)
{
}

D3D9IndexBuffer::~D3D9IndexBuffer()
{
	SE_RELEASE(_D3DBuffer);
}

bool D3D9IndexBuffer::IsMapped()
{
	return _isMapped;
}

bool D3D9IndexBuffer::Map(HardwareBufferMode mode, void** data)
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

void D3D9IndexBuffer::Unmap()
{
	if (!_isMapped)
		return;

	HRESULT hr = _D3DBuffer->Unlock();
	if (FAILED(hr))
	{
		return;
	}

	_isMapped = !SUCCEEDED(_D3DBuffer->Unlock());
}

}
