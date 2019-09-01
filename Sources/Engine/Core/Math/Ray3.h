/*=============================================================================
Ray3.h
Project: Sonata Engine
Copyright (c) 2005
Julien Delezenne
=============================================================================*/

#ifndef _SE_RAY3_H_
#define _SE_RAY3_H_

#include "Core/Common.h"
#include "Core/Math/Math.h"
#include "Core/Math/Vector3.h"

namespace SonataEngine
{

class BoundingBox;
class BoundingSphere;
class Frustum;
class Plane;

/** Ray 3D. */
class SE_CORE_EXPORT Ray3
{
public:
	Vector3 Origin;
	Vector3 Direction;

public:
	/** @name Constructors. */
	//@{
	/** Default constructor. */
	Ray3();

	/**
		Initialize a new ray from an origin and a direction.
		@param origin The origin.
		@param direction The direction.
	*/
	Ray3(const Vector3& origin, const Vector3& direction);

	/** Copy constructor. */
	Ray3(const Ray3& value);
	//@}

	/** @name Binary Operators. */
	//@{
	/** Compares this instance to another instance to determine whether they are the same. */
	bool operator==(const Ray3& value) const;

	/** Compares this instance to another instance to determine whether they are different. */
	bool operator!=(const Ray3& value) const;
	//@}

	/** @name Operations. */
	//@{
	/**
		Computes a point on the ray.
		@param t Parameter [t=0 => p = Origin, t=1 => p = Origin + Direction * t].
		@return The point on the ray.
	*/
	Vector3 ComputePoint(real t) const;

	/**
		Gets the distance to the specified point.
		@param value The point.
		@param t [out] The parameter.
		@return The distance to the point.
	*/
	real GetDistance(const Vector3& value, real* t = NULL) const;

	/**
		Gets the square of the distance to the specified point.
		@param value The point.
		@param t [out] The parameter.
		@return The distance to the point.
	*/
	real GetDistanceSquared(const Vector3& value, real* t = NULL) const;
	//@}

	/** @name Intersection. */
	//@{
    bool Intersects(const BoundingSphere& sphere, real& result) const;

    bool Intersects(const BoundingBox& box, real& result) const;

    bool Intersects(const Frustum& frustum, real& result) const;

    bool Intersects(const Plane& plane, real& result) const;
	//@}
};

#include "Ray3.inl"

}

#endif 
