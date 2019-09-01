/*=============================================================================
EngineLib.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ENGINELIB_H_
#define _SE_ENGINELIB_H_

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#	ifdef SE_STATIC
		// Static library.
#		define SE_ENGINE_EXPORT
#	else
#		ifdef SE_ENGINE
			// DLL library.
#			define SE_ENGINE_EXPORT __declspec(dllexport)
#		else
			// Client of the DLL library.
#			define SE_ENGINE_EXPORT __declspec(dllimport)
#		endif 
#	endif 
#else
#	define SE_ENGINE_EXPORT
#endif

#endif 
