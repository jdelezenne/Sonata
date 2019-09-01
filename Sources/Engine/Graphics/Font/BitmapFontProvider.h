/*=============================================================================
BitmapFontProvider.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_BITMAPFONTPROVIDER_H_
#define _SE_BITMAPFONTPROVIDER_H_

#include "Graphics/Font/FontProvider.h"
#include "Graphics/Common.h"

namespace SonataEngine
{

/**
	Bitmap font provider.
*/
class SE_GRAPHICS_EXPORT BitmapFontProvider : public FontProvider
{
public:
	Image* Image;
	int32 FontHeight;
	int32 Spacing;
	int32 SpaceWidth;

public:
	/** @name Constructors / Destructor. */
	//@{
	BitmapFontProvider();
	virtual ~BitmapFontProvider();
	//@}

	Font* Create();
};

}

#endif 
