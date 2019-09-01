/*=============================================================================
BitmapFontProvider.cpp
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#include "BitmapFontProvider.h"

namespace SonataEngine
{

BitmapFontProvider::BitmapFontProvider() :
	FontProvider(),
	SpaceWidth(0)
{
}

BitmapFontProvider::~BitmapFontProvider()
{
}

Font* BitmapFontProvider::Create(Image* image)
{
	Font* font = new Font();

	Font::GlythList::DictionaryIterator it = _Glyths.GetIterator();
	while (it.Next())
	{
	}

	return font;
}

}
