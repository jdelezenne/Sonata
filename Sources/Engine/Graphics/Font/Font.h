/*=============================================================================
Font.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_FONT_H_
#define _SE_FONT_H_

#include "Core/Core.h"
#include "Graphics/Common.h"
#include "Graphics/Image.h"
#include "Graphics/System/Texture.h"
#include "Graphics/Materials/ShaderMaterial.h"
#include "Graphics/Sprite.h"

namespace SonataEngine
{

class SE_GRAPHICS_EXPORT FontGlyth
{
public:
	/** @name Constructors / Destructor. */
	//@{
	FontGlyth();
	FontGlyth(const RectangleInt& rect);
	//@}

public:
	Char Character;
	RectangleInt Rectangle;
};

const int32 FontMaxGlyths = 256;

/**
	@brief Font.

	Represent a font.
*/
class SE_GRAPHICS_EXPORT Font : public RefObject
{
public:
	typedef Dictionary<Char, FontGlyth> GlythList;
	typedef Dictionary<Char, SpritePtr> SpriteList;

protected:
	TexturePtr _texture;
	ShaderMaterialPtr _shader;
	GlythList _glyths;
	int32 _height;
	int32 _spacing;
	int32 _spaceWidth;
	int32 _maxWidth;

public:
	/** @name Constructors / Destructor. */
	//@{
	Font();
	virtual ~Font();
	//@}

	void Build();

	/** @name Properties. */
	//@{
	Texture* GetTexture() const { return _texture; }
	void SetTexture(Texture* value) { _texture = value; }

	ShaderMaterial* GetShader() const { return _shader; }

	void RemoveGlyth(Char c) { _glyths.Remove(c); }
	int GetGlythCount() const { return _glyths.Count(); }
	bool ContainsGlyth(Char c) const;
	FontGlyth GetGlyth(Char c) const;
	void SetGlyth(Char c, FontGlyth glyth) { _glyths[c] = glyth; }

	SpritePtr GetSprite(Char c) const;

	int32 GetHeight() const { return _height; }
	void SetHeight(int32 value) { _height = value; }

	int32 GetSpacing() const { return _spacing; }
	void SetSpacing(int32 value) { _spacing = value; }

	int32 GetSpaceWidth() const { return _spaceWidth; }
	void SetSpaceWidth(int32 value) { _spaceWidth = value; }

	int32 GetMaxWidth() const { return _maxWidth; }
	//@}
};

typedef SmartPtr<Font> FontPtr;

}

#endif 
