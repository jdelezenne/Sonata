/*=============================================================================
ImageReader.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_IMAGEREADER_H_
#define _SE_IMAGEREADER_H_

#include "Core/Core.h"
#include "Core/IO/Stream.h"
#include "Graphics/Image.h"

namespace SonataEngine
{

enum ImageOptionsFlag
{
	ImageOptions_Format = (1<<0),
	ImageOptions_Size = (1<<1)
};

/** Image reader options. */
struct ImageReaderOptions
{
	PixelFormat Format;
	int32 Width;
	int32 Height;
};

/** Image reader. */
class SE_GRAPHICS_EXPORT ImageReader
{
public:
	virtual Image* LoadImage(Stream& stream, ImageReaderOptions* options = NULL) = 0;
};

}

#endif 
