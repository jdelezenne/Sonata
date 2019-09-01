/*=============================================================================
D3D8Texture.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D8Texture.h"
#include "D3D8Helper.h"

namespace SE_D3D8
{

D3D8Texture::D3D8Texture(IDirect3DDevice8* d3dDevice) :
	_D3DDevice(d3dDevice),
	_D3DTexture(NULL)
{
}

D3D8Texture::~D3D8Texture()
{
	Destroy();
}

bool D3D8Texture::Create(Image* image)
{
	if (_D3DDevice == NULL)
		return false;

	/*D3DFORMAT d3dFormat = D3D8Helper::GetFormat(image->GetFormat());
	if (d3dFormat == D3DFMT_UNKNOWN)
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D8Texture.Create"),
			_T("Format not supported."));
		return false;
	}*/

	int32 width = image->GetWidth();
	int32 height = image->GetHeight();
	int32 bits = image->GetBitsPerPixel();
	int32 bitDepth = bits / 8;

	D3DFORMAT d3dFormat;
	if (bitDepth == 3)
		d3dFormat = D3DFMT_X8R8G8B8;
	else
		d3dFormat = D3DFMT_A8R8G8B8;

	HRESULT hr = D3DXCreateTexture(_D3DDevice, width, height, image->GetMipLevels(),
		0, d3dFormat, D3DPOOL_MANAGED, &_D3DTexture);

	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D8Texture.Create"),
			_T("Failed to create the texture."));
		return false;
	}

	// D3DX might have used other settings, so we get them back
	D3DSURFACE_DESC d3dDesc;
	_D3DTexture->GetLevelDesc(0, &d3dDesc);

	_Width = d3dDesc.Width;
	_Height = d3dDesc.Height;
	_BitsPerPixel = D3D8Helper::GetBitDepth(d3dDesc.Format) * 8;

	// convert the texture
	D3DLOCKED_RECT rect;
	if (FAILED(_D3DTexture->LockRect(0, &rect, 0, 0)))
		return false;
/*
	PixelFormat format;
	if (bitDepth == 3)
		format = PixelFormat::X8R8G8B8;
	else
		format = PixelFormat::A8R8G8B8;
*/
	//if (image->GetFormat() == format)
	//{
		Memory::Copy((SEbyte*)rect.pBits, image->GetData(), image->GetDataSize());
	/*}
	else
	{
		Image* destImage = new Image();
		destImage->Create(format, _Width, _Height);

		ImageHelper::Convert(destImage, image, format);

		Memory::Copy((SEbyte*)rect.pBits, destImage->GetData(), destImage->GetDataSize());
		delete destImage;
	}*/

	/*SEbyte* line;
	for (int y=0; y<_Height; ++y)
	{
		line = dest;
		for (int x=0; x<_Width; ++x)
		{
			Memory::Copy(dest+1, data, bitDepth);
			dest += destBitDepth;
			data += bitDepth;
		}
		dest = line + rect.Pitch;
	}*/

	_D3DTexture->UnlockRect(0);

	return true;
}

bool D3D8Texture::Destroy()
{
	SE_RELEASE(_D3DTexture);

	return true;
}

}
