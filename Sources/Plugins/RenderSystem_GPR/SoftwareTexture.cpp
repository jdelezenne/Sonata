/*=============================================================================
SoftwareTexture.cpp
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#include "SoftwareTexture.h"
using namespace Teesside;

SoftwareTexture::SoftwareTexture(OGPRRenderer* renderer) :
	_Renderer(renderer),
	_Buffer(NULL)
{
}

SoftwareTexture::~SoftwareTexture()
{
	Destroy();
}

bool SoftwareTexture::Create(Image* image)
{
	Destroy();

	_Width = image->GetWidth();
	_Height = image->GetHeight();
	_Bpp = image->GetBpp() * 8;

	UINT bufferDimensions[] = { _Width, _Height };
	if (_Renderer->CreateBuffer(2, bufferDimensions,
		GPRDECLTYPE_BYTE, (_Bpp/8), &_Buffer) != GPRRESULT_OK)
	{
		return false;
	}

	uint32 size = _Width * _Height * (_Bpp/8);
	byte* data = image->GetData();

	byte* gprdata;
	_Buffer->Lock(GPRLOCK_WRITEONLY, (void**)&gprdata);

	// Swap upside down
	int stride = (_Bpp/8) * _Width;
	Teesside::byte* swap = new Teesside::byte[stride];
	for (int i = 0; i < _Height; i++)
	{
		memcpy(swap, (void*)&data[i * stride], stride);
		int index = size - (i+1) * stride;
		memcpy((void*)&gprdata[(size - (i+1) * stride)], swap, stride);
	}

	_Buffer->Unlock((void**)&gprdata);

	return true;
}

bool SoftwareTexture::Destroy()
{
	SAFE_RELEASE(_Buffer);

	return true;
}
