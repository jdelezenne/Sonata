/*=============================================================================
ScissorState.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ScissorState.h"

namespace SonataEngine
{

SE_IMPLEMENT_STRUCT(ScissorState);

ScissorState::ScissorState() :
	Enable(false),
	Rectangle(RectangleInt(0, 0, 0, 0))
{
}

}
