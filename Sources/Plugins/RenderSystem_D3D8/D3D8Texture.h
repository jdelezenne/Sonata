/*=============================================================================
D3D8Texture.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D8TEXTURE_H_
#define _SE_D3D8TEXTURE_H_

#include "D3D8RenderSystem.h"

namespace SE_D3D8
{

/** D3D8 texture. */
class D3D8Texture : public Texture
{
public:
	D3D8Texture(IDirect3DDevice8* d3dDevice);
	virtual ~D3D8Texture();

	virtual bool Create(Image* image);
	virtual bool Destroy();

	IDirect3DTexture8* GetTexture() const { return _D3DTexture; }

protected:
	IDirect3DDevice8* _D3DDevice;
	IDirect3DTexture8* _D3DTexture;
};

}

#endif
