/*=============================================================================
ScissorTest.cpp
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#include "Software/SoftwareRenderer.h"

bool SoftwareRenderer::ScissorTest(OGPRFragmentFuncEnv& env, const Vector4& position)
{
	bool pass = true;
	int32 X = position.x * _Width;
	int32 Y = position.y * _Height;

	// Scissor test
	if (States.ScissorState.Enable)
	{
		pass = !(X < States.ScissorState.Rectangle.X ||
			X > States.ScissorState.Rectangle.X + States.ScissorState.Rectangle.Width ||
			Y < States.ScissorState.Rectangle.Y ||
			Y > States.ScissorState.Rectangle.Y + States.ScissorState.Rectangle.Height);
	}

	return pass;
}
