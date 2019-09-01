/*=============================================================================
RenderContext.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "RenderContext.h"

namespace SonataEngine
{

RenderContext::RenderContext() :
	_window(NULL)
{
}

RenderContext::~RenderContext()
{
}

PixelFormat RenderContext::GetPixelFormat() const
{
	if (_description.Mode.Depth == 24)
		return PixelFormat_R8G8B8;
	else if (_description.Mode.Depth == 32)
		return PixelFormat_R8G8B8A8;
	else
		return PixelFormat_Unknown;
}

int32 RenderContext::GetWidth() const
{
	return _description.Mode.Width;
}

int32 RenderContext::GetHeight() const
{
	return _description.Mode.Height;
}

int32 RenderContext::GetBitsPerPixel() const
{
	return _description.Mode.Depth;
}

bool RenderContext::GetDescription(RenderContextDescription& desc) const
{
	if (!IsValid())
		return false;

	desc = _description;
	return true;
}

}
