/*=============================================================================
D3D8IndexBuffer.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D8IndexBuffer.h"

namespace SE_D3D8
{

D3D8IndexBuffer::D3D8IndexBuffer() :
	_Mapped(false),
	_D3DBuffer(NULL)
{
}

D3D8IndexBuffer::D3D8IndexBuffer(IDirect3DIndexBuffer8* buffer) :
	_Mapped(false),
	_D3DBuffer(buffer)
{
}

D3D8IndexBuffer::~D3D8IndexBuffer()
{
	SE_RELEASE(_D3DBuffer);
}

bool D3D8IndexBuffer::IsMapped()
{
	return _Mapped;
}

bool D3D8IndexBuffer::Map(HardwareBufferMode mode, void** data)
{
	if (_D3DBuffer == NULL)
	{
		return false;
	}
	else
	{
		DWORD flags = 0;
		switch (mode)
		{
		case HardwareBufferMode_Normal: flags = 0; break;
		case HardwareBufferMode_ReadOnly: flags = D3DLOCK_READONLY; break;
		case HardwareBufferMode_WriteOnly: flags = D3DLOCK_DISCARD; break;
		case HardwareBufferMode_ReadWrite: flags = D3DLOCK_DISCARD; break;
		}

		_Mapped = SUCCEEDED(_D3DBuffer->Lock(0, 0, (BYTE**)data, flags));
		return _Mapped;
	}
}

void D3D8IndexBuffer::Unmap()
{
	if (!_Mapped)
		return;

	_D3DBuffer->Unlock();
	_Mapped = false;
}

}
