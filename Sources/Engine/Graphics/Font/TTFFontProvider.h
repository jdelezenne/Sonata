/*=============================================================================
BitmapFontProvider.h
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#ifndef _BITMAPFONTPROVIDER_H_
#define _BITMAPFONTPROVIDER_H_

#include "Graphics/Font/FontProvider.h"

namespace SonataEngine
{

/**
	Bitmap font provider.
*/
class BitmapFontProvider : public FontProvider
{
public:
	/** Constructors / Destructor. */
	//@{
	BitmapFontProvider();
	virtual ~BitmapFontProvider();
	//@}

	Font* Create();

public:
	Image* Image;
	int32 FontHeight;
	int32 SpaceWidth;
	Font::GlythList _Glyths;
};

}

#endif 
