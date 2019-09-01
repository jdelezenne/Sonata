/*=============================================================================
Font.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Font.h"
#include "Graphics/Materials/DefaultMaterial.h"

namespace SonataEngine
{

FontGlyth::FontGlyth()
{
}

FontGlyth::FontGlyth(const RectangleInt& rect) :
	Rectangle(rect)
{
}


Font::Font() :
	RefObject(),
	_texture(NULL),
	_shader(NULL),
	_glyths(),
	_height(0),
	_spacing(0),
	_spaceWidth(0),
	_maxWidth(0)
{
}

Font::~Font()
{
}

void Font::Build()
{
	if (_texture == NULL)
		return;

	if (_glyths.IsEmpty())
		return;

	_maxWidth = 0;

	DefaultMaterial* shader = new DefaultMaterial();
	FFPPass* pass = (FFPPass*)shader->GetTechnique()->GetPassByIndex(0);
	pass->RasterizerState.CullMode = CullMode_None;
	pass->DepthState.Enable = false;
	pass->DepthState.WriteEnable = false;
	pass->AlphaState.BlendEnable[0] = true;
	pass->AlphaState.SourceBlend = BlendMode_SourceAlpha;
	pass->AlphaState.DestinationBlend = BlendMode_InvSourceAlpha;
	pass->LightState.Lighting = false;
	SamplerState* samplerState = pass->GetSamplerStateByIndex(0);
	samplerState->SetTexture(_texture);
	samplerState->MinFilter = TextureFilterType_Linear;
	samplerState->MagFilter = TextureFilterType_Linear;
	_shader = shader;

	Font::GlythList::DictionaryIterator it = _glyths.GetIterator();
	while (it.Next())
	{
		if (it.Value().Rectangle.GetSize().Width > _maxWidth)
		{
			_maxWidth = it.Value().Rectangle.GetSize().Width;
		}
	}
}

bool Font::ContainsGlyth(Char c) const
{
	if (_glyths.Contains(c))
		return true;
	else if (Char::IsLower(c) && _glyths.Contains(Char::ToUpper(c)))
		return true;
	else if (Char::IsUpper(c) && _glyths.Contains(Char::ToLower(c)))
		return true;
	else
		return false;
}

FontGlyth Font::GetGlyth(Char c) const
{
	if (_glyths.Contains(c))
		return _glyths[c];
	else if (Char::IsLower(c) && _glyths.Contains(Char::ToUpper(c)))
		return _glyths[Char::ToUpper(c)];
	else if (Char::IsUpper(c) && _glyths.Contains(Char::ToLower(c)))
		return _glyths[Char::ToLower(c)];
	else
		return FontGlyth();
}

}
