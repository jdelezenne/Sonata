/*=============================================================================
Common.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_CORE_COMMON_H_
#define _SE_CORE_COMMON_H_

#include "Core/Base.h"
#include "Core/SmartPtr.h"

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#	ifdef SE_STATIC
#		define SE_CORE_EXPORT
#	else
#		ifdef SE_ENGINECORE
#			define SE_CORE_EXPORT __declspec(dllexport)
#		else
#			define SE_CORE_EXPORT __declspec(dllimport)
#		endif 
#	endif 
#else
#	define SE_CORE_EXPORT
#endif

namespace SonataEngine
{

}

#endif
