/*=============================================================================
Viewport.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_VIEWPORT_H_
#define _SE_VIEWPORT_H_

#include "Graphics/Common.h"

namespace SonataEngine
{

/**
	@brief Viewport.
*/
class SE_GRAPHICS_EXPORT Viewport : public Object
{
	SE_DECLARE_CLASS(Viewport, Object);
	SE_BEGIN_REFLECTION(Viewport);
		SE_Field(_left, int32, Public);
		SE_Field(_top, int32, Public);
		SE_Field(_width, int32, Public);
		SE_Field(_height, int32, Public);
	SE_END_REFLECTION(Viewport);

protected:
	int32 _left;
	int32 _top;
	int32 _width;
	int32 _height;

public:
	/** @name Constructors / Destructor. */
	//@{
	Viewport();
	Viewport(int32 left, int32 top, int32 width, int32 height);
	Viewport(const Viewport& value);
	virtual ~Viewport();
	//@}

public:
	/** @name Properties. */
	//@{
	int32 GetLeft() const { return _left; }
	void SetLeft(int32 left) { _left = left; }

	int32 GetTop() const { return _top; }
	void SetTop(int32 top) { _top = top; }

	int32 GetWidth() const { return _width; }
	void SetWidth(int32 width) { _width = width; }

	int32 GetHeight() const { return _height; }
	void SetHeight(int32 height) { _height = height; }
	//@}
};

}

#endif
