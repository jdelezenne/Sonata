/*=============================================================================
Viewport.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Viewport.h"

namespace SonataEngine
{

SE_IMPLEMENT_CLASS(Viewport);
SE_IMPLEMENT_REFLECTION(Viewport);

Viewport::Viewport() :
	_left(0),
	_top(0),
	_width(0),
	_height(0)
{
}

Viewport::Viewport(int32 left, int32 top, int32 width, int32 height) :
	_left(left),
	_top(top),
	_width(width),
	_height(height)
{
}

Viewport::Viewport(const Viewport& value)
{
	_left = value._left;
	_top = value._top;
	_width = value._width;
	_height = value._height;
}

Viewport::~Viewport()
{
}

}
