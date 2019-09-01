/*=============================================================================
Ray2.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_RAY2_H_
#define _SE_RAY2_H_

#include "Core/Common.h"
#include "Core/Math/Math.h"
#include "Core/Math/Vector2.h"

namespace SonataEngine
{

/** Ray 2D. */
class SE_CORE_EXPORT Ray2
{
public:
	Vector2 Origin;
	Vector2 Direction;

public:
	/** @name Constructors. */
	//@{
	/** Default constructor. */
	Ray2();

	/**
		Initialize a new ray from an origin and a direction.
		@param origin The origin.
		@param direction The direction.
	*/
	Ray2(const Vector2& origin, const Vector2& direction);

	/** Copy constructor. */
	Ray2(const Ray2& value);
	//@}

	/** @name Binary Operators. */
	//@{
	/** Compares this instance to another instance to determine whether they are the same. */
	bool operator==(const Ray2& value) const;

	/** Compares this instance to another instance to determine whether they are different. */
	bool operator!=(const Ray2& value) const;
	//@}

	/** @name Operations. */
	//@{
	/**
		Computes a point on the ray.
		@param t Parameter [t=0 => p = Origin, t=1 => p = Origin + Direction * t].
		@return The point on the ray.
	*/
	Vector2 ComputePoint(real t) const;

	/**
		Gets the distance to the specified point.
		@param value The point.
		@param t [out] The parameter.
		@return The distance to the point.
	*/
	real GetDistance(const Vector2& value, real* t = NULL) const;

	/**
		Gets the distance to the specified point.
		@param value The point.
		@param t [out] The parameter.
		@return The distance to the point.
	*/
	real GetSquareDistance(const Vector2& value, real* t = NULL) const;
	//@}
};

#include "Ray2.inl"

}

#endif 
