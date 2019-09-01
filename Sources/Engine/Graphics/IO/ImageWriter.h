/*=============================================================================
ImageWriter.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_IMAGEWRITER_H_
#define _SE_IMAGEWRITER_H_

#include "Core/IO/Stream.h"
#include "Graphics/Image.h"

namespace SonataEngine
{

/** Image writer options. */
struct ImageWriterOptions
{
};

/** Image writer. */
class SE_GRAPHICS_EXPORT ImageWriter
{
public:
	virtual bool SaveImage(Stream& stream, Image* image, ImageWriterOptions* options = NULL) = 0;
};

}

#endif 
