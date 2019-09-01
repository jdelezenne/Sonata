/*=============================================================================
RenderTexture.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_RENDERTEXTURE_H_
#define _SE_RENDERTEXTURE_H_

#include "Graphics/Common.h"
#include "Graphics/System/RenderTarget.h"
#include "Graphics/System/Texture.h"

namespace SonataEngine
{

/** Render Texture. */
class SE_GRAPHICS_EXPORT RenderTexture : public RenderTarget 
{
protected:
	Texture* _texture;

public:
	/** Constructor. */
	RenderTexture();

	/** Destructor. */
	virtual ~RenderTexture();

	virtual bool IsTexture() const { return true; }

	virtual PixelFormat GetPixelFormat() const
	{
		return (_texture != NULL ? _texture->GetFormat() : PixelFormat_Unknown);
	}

	virtual int32 GetWidth() const
	{
		return (_texture != NULL ? _texture->GetWidth() : 0);
	}

	virtual int32 GetHeight() const
	{
		return (_texture != NULL ? _texture->GetHeight() : 0);
	}

	virtual int32 GetBitsPerPixel() const
	{
		return (_texture != NULL ? _texture->GetBitsPerPixel() : 0);
	}

	Texture* GetTexture() const { return _texture; }
	void SetTexture(Texture* value) { _texture = value; }
};

}

#endif 
