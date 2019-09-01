/*=============================================================================
Common.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "Common.h"

namespace Raytracer
{
	RenderOptions::RenderOptions()
	{
		_ReflectionDepth = 5;
		_RefractionDepth = 5;
		_TraceDepth = 5;
		_Shadows = true;
	}

	RenderState::RenderState()
	{
		_Options = NULL;
		_Scene = NULL;
		_RayType = RayType_Eye;
		_ReflectionDepth = 0;
		_RefractionDepth = 0;
		_TraceDepth = 0;
		_IORIn = 1.0f;
		_IOROut = 1.0f;
		_Object = NULL;
	}
}
