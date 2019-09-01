/*=============================================================================
Common.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _RAYTRACER_COMMON_H_
#define _RAYTRACER_COMMON_H_

#include <Core/Core.h>
#include <Core/Engine.h>
#include <Core/System/Application.h>

#ifdef WIN32
#	include <Platforms/Win32/Win32DebugLogHandler.h>
#endif

namespace Raytracer
{
	using namespace SonataEngine;

	typedef SonataEngine::byte kmByte;

	class RTScene;
	class RTSceneObject;

	enum RayType
	{
		RayType_Eye, // eye ray
		RayType_Transparent, // transparency ray
		RayType_Reflect, // reflection ray
		RayType_Refract, // refraction ray
		RayType_Shadow, // shadow ray
		RayType_None // other ray
	};

	struct RenderOptions
	{
		RenderOptions();

		int32 _ReflectionDepth;
		int32 _RefractionDepth;
		int32 _TraceDepth;
		bool _Shadows;
	};

	struct RenderState
	{
		RenderState();

		RenderOptions* _Options;
		RTScene* _Scene;
		Ray3 _Ray;
		RayType _RayType;
		int32 _ReflectionDepth;
		int32 _RefractionDepth;
		int32 _TraceDepth;
		real32 _Time;
		int8 _Face;
		real32 _IORIn;
		real32 _IOROut;

		Vector3 _Point;
		Vector3 _Normal;
		Vector2 _TexCoord;
		RTSceneObject* _Object;
	};
}

#endif 
