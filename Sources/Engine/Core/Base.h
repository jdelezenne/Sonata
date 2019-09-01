/*=============================================================================
Base.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_BASE_H_
#define _SE_BASE_H_

#include "Config.h"

/** Macros. */

#if defined(SE_USE_NAMESPACE)
#	define ENGINE_NAMESPACE_BEGIN     namespace SonataEngine {
#	define ENGINE_NAMESPACE_END       }
#	define ENGINE_NAMESPACE_USE       using namespace SonataEngine;
#	define ENGINE_NAMESPACE_QUALIFIER SonataEngine::
#else
#	define ENGINE_NAMESPACE_BEGIN
#	define ENGINE_NAMESPACE_END
#	define ENGINE_NAMESPACE_USE
#	define ENGINE_NAMESPACE_QUALIFIER
#endif

#define SE_INTERFACE            class;

#define SE_STRINGER(x)          #x
#define SE_PASTER(n, x)         n##x

/// Compile-time assert
#define SE_COMPILE_TIME_ASSERT(exp) extern char SE_CompileTimeAssert[(exp) ? 1 : -1]

/// Safe destructions
#define SE_DELETE(x)       if (x) { delete x; x = NULL; }
#define SE_DELETE_ARRAY(x) if (x) { delete[] x; x = NULL; }
#define SE_RELEASE(x)      if (x) { x->Release(); x = NULL; }

/// Inline
#if (_MSC_VER >= 1000)
#	define SE_INLINE __forceinline
#elif defined(__MWERKS__) 
#	define SE_INLINE inline
#else
#	define SE_INLINE inline
#endif

/// Offset
#ifdef  _WIN64
#	define SE_OffsetOf(s, m)   (SEptr)((ptrdiff_t)&(((s *)0)->m))
#else
#	define SE_OffsetOf(s, m)   (SEptr)&(((s *)0)->m)
#endif

/// Swap
template <class T>
SE_INLINE void SE_Swap(T& a, T& b)
{
	const T c = a;
	a = b;
	b = c;
}

/// Number of elements in an array.
#define SE_ARRAY_COUNT(array) \
	(sizeof(array) / sizeof((array)[0]))

/// Offset of a struct member.
#define SE_STRUCT_OFFSET(structure, member) \
	((int32)&((structure*)NULL)->member)


/// foreach keyword
#define foreach(item, container, containerType) \
	item = NULL; \
	containerType##::Iterator it##item = container.GetIterator(); \
	while (it##item.Next() && ((item = it##item.Current()) || true))

/** Types */
#include "Platforms/Platforms.h"
#include "Core/Debug/Debug.h"

#define boolean(b)              uint b : 1;
#define SE_ERROR                uint

#endif
