/*=============================================================================
D3D9Texture.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D9Texture.h"
#include "D3D9Helper.h"

namespace SE_D3D9
{

D3D9Texture::D3D9Texture(LPDIRECT3DDEVICE9 d3dDevice) :
	Texture(),
	_D3DDevice(d3dDevice),
	_D3DTexture(NULL),
	_D3DCubeTexture(NULL),
	_D3DVolumeTexture(NULL),
	_isMapped(false)
{
	_D3DSurface = NULL;
	Memory::Zero(_D3DCubeSurfaces, sizeof(IDirect3DSurface9*) * 6);
	_D3DVolume = NULL;
}

D3D9Texture::~D3D9Texture()
{
	Destroy();
}

Texture* D3D9Texture::Clone() const
{
	D3D9Texture* texture = new D3D9Texture(_D3DDevice);

	texture->_D3DTexture = _D3DTexture;
	texture->_D3DCubeTexture = _D3DCubeTexture;
	texture->_D3DVolumeTexture = _D3DVolumeTexture;
	texture->_isMapped = _isMapped;

	return texture;
}

bool D3D9Texture::Create(TextureType textureType, PixelFormat format, int width, int height, int depth, int mipLevels, TextureUsage usage)
{
	Destroy();

	if (_D3DDevice == NULL)
		return false;

	DWORD d3dUsage = 0;
	switch (usage)
	{
	case TextureUsage_Dynamic:
		d3dUsage = D3DUSAGE_DYNAMIC;
		break;

	case TextureUsage_RenderTarget:
		d3dUsage = D3DUSAGE_RENDERTARGET;
		break;

	case TextureUsage_DepthStencil:
		d3dUsage = D3DUSAGE_DEPTHSTENCIL;
		break;
	}

	D3DFORMAT d3dFormat;
	int bitDepth = PixelFormatDesc(format).GetDepth();
	if (bitDepth == 3)
		d3dFormat = D3DFMT_X8R8G8B8;
	else
		d3dFormat = D3DFMT_A8R8G8B8;

	UINT d3dMipLevels;
	if (mipLevels == -1)
		d3dMipLevels = D3DX_DEFAULT;
	else
		d3dMipLevels = mipLevels;

	HRESULT hr;
	switch (textureType)
	{
	case TextureType_Texture1D:
	case TextureType_Texture2D:
		hr = D3DXCreateTexture(_D3DDevice, width, height, d3dMipLevels,
			d3dUsage, d3dFormat, D3DPOOL_MANAGED, &_D3DTexture);
		break;

	case TextureType_TextureCube:
		hr = D3DXCreateCubeTexture(_D3DDevice, width, d3dMipLevels,
			d3dUsage, d3dFormat, D3DPOOL_MANAGED, &_D3DCubeTexture);
		break;

	case TextureType_TextureVolume:
		hr = D3DXCreateVolumeTexture(_D3DDevice, width, height, depth, d3dMipLevels,
			d3dUsage, d3dFormat, D3DPOOL_MANAGED, &_D3DVolumeTexture);
		break;

	default:
		return false;
	}

	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D9Texture.Create"),
			_T("Failed to create the texture."));
		return false;
	}

	_textureType = textureType;
	_textureUsage = usage;

	// D3DX might have used other settings, so we get them back
	if (_textureType == TextureType_Texture1D || _textureType == TextureType_Texture2D)
	{
		D3DSURFACE_DESC d3dDesc;
		_D3DTexture->GetLevelDesc(0, &d3dDesc);
		_width = d3dDesc.Width;
		_height = d3dDesc.Height;
		_bitsPerPixel = D3D9Helper::GetBitDepth(d3dDesc.Format) * 8;
	}
	else if (_textureType == TextureType_TextureCube)
	{
		D3DSURFACE_DESC d3dDesc;
		_D3DCubeTexture->GetLevelDesc(0, &d3dDesc);
		_width = d3dDesc.Width;
		_height = d3dDesc.Height;
		_bitsPerPixel = D3D9Helper::GetBitDepth(d3dDesc.Format) * 8;
	}
	else
	{
		D3DVOLUME_DESC d3dDesc;
		_D3DVolumeTexture->GetLevelDesc(0, &d3dDesc);
		_width = d3dDesc.Width;
		_height = d3dDesc.Height;
		_depth = d3dDesc.Depth;
		_bitsPerPixel = D3D9Helper::GetBitDepth(d3dDesc.Format) * 8;
	}

	if (_textureType == TextureType_Texture1D || _textureType == TextureType_Texture2D)
	{
		_D3DTexture->GetSurfaceLevel(0, &_D3DSurface);
	}
	else if (_textureType == TextureType_TextureCube)
	{
		_D3DCubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_X,
			0, &_D3DCubeSurfaces[CubeTextureFace_PositiveX]);
		_D3DCubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_X,
			0, &_D3DCubeSurfaces[CubeTextureFace_NegativeX]);
		_D3DCubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_Y,
			0, &_D3DCubeSurfaces[CubeTextureFace_PositiveY]);
		_D3DCubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_Y,
			0, &_D3DCubeSurfaces[CubeTextureFace_NegativeY]);
		_D3DCubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_Z,
			0, &_D3DCubeSurfaces[CubeTextureFace_PositiveZ]);
		_D3DCubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_Z,
			0, &_D3DCubeSurfaces[CubeTextureFace_NegativeZ]);
	}
	else if (_textureType == TextureType_TextureVolume)
	{
		_D3DVolumeTexture->GetVolumeLevel(0, &_D3DVolume);
	}

	return true;
}

bool D3D9Texture::Create(Image* image, TextureUsage usage)
{
	PixelFormat format = image->GetFormat();
	int32 width = image->GetWidth();
	int32 height = image->GetHeight();
	int32 depth = image->GetDepth();
	int32 mipLevels = image->GetMipLevels();
	int32 bits = image->GetBitsPerPixel();
	int32 bitDepth = bits / 8;

	if (width == 0 || height == 0 || depth == 0)
		return false;

	TextureType textureType;
	if ((width == 1 || height == 1) && depth == 1)
	{
		textureType = TextureType_Texture1D;
	}
	else if (width == height && height == depth)
	{
		textureType = TextureType_TextureCube;
	}
	else if (depth > 1)
	{
		textureType = TextureType_TextureVolume;
	}
	else
	{
		textureType = TextureType_Texture2D;
	}

	if (!Create(textureType, format, width, height, depth, mipLevels, usage))
		return false;

	// Convert the texture
	SEbyte* data;
	if (!Map(HardwareBufferMode_ReadWrite, (void**)&data))
		return false;

	Memory::Copy(data, image->GetData(), image->GetDataSize());

	Unmap();

	return true;
}

bool D3D9Texture::Destroy()
{
	SE_RELEASE(_D3DSurface);
	SE_RELEASE(_D3DCubeSurfaces[0]);
	SE_RELEASE(_D3DCubeSurfaces[1]);
	SE_RELEASE(_D3DCubeSurfaces[2]);
	SE_RELEASE(_D3DCubeSurfaces[3]);
	SE_RELEASE(_D3DCubeSurfaces[4]);
	SE_RELEASE(_D3DCubeSurfaces[5]);
	SE_RELEASE(_D3DVolume);

	SE_RELEASE(_D3DTexture);
	SE_RELEASE(_D3DCubeTexture);
	SE_RELEASE(_D3DVolumeTexture);

	return true;
}

bool D3D9Texture::IsMapped()
{
	return _isMapped;
}

bool D3D9Texture::Map(HardwareBufferMode mode, void** data, int mipLevel)
{
	if (_textureType == TextureType_TextureCube)
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

	D3DLOCKED_RECT lockedRect;
	D3DLOCKED_BOX lockedBox;
	HRESULT hr;
	switch (_textureType)
	{
	case TextureType_Texture1D:
	case TextureType_Texture2D:
		if (_D3DTexture == NULL)
		{
			return false;
		}
		hr = _D3DTexture->LockRect(mipLevel, &lockedRect, NULL, flags);
		break;

	case TextureType_TextureVolume:
		if (_D3DVolumeTexture == NULL)
		{
			return false;
		}
		hr = _D3DVolumeTexture->LockBox(mipLevel, &lockedBox, NULL, flags);
		break;

	default:
		return false;
	}

	if (FAILED(hr))
	{
		return false;
	}

	if (_textureType == TextureType_TextureVolume)
		*data = lockedBox.pBits;
	else
		*data = lockedRect.pBits;

	_isMapped = true;
	return true;
}

bool D3D9Texture::MapCubeFace(HardwareBufferMode mode, CubeTextureFace face, void** data, int mipLevel)
{
	if (_textureType != TextureType_TextureCube)
	{
		return false;
	}

	D3DCUBEMAP_FACES d3dFace;
	switch (face)
	{
	case CubeTextureFace_PositiveX:
		d3dFace = D3DCUBEMAP_FACE_POSITIVE_X;
		break;
	case CubeTextureFace_NegativeX:
		d3dFace = D3DCUBEMAP_FACE_NEGATIVE_Y;
		break;
	case CubeTextureFace_PositiveY:
		d3dFace = D3DCUBEMAP_FACE_POSITIVE_Y;
		break;
	case CubeTextureFace_NegativeY:
		d3dFace = D3DCUBEMAP_FACE_NEGATIVE_Y;
		break;
	case CubeTextureFace_PositiveZ:
		d3dFace = D3DCUBEMAP_FACE_POSITIVE_Z;
		break;
	case CubeTextureFace_NegativeZ:
		d3dFace = D3DCUBEMAP_FACE_NEGATIVE_Z;
		break;
	default:
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

	if (_D3DCubeTexture == NULL)
	{
		return false;
	}

	D3DLOCKED_RECT lockedRect;
	HRESULT hr = _D3DCubeTexture->LockRect(d3dFace, mipLevel, &lockedRect, NULL, flags);
	if (FAILED(hr))
	{
		return false;
	}

	*data = lockedRect.pBits;

	_isMapped = true;
	_mappedFace = d3dFace;
	_mappedMipLevel = mipLevel;
	return true;
}

void D3D9Texture::Unmap()
{
	if (!_isMapped)
		return;

	HRESULT hr;
	switch (_textureType)
	{
	case TextureType_Texture1D:
	case TextureType_Texture2D:
		if (_D3DTexture == NULL)
		{
			return;
		}
		hr = _D3DTexture->UnlockRect(_mappedMipLevel);
		break;

	case TextureType_TextureCube:
		if (_D3DCubeTexture == NULL)
		{
			return;
		}
		hr = _D3DCubeTexture->UnlockRect(_mappedFace, _mappedMipLevel);
		break;

	case TextureType_TextureVolume:
		if (_D3DVolumeTexture == NULL)
		{
			return;
		}
		hr = _D3DVolumeTexture->UnlockBox(_mappedMipLevel);
		break;

	default:
		return;
	}

	_isMapped = !SUCCEEDED(hr);
}

IDirect3DBaseTexture9* D3D9Texture::GetD3DBaseTexture() const
{
	switch (_textureType)
	{
	case TextureType_Texture1D:
	case TextureType_Texture2D:
		return _D3DTexture;
	case TextureType_TextureCube:
		return _D3DCubeTexture;
	case TextureType_TextureVolume:
		return _D3DVolumeTexture;
	default:
		return NULL;
	}
}

IDirect3DSurface9* D3D9Texture::GetD3DSurface() const
{
	if (_textureType != TextureType_Texture1D && _textureType != TextureType_Texture2D)
	{
		return NULL;
	}

	return _D3DSurface;
}

IDirect3DSurface9* D3D9Texture::GetD3DCubeFaceSurface(CubeTextureFace face) const
{
	if (_textureType != TextureType_TextureCube)
	{
		return NULL;
	}

	return _D3DCubeSurfaces[face];
}

IDirect3DVolume9* D3D9Texture::GetD3DVolume() const
{
	if (_textureType != TextureType_TextureVolume)
	{
		return NULL;
	}

	return _D3DVolume;
}

}
