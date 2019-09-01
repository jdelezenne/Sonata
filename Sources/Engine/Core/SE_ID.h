/*=============================================================================
SE_ID.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ID_H_
#define _SE_ID_H_

#include "Core/Common.h"
#include "Core/String.h"
#include "Core/Exception/ArgumentException.h"

namespace SonataEngine
{

class Stream;

/** @Brief Universally Unique Identifier.
	SE_IDs are used to uniquely identify components.
*/
class SE_CORE_EXPORT SE_ID
{
public:
	static const SE_ID Null;

public:
	union
	{
		struct
		{
			uint32 Data1;
			uint32 Data2;
			uint32 Data3;
			uint32 Data4;
		};
		uint32 Data[4];
	};

public:
	SE_ID(uint32 d1 = 0, uint32 d2 = 0, uint32 d3 = 0, uint32 d4 = 0);

	friend bool operator==(const SE_ID& left, const SE_ID& right);
	
	friend bool operator!=(const SE_ID& left, const SE_ID& right);

	friend bool operator<(const SE_ID& left, const SE_ID& right);
	
	friend bool operator<=(const SE_ID& left, const SE_ID& right);

	friend bool operator>(const SE_ID& left, const SE_ID& right);

	friend bool operator>=(const SE_ID& left, const SE_ID& right);

	void Clear();

	bool IsValid();

	String ToString();

	static SE_ID FromString(const String& value);

	friend Stream& operator<<(Stream& stream, SE_ID& id);
	friend Stream& operator>>(Stream& stream, SE_ID& id);
};

#include "SE_ID.inl"

}

#endif
