/*=============================================================================
BitmapFontProvider.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "BitmapFontProvider.h"
#include "Graphics/System/RenderSystem.h"

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

Font* BitmapFontProvider::Create()
{
	if (Image == NULL)
		return NULL;

	Font* font = new Font();

	Texture* texture;
	if (!RenderSystem::Current()->CreateTexture(&texture))
		return NULL;

	if (!texture->Create(Image, TextureUsage_Static))
	{
		SE_DELETE(texture);
		return NULL;
	}

	font->SetTexture(texture);

	Font::GlythList::DictionaryIterator it = Glyths.GetIterator();
	while (it.Next())
	{
		font->SetGlyth(it.Key(), it.Value());
	}

	font->SetHeight(FontHeight);
	font->SetSpacing(Spacing);
	font->SetSpaceWidth(SpaceWidth);
	font->Build();

	return font;
}

}
