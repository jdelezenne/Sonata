/*=============================================================================
D3D9Texture.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D9TEXTURE_H_
#define _SE_D3D9TEXTURE_H_

#include "D3D9RenderSystem.h"

namespace SE_D3D9
{

/** D3D9 texture. */
class D3D9Texture : public Texture
{
protected:
	LPDIRECT3DDEVICE9 _D3DDevice;
	IDirect3DTexture9* _D3DTexture;
	IDirect3DCubeTexture9* _D3DCubeTexture;
	IDirect3DVolumeTexture9* _D3DVolumeTexture;
	IDirect3DSurface9* _D3DSurface;
	IDirect3DSurface9* _D3DCubeSurfaces[6];
	IDirect3DVolume9* _D3DVolume;
	bool _isMapped;
	D3DCUBEMAP_FACES _mappedFace;
	int _mappedMipLevel;

public:
	D3D9Texture(LPDIRECT3DDEVICE9 d3dDevice);
	virtual ~D3D9Texture();

	virtual Texture* Clone() const;

	virtual bool Create(TextureType textureType, PixelFormat format, int width, int height, int depth, int mipLevels, TextureUsage usage);
	virtual bool Create(Image* image, TextureUsage usage);
	virtual bool Destroy();

	virtual bool IsMapped();
	virtual bool Map(HardwareBufferMode mode, void** data, int mipLevel = 0);
	virtual bool MapCubeFace(HardwareBufferMode mode, CubeTextureFace face, void** data, int mipLevel = 0);
	virtual void Unmap();

	IDirect3DBaseTexture9* GetD3DBaseTexture() const;
	IDirect3DTexture9* GetD3DTexture() const { return _D3DTexture; }
	IDirect3DCubeTexture9* GetD3DCubeTexture() const { return _D3DCubeTexture; }
	IDirect3DVolumeTexture9* GetD3DVolumeTexture() const { return _D3DVolumeTexture; }
	IDirect3DSurface9* GetD3DSurface() const;
	IDirect3DSurface9* GetD3DCubeFaceSurface(CubeTextureFace face) const;
	IDirect3DVolume9* GetD3DVolume() const;
};

}

#endif
