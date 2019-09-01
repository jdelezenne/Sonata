/*=============================================================================
FontFamily.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "FontFamily.h"

namespace SonataEngine
{

FontFamily::FontFamily() :
	RefObject()
{
}

FontFamily::FontFamily(const String& faceName, FontStyle fontStyle, int32 weight, int32 height, int32 averageWidth) :
	RefObject(),
	_FaceName(faceName),
	_FontStyle(fontStyle),
	_Weight(weight),
	_Height(height),
	_AverageWidth(averageWidth)
{
}

FontFamily::~FontFamily()
{
}

}
