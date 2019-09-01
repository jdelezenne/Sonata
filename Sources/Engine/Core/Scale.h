/*=============================================================================
Scale.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SCALE_H_
#define _SE_SCALE_H_

#include "Core/Containers/Array.h"
#include "Core/Color32.h"
#include "Core/Math/Math.h"

namespace SonataEngine
{

/**
	Value Scale Key.
	Represents a step in a scale.
*/
template <class T>
class ScaleKey
{
public:
	ScaleKey();

	ScaleKey(real32 relativeTime, T value);

	void Get(real32& relativeTime, T& value) const;

	void Set(real32 relativeTime, T value);

	bool operator==(const ScaleKey<T>& other) const;

	bool operator!=(const ScaleKey<T>& other) const;

public:
	real32 RelativeTime;
	T Value;
};

typedef ScaleKey<int32> ScaleKeyInt32;
typedef ScaleKey<real32> ScaleKeyReal32;
typedef ScaleKey<real64> ScaleKeyReal64;
typedef ScaleKey<Color32> ScaleKeyColor32;
typedef ScaleKey<Vector2> ScaleKeyVector2;
typedef ScaleKey<Vector3> ScaleKeyVector3;

/**
	Value Scale.
	Represents a scale of keys made of a relative time/value pair.
*/
template <class T>
class Scale : public Array< ScaleKey<T> >
{
public:
	Scale();

	bool operator==(const Scale<T>& other) const;

	bool operator!=(const Scale<T>& other) const;
};

typedef Scale<int32> ScaleInt32;
typedef Scale<real32> ScaleReal32;
typedef Scale<real64> ScaleReal64;
typedef Scale<Color32> ScaleColor32;
typedef Scale<Vector2> ScaleVector2;
typedef Scale<Vector3> ScaleVector3;

#include "Scale.inl"

}

#endif 
