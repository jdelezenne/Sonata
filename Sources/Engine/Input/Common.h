/*=============================================================================
Common.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_INPUT_COMMON_H_
#define _SE_INPUT_COMMON_H_

#include <Core/Core.h>

#define SE_ID_INPUTSYSTEM SonataEngine::SE_ID(0x852028,0x5c3d2635)

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#	ifdef SE_STATIC
#		define SE_INPUT_EXPORT
#	else
#		ifdef SE_ENGINEINPUT
#			define SE_INPUT_EXPORT __declspec(dllexport)
#		else
#			define SE_INPUT_EXPORT __declspec(dllimport)
#		endif 
#	endif 
#else
#	define SE_INPUT_EXPORT
#endif

#endif
