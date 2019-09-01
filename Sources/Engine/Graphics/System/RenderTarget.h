/*=============================================================================
RenderTarget.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_RENDERTARGET_H_
#define _SE_RENDERTARGET_H_

#include "Graphics/Common.h"
#include "Graphics/PixelFormat.h"

namespace SonataEngine
{

/** Base class for the render targets. */
class SE_GRAPHICS_EXPORT RenderTarget : public RefObject
{
public:
	/** Destructor. */
	virtual ~RenderTarget();

	virtual bool IsTexture() const = 0;

	virtual PixelFormat GetPixelFormat() const = 0;

	virtual int32 GetWidth() const = 0;

	virtual int32 GetHeight() const = 0;

	virtual int32 GetBitsPerPixel() const = 0;

protected:
	/** Constructor. */
	RenderTarget();
};

}

#endif 
