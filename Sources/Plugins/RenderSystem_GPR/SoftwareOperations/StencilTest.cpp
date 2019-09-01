/*=============================================================================
StencilTest.cpp
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#include "Software/SoftwareRenderer.h"

bool SoftwareRenderer::StencilTest(OGPRFragmentFuncEnv& env)
{
	bool pass = true;
	int32 current;
	int32 left, right;

	// Stencil comparison
	if (States.StencilState.Enable)
	{
		env.ReadPixel(DBT_StencilBuffer, &current);

		left = (States.StencilState.Reference & States.StencilState.Mask);
		right = (current & States.StencilState.Mask);

		switch (States.StencilState.Function)
		{
		case CF_Never:
			pass = false;
			break;
		case CF_Less:
			pass = (left < right);
			break;
		case CF_Equal:
			pass = (left == right);
			break;
		case CF_LessEqual:
			pass = (left <= right);
			break;
		case CF_Greater:
			pass = (left > right);
			break;
		case CF_NotEqual:
			pass = (left != right);
			break;
		case CF_GreaterEqual:
			pass = (left >= right);
			break;
		case CF_Always:
			pass = true;
			break;
		}

		if (!pass)
		{
			_StencilOperation(env, States.StencilState.Fail, current);
		}
	}

	return pass;
}

void SoftwareRenderer::_StencilOperation(OGPRFragmentFuncEnv& env, StencilOperation operation, int32 current)
{
	int32 value;
	switch (operation)
	{
	case SO_Keep:
		value = current;
		break;
	case SO_Zero:
		value = 0;
		break;
	case SO_Replace:
		value = States.StencilState.Reference;
		break;
	case SO_IncrementSaturation:
		if (current < ~(int32)0)
			value = current + 1;
		break;
	case SO_DecrementSaturation:
		if (current > 0)
			value = current - 1;
		break;
	case SO_Invert:
		value = ~current;
		break;
	case SO_Increment:
		if (current < ~(int32)0)
			value = current + 1;
		else
			value = 0;
		break;
	case SO_Decrement:
		if (current > 0)
			value = current - 1;
		else
			value = ~(int32)0;
		break;
	}

	int32 stencil = (current & ~States.StencilState.WriteMask) |
		(States.StencilState.WriteMask & value);
	env.WritePixel(DBT_StencilBuffer, &stencil);
}
