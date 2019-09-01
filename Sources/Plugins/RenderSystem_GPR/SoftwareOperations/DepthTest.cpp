/*=============================================================================
DepthTest.cpp
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#include "Software/SoftwareRenderer.h"

bool SoftwareRenderer::DepthTest(OGPRFragmentFuncEnv& env, real32 depth)
{
	bool pass = true;
	real32 current;

	if (States.DepthState.Enable || States.DepthState.WriteEnable)
	{
		// Depth testing enabled
		if (States.DepthState.Enable)
		{
			env.ReadPixel(DBT_DepthBuffer, &current);
		}

		// Depth write enabled
		if (States.DepthState.WriteEnable)
		{
			env.WritePixel(DBT_DepthBuffer, &depth);
		}
	}

	// Depth comparison
	if (States.DepthState.Enable)
	{
		switch (States.DepthState.Function)
		{
		case CF_Never:
			pass = false;
			break;
		case CF_Less:
			pass = (depth < current);
			break;
		case CF_Equal:
			pass = (fabsf(depth - current) < EPSILON);
			break;
		case CF_LessEqual:
			pass = (depth <= current);
			break;
		case CF_Greater:
			pass = (depth > current);
			break;
		case CF_NotEqual:
			pass = (fabsf(depth - current) >= EPSILON);
			break;
		case CF_GreaterEqual:
			pass = (depth >= current);
			break;
		case CF_Always:
			pass = true;
			break;
		}

		if (States.StencilState.Enable)
		{
			int32 stencil;
			env.ReadPixel(DBT_StencilBuffer, &stencil);

			if (pass)
			{
				_StencilOperation(env, States.StencilState.Pass, stencil);
			}
			else
			{
				_StencilOperation(env, States.StencilState.ZBufferFail, stencil);
			}
		}
	}

	return pass;
}
