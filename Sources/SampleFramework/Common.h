/*=============================================================================
Common.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SFW_COMMON_H_
#define _SFW_COMMON_H_

#include <Core/Core.h>
#include <Graphics/Graphics.h>
#include <Input/Input.h>
#include <Audio/Audio.h>
#include <Physics/Physics.h>

using namespace SonataEngine;
using namespace SonataEngine::Input;
using namespace SonataEngine::Audio;
using namespace SonataEngine::Physics;

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#	ifdef SE_STATIC
#		define SE_SAMPLEFRAMEWORK_EXPORT
#	else
#		ifdef SE_SAMPLEFRAMEWORK
#			define SE_SAMPLEFRAMEWORK_EXPORT __declspec(dllexport)
#		else
#			define SE_SAMPLEFRAMEWORK_EXPORT __declspec(dllimport)
#		endif 
#	endif 
#else
#	define SE_SAMPLEFRAMEWORK_EXPORT
#endif

namespace SampleFramework
{
	typedef int32 EntityID;
	const int32 EntityID_Unknown = -1;

	typedef int32 MessageID;
	const int32 MessageID_Unknown = -1;
}

#endif
