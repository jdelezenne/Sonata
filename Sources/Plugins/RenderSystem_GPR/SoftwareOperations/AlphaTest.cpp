/*=============================================================================
AlphaTest.cpp
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#include "Software/SoftwareRenderer.h"

bool SoftwareRenderer::AlphaTest(OGPRFragmentFuncEnv& env, real32 alpha)
{
	bool pass = true;
	real32 reference = States.AlphaState.Reference / 255.0f;

	// Alpha comparison
	if (States.AlphaState.TestEnable)
	{
		switch (States.AlphaState.Function)
		{
		case CF_Never:
			pass = false;
			break;
		case CF_Less:
			pass = (alpha < reference);
			break;
		case CF_Equal:
			pass = (fabsf(alpha - reference) < EPSILON);
			break;
		case CF_LessEqual:
			pass = (alpha <= reference);
			break;
		case CF_Greater:
			pass = (alpha > reference);
			break;
		case CF_NotEqual:
			pass = (fabsf(alpha - reference) >= EPSILON);
			break;
		case CF_GreaterEqual:
			pass = (alpha >= reference);
			break;
		case CF_Always:
			pass = true;
			break;
		}
	}

	return pass;
}
