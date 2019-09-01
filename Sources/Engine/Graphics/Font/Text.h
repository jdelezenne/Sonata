/*=============================================================================
Text.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_TEXT_H_
#define _SE_TEXT_H_

#include "Core/Core.h"
#include "Graphics/Common.h"
#include "Graphics/Font/Font.h"

#ifdef DrawText
#undef DrawText
#endif

namespace SonataEngine
{

/** Text Alignment
	Specifies the alignment of a text string relative to its layout rectangle.
*/
enum HorizontalAlignment
{
	HorizontalAlignment_Left, /// Specifies the text be aligned on the left side of the layout.
	HorizontalAlignment_Center, /// Specifies that text is aligned in the center of the layout. 
	HorizontalAlignment_Right /// Sppecifies that text is aligned on the right side of the layout.
};

/** Text Alignment
	Specifies the alignment of a text string relative to its layout rectangle.
*/
enum VerticalAlignment
{
	VerticalAlignment_Top, /// Specifies the text be aligned on the top side of the layout.
	VerticalAlignment_Center, /// Specifies that text is aligned in the center of the layout. 
	VerticalAlignment_Bottom /// Sppecifies that text is aligned on the bottom side of the layout.
};

/** Text Format
	[Flags]
	Specifies the display and layout information for text strings.
*/
enum TextFormatFlags
{
	TextFormatFlags_DirectionRightToLeft = (1<<0), /// Specifies that text is right to left.
	TextFormatFlags_DirectionVertical = (1<<1), /// Specifies that text is vertical.
	TextFormatFlags_NoClip = (1<<2), /// Overhanging parts of glyphs, and unwrapped text reaching outside the formatting rectangle are allowed to show.
	TextFormatFlags_NoWrap = (1<<3), /// Disables wrapping of text between lines when formatting within a rectangle.
	TextFormatFlags_WordBreak = (1<<4) /// Breaks the text at the end of a word.
};

struct TextFormat
{
	HorizontalAlignment HAlignment;
	VerticalAlignment VAlignment;
	TextFormatFlags Flags;
	Char DefaultGlyth;

	TextFormat()
	{
		HAlignment = HorizontalAlignment_Left;
		VAlignment = VerticalAlignment_Top;
		Flags = TextFormatFlags_WordBreak;
		DefaultGlyth = Char::Space;
	}
};

/**
	@brief Text.

	Renderable text.
*/
class SE_GRAPHICS_EXPORT Text : public RefObject
{
public:
	/** @name Constructors / Destructor. */
	//@{
	Text();
	virtual ~Text();
	//@}

	virtual void Render();

	/** @name Properties. */
	//@{
	String GetText() const { return _Text; }
	void SetText(const String& value);

	Font* GetFont() const { return _Font; }
	void SetFont(Font* value) { _Font = value; }

	Color32 GetColor() const { return _Color; }
	void SetColor(const Color32& value);

	RectangleReal& GetBounds() { return _Bounds; }
	void SetBounds(const RectangleReal& value) { _Bounds = value; }

	void SetPosition(const PointReal& value) { _Bounds.SetLocation(value); }

	TextFormat GetFormat() const { return _Format; }
	void SetFormat(TextFormat value) { _Format = value; }

	real32 GetScale() const { return _Scale; }
	void SetScale(real32 value) { _Scale = value; }
	//@}

	/// Draws the specified text within the specified bounds using the specified font, color, and format.
	static void DrawText(const String& text, Font* font, const PointReal& location, const Color32& color);
	static void DrawText(const String& text, Font* font, const RectangleReal& bounds, const Color32& color);
	static void DrawText(const String& text, Font* font, const PointReal& location, const Color32& color, TextFormat format);
	static void DrawText(const String& text, Font* font, const RectangleReal& bounds, const Color32& color, TextFormat format);

	/// Measures the specified text when drawn with the specified font.
	static SizeReal MeasureText(const String& text, Font* font);
	static SizeReal MeasureText(const String& text, Font* font, TextFormat format);

protected:
	void CreateMesh();
	void UpdateMesh();
	void UpdateColor();

protected:
	String _Text;
	FontPtr _Font;
	Color32 _Color;
	RectangleReal _Bounds;
	TextFormat _Format;
	real32 _Scale;

	bool _NeedUpdateMesh;
	bool _NeedUpdateColor;
	VertexLayoutPtr _vertexLayout;
	MeshPtr _mesh;
};

typedef SmartPtr<Text> TextPtr;

}

#endif 
