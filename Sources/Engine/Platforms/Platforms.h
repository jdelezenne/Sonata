/*=============================================================================
Platforms.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_PLATFORMS_H_
#define _SE_PLATFORMS_H_

#include <float.h>

namespace SonataEngine
{

/// Disable annoying warnings.

#if defined(_MSC_VER)
	// conversion from 'type1' to 'type2', possible loss of data
#	pragma warning(disable : 4244)
	// 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
#	pragma warning (disable : 4251)
	// truncation from 'type1' to 'type2'
#	pragma warning(disable : 4305)
	// 'function': was declared deprecated
#	pragma warning(disable : 4996)

	// 'type cast' : pointer truncation from 'PVOID' to 'LONG'
#	pragma warning(disable : 4311)
	// 'type cast' : conversion from 'LONG' to 'PVOID' of greater size
#	pragma warning(disable : 4312)
#endif

/// Platform specific types.

#ifdef WIN32
	typedef signed char         int8;
	typedef unsigned char       uint8, uchar;
	typedef signed short        int16;
	typedef unsigned short      uint16, ushort;
	typedef signed int          int32;
	typedef unsigned int        uint32, uint;
	typedef __int64             int64;
	typedef unsigned __int64    uint64, ulong;
	typedef float               real32;
	typedef double              real64;

#elif LINUX
	typedef signed char         int8;
	typedef unsigned char       uint8, uchar;
	typedef signed short        int16;
	typedef unsigned short      uint16, ushort;
	typedef signed int          int32;
	typedef unsigned int        uint32, uint;
	typedef long long           int64;
	typedef unsigned long long  uint64, ulong;
	typedef float               real32;
	typedef double              real64;

#elif __APPLE__
	typedef signed char         int8;
	typedef unsigned char       uint8, uchar;
	typedef signed short        int16;
	typedef unsigned short      uint16, ushort;
	typedef signed int          int32;
	typedef unsigned int        uint32, uint;
	typedef long long           int64;
	typedef unsigned long long  uint64, ulong;
	typedef float               real32;
	typedef double              real64;

#elif XBOX
	typedef signed char         int8;
	typedef unsigned char       uint8, uchar;
	typedef signed short        int16;
	typedef unsigned short      uint16, ushort;
	typedef signed int          int32;
	typedef unsigned int        uint32, uint;
	typedef __int64             int64;
	typedef unsigned __int64    uint64, ulong;
	typedef float               real32;
	typedef double              real64;

#elif PS2
	typedef signed char         int8;
	typedef unsigned char       uint8, uchar;
	typedef signed short        int16;
	typedef unsigned short      uint16, ushort;
	typedef signed int          int32;
	typedef unsigned int        uint32, uint;
	typedef long                int64;
	typedef unsigned long       uint64, ulong;
	typedef float               real32;
	typedef double              real64;

#elif PSP
	typedef signed char         int8;
	typedef unsigned char       uint8, uchar;
	typedef signed short        int16;
	typedef unsigned short      uint16, ushort;
	typedef signed int          int32;
	typedef unsigned int        uint32, uint;
	typedef long                int64;
	typedef unsigned long       uint64, ulong;
	typedef float               real32;
	typedef double              real64;

#elif XENON
	typedef signed char         int8;
	typedef unsigned char       uint8, uchar;
	typedef signed short        int16;
	typedef unsigned short      uint16, ushort;
	typedef signed int          int32;
	typedef unsigned int        uint32, uint;
	typedef __int64             int64;
	typedef unsigned __int64    uint64, ulong;
	typedef float               real32;
	typedef double              real64;

#elif __CELLOS_LV2__
	typedef signed char         int8;
	typedef unsigned char       uint8, uchar;
	typedef signed short        int16;
	typedef unsigned short      uint16, ushort;
	typedef signed int          int32;
	typedef unsigned int        uint32, uint;
	typedef long long           int64;
	typedef unsigned long long  uint64, ulong;
	typedef float               real32;
	typedef double              real64;

#else
#	error Unknown platform!
#endif

/// Check type sizes.
SE_COMPILE_TIME_ASSERT(sizeof(int8)==1);
SE_COMPILE_TIME_ASSERT(sizeof(uint8)==1);
SE_COMPILE_TIME_ASSERT(sizeof(int16)==2);
SE_COMPILE_TIME_ASSERT(sizeof(uint16)==2);
SE_COMPILE_TIME_ASSERT(sizeof(int32)==4);
SE_COMPILE_TIME_ASSERT(sizeof(uint32)==4);
SE_COMPILE_TIME_ASSERT(sizeof(int64)==8);
SE_COMPILE_TIME_ASSERT(sizeof(uint64)==8);

/// Type ranges.
#define SE_MAX_I8  0x7f                // Max possible sbyte value
#define SE_MIN_I8  0x80                // Min possible sbyte value
#define SE_MAX_U8  0xff                // Max possible ubyte value
#define SE_MIN_U8  0x00                // Min possible ubyte value
#define SE_MAX_I16 0x7fff              // Max possible sword value
#define SE_MIN_I16 0x8000              // Min possible sword value
#define SE_MAX_U16 0xffff              // Max possible uword value
#define SE_MIN_U16 0x0000              // Min possible uword value
#define SE_MAX_I32 0x7fffffff          // Max possible sdword value
#define SE_MIN_I32 0x80000000          // Min possible sdword value
#define SE_MAX_U32 0xffffffff          // Max possible udword value
#define SE_MIN_U32 0x00000000          // Min possible udword value
#define SE_MAX_I64 0x7fffffffffffffff  // Max possible sdword64 value
#define SE_MIN_I64 0x8000000000000000  // Min possible sdword64 value
#define SE_MAX_U64 0xffffffffffffffff  // Max possible udword64 value
#define SE_MIN_U64 0x0000000000000000  // Min possible udword64 value
#define SE_MAX_R32 FLT_MAX             // Max possible float value
#define SE_MIN_R32 (-FLT_MAX)          // Min possible float value
#define SE_MAX_R64 DBL_MAX             // Max possible double value
#define SE_MIN_R64 (-DBL_MAX)          // Min possible double value

#define SE_EPS_R32 FLT_EPSILON         // Smallest number not zero
#define SE_EPS_R64 DBL_EPSILON         // Smallest number not zero

#define SE_IEEE_1_0     0x3f800000     // Integer representation of 1.0
#define SE_IEEE_255_    0 0x437f0000   // Integer representation of 255.0
#define SE_IEEE_MAX_R32 0x7f7fffff
#define SE_IEEE_MIN_R32 0xff7fffff

/// Byte order.
enum ByteOrder
{
	ByteOrder_BigEndian,   /// Big Endian
	ByteOrder_LittleEndian /// Little Endian
};

#if SE_USE_BIGENDIAN
#	define SE_ENDIAN ByteOrder_BigEndian
#else
#	define SE_ENDIAN ByteOrder_LittleEndian
#endif 

/// Real type
#if SE_USE_REAL64
	typedef real64 real;
#else
	typedef real32 real;
#endif

/// Pointer type
//#if SE_USE_CPU64
//	typedef uint64 SEptr;
//#else
//	typedef uint32 SEptr;
//#endif
typedef size_t SEptr;

typedef uint8 SEbyte;
typedef ushort SEunicode;
typedef real64 SEtime;

/// unicode strings
#if SE_USE_UNICODE
	typedef SEunicode SEchar;
#else
	typedef char SEchar;
#endif

#ifdef NULL
#undef NULL
#endif
#define NULL 0L

#ifdef _T
#undef _T
#endif

#if SE_USE_UNICODE
#	define _T(x) L##x
#else
#	define _T(x) x
#endif

/// Packing alignment
/* With Microsoft Visual Studio, the byte aligned structures must be declared as follow:
#ifdef _MSC_VER
	#pragma pack(push, packing)
	#pragma pack(1)
#endif

SE_ALIGN_BEGIN
struct structname
{
} SE_ALIGN_END;

#ifdef _MSC_VER
	#pragma pack(pop, packing)
#endif
*/

#if defined( __GNUC__ )
#	define SE_ALIGN_BEGIN
#	define SE_ALIGN_END __attribute__((packed))
#else
#	define SE_ALIGN_BEGIN
#	define SE_ALIGN_END
#endif

}

#endif 
