/*=============================================================================
Common.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AI_COMMON_H_
#define _SE_AI_COMMON_H_

#include <Core/Core.h>

#define SE_ID_AI SonataEngine::SE_ID(0xbfb912ee,0x5d1743b3)

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#	ifdef SE_STATIC
#		define SE_AI_EXPORT
#	else
#		ifdef SE_ENGINEAI
#			define SE_AI_EXPORT __declspec(dllexport)
#		else
#			define SE_AI_EXPORT __declspec(dllimport)
#		endif 
#	endif 
#else
#	define SE_AI_EXPORT
#endif

namespace SonataEngine
{
	namespace AI
	{
		class PathNode
		{
		public:
			virtual const Array<PathNode*>& GetNeighbours() = 0;
			Vector2 Position;

		protected:
			Array<PathNode*> _Neighbours; // cache
		};
	}
}

#endif
