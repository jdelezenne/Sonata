/*=============================================================================
FontFamily.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_FONTFAMILY_H_
#define _SE_FONTFAMILY_H_

#include "Core/Core.h"
#include "Graphics/Common.h"
#include "Graphics/Font/Font.h"

namespace SonataEngine
{

/** Font Styles
	Specifies style information applied to text.
	[Flags]
*/
enum FontStyle
{
	/// Normal text.
	FontStyle_Regular = 0,

	/// Bold text.
	FontStyle_Bold = 1,
	
	/// Italic text.
	FontStyle_Italic = 2,
	
	/// Underlined text.
	FontStyle_Underline = 4,
	
	/// Text with a line through the middle.
	FontStyle_Strikeout = 8
};

/**
	Font family.
*/
class SE_GRAPHICS_EXPORT FontFamily : public RefObject
{
public:
	/** @name Constructors / Destructor. */
	//@{
	FontFamily();
	FontFamily(const String& faceName, FontStyle fontStyle, int32 weight, int32 height, int32 averageWidth);
	virtual ~FontFamily();
	//@}

	/** @name Properties. */
	//@{
	/// Gets the face name of this FontFamily object.
	String GetFaceName() const { return _FaceName; }

	/// Gets style information for this Font object.
	FontStyle GetFontStyle() const { return _FontStyle; }

	/// Gets a value that indicates whether this Font object is bold.
	bool Bold() const { return (_FontStyle & FontStyle_Bold) != 0; }

	/// Gets a value that indicates whether this Font object is italic.
	bool Italic() const { return (_FontStyle & FontStyle_Italic) != 0; }

	/// Gets a value that indicates whether this Font object is underlined.
	bool Underline() const { return (_FontStyle & FontStyle_Underline) != 0; }

	/// Gets a value that indicates whether this Font object specifies a horizontal line through the font.
	bool Strikeout() const { return (_FontStyle & FontStyle_Strikeout) != 0; }

	int32 GetWeight() const { return _Weight; }
	int32 GetHeight() const { return _Height; }
	int32 GetAverageWidth() const { return _AverageWidth; }
	//@}

protected:
	String _FaceName;
	FontStyle _FontStyle;
	int32 _Weight;
	int32 _Height;
	int32 _AverageWidth;
};

}

#endif 
