/*=============================================================================
Common.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SCRIPT_COMMON_H_
#define _SE_SCRIPT_COMMON_H_

#define SE_ID_SCRIPTSYSTEM SonataEngine::SE_ID(0xe33ba446,0xd1ef42eb)

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#	ifdef SE_STATIC
#		define SE_SCRIPT_EXPORT
#	else
#		ifdef SE_ENGINE
#			define SE_SCRIPT_EXPORT __declspec(dllexport)
#		else
#			define SE_SCRIPT_EXPORT __declspec(dllimport)
#		endif 
#	endif 
#else
#	define SE_SCRIPT_EXPORT
#endif

namespace SonataEngine
{
	namespace Script
	{
	}
}

#endif
