/*=============================================================================
Range.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_RANGE_H_
#define _SE_RANGE_H_

#include "Core/Common.h"
#include "Core/Color32.h"
#include "Core/Math/Math.h"

namespace SonataEngine
{

/**
	Value Range.
	Represents a range of values with a Min/Max pair.
*/
template <class T>
class Range
{
public:
	Range();

	Range(const T& min, const T& max);

	void Get(T& min, T& max) const;

	void Set(const T& min, const T& max);

	bool operator==(const Range& value) const;

	bool operator!=(const Range& value) const;

public:
	T Min;
	T Max;
};

typedef Range<int32> RangeInt32;
typedef Range<real32> RangeReal32;
typedef Range<real64> RangeReal64;
typedef Range<Color32> RangeColor32;
typedef Range<Vector2> RangeVector2;
typedef Range<Vector3> RangeVector3;

#include "Range.inl"

}

#endif 
