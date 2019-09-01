/*=============================================================================
ImageHelper.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_IMAGEHELPER_H_
#define _SE_IMAGEHELPER_H_

#include "Graphics/Common.h"
#include "Graphics/Image.h"

namespace SonataEngine
{

enum ImageFlipType
{
	ImageFlipType_X,
	ImageFlipType_Y,
	ImageFlipType_XY
};

enum ImageResizeType
{
	ImageResizeType_Point,
	ImageResizeType_Linear
};

/**
	@brief Image helper.

	Provides operations on images.
*/
class SE_GRAPHICS_EXPORT ImageHelper
{
public:
	/** This method converts the Image. */
	static void Convert(Image* destination, Image* source, const PixelFormatDesc& format);

	/** This method flips the Image. */
	static void Flip(Image* destination, Image* source, ImageFlipType type);

	/** This method resizes the Image. */
	static void Resize(Image* destination, Image* source, ImageResizeType type);
};

}

#endif 
