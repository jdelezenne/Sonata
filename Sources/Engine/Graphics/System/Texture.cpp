/*=============================================================================
Texture.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Texture.h"

namespace SonataEngine
{

Texture::Texture() :
	RefObject(),
	_textureType(TextureType_Texture2D),
	_textureUsage(TextureUsage_Static),
	_width(0),
	_height(0),
	_depth(0),
	_mipLevels(0)
{
}

Texture::~Texture()
{
}

int32 Texture::GetFaceCount() const
{
	if (_textureType == TextureType_TextureCube)
	{
		return 6;
	}
	else
	{
		return 1;
	}
}

}
