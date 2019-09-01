/*=============================================================================
ScissorState.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SCISSORSTATE_H_
#define _SE_SCISSORSTATE_H_

#include "Graphics/Common.h"

namespace SonataEngine
{

/**
	Scissor state.
*/
class SE_GRAPHICS_EXPORT ScissorState
{
public:
	/** @name Scissor. */
	//@{
	///Enables or disables fog blending.
	bool Enable;

	/// Retrieves or sets the scissor rectangle.
	RectangleInt Rectangle;
	//@}

public:
	/** @name Constructor. */
	//@{
	ScissorState();
	//@}
};

SE_DECLARE_STRUCT(ScissorState);
SE_BEGIN_STRUCT(ScissorState);
	SE_StructField(Enable, bool);
	SE_StructField(Rectangle, RectangleInt);
SE_END_STRUCT(ScissorState);

}

#endif
