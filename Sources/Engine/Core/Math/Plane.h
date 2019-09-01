/*=============================================================================
Plane.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_PLANE_H_
#define _SE_PLANE_H_

#include "Core/Common.h"
#include "Core/Math/Math.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Matrix4.h"

namespace SonataEngine
{

class BoundingBox;
class BoundingSphere;
class Frustum;

/** Plane side for classification. */
enum PlaneIntersectionType
{
	/** There is no intersection, and the bounding volume is in the positive half-space of the Plane. */
	PlaneIntersectionType_Front,

	/** There is no intersection, and the bounding volume is in the negative half-space of the Plane. */
	PlaneIntersectionType_Back,

	/** The Plane is intersected. */
	PlaneIntersectionType_Intersecting
};

/**
	@brief Plane.

	A plane is defined by a normal vector and a distance.
*/
class SE_CORE_EXPORT Plane
{
public:
	/** Normal of the plane. */
	Vector3 Normal;

	/** Distance of the plane. */
	real D;

public:
	/** @name Constructors. */
	//@{
	/** Default constructor. */
	Plane();

	/**
		Initialize a new plane from its equation.
		@param normal The normal of the plane.
		@param distance The distance of the plane.
	*/
	Plane(real a, real b, real c, real d);

	/**
		Initialize a new plane from a normal and a distance.
		@param normal The normal of the plane.
		@param d The distance of the plane.
	*/
	Plane(const Vector3& normal, real d);

	/**
		Initialize a new plane from a normal and a distance.
		@param value The normal and the distance of the plane.
	*/
	Plane(const Vector4& value);

	/**
		Initialize a new plane from a normal and a point in the plane.
		@param normal The normal of the plane.
		@param point The point in the plane.
	*/
	Plane(const Vector3& normal, const Vector3& point);

	/**
		Initialize a new plane from three non-colinear points.
		@param p0 p1 p2 The points on the plane.
	*/
	Plane(const Vector3& p0, const Vector3& p1, const Vector3& p2);

	/** Copy constructor. */
	Plane(const Plane& value);
	//@}

	/** @name Binary Operators. */
	//@{
	/** Compares this instance to another instance to determine whether they are the same. */
	bool operator==(const Plane& value) const;

	/** Compares this instance to another instance to determine whether they are different. */
	bool operator!=(const Plane& value) const;
	//@}

	/** @name Operations. */
	//@{
	/**
		Gets the distance to the specified point.
		@param value The point.
		@return The distance to the point.
	*/
	real GetDistance(const Vector3& value) const;

	/**
		Returns whteher the plane contains the specified point.
		@param value The point.
		@return true if the plane contains the specified point; otherwise, false.
	*/
	bool ContainsPoint(const Vector3& value) const;

	/**
		Returns the side of the point.
		@param value The point.
		@return The result of the classification.
	*/
	PlaneIntersectionType ClassifyPoint(const Vector3& value) const;

	/**
		Projects a point into the plane.
		@param value The point.
		@retur The projected point.
	*/
	Vector3 Project(const Vector3& value) const;

	/** Flips the normal of the plane. */
	void Flip();

	/** Normalizes the plane so that the normal is a unit vector. */
	void Normalize();

	/**
		Transforms the plane by the specified matrix.
		@param transform The transform matrix.
	*/
	void Transform(const Matrix4& transform);

	/**
		Transforms the specified plane by the specified matrix.
		@param plane The plane.
		@param transform The transform matrix.
		@return The result of the transformation.
	*/
	static Plane Transform(const Plane& plane, const Matrix4& transform);
	//@}

	/** @name Intersection. */
	//@{
	/** Checks whether the current Plane intersects a specified BoundingSphere. */
	PlaneIntersectionType Intersects(const BoundingSphere& sphere) const;

	/** Checks whether the current Plane intersects a specified BoundingBox. */
	PlaneIntersectionType Intersects(const BoundingBox& box) const;

	/** Checks whether the current Plane intersects a specified Frustum. */
	PlaneIntersectionType Intersects(const Frustum& frustum) const;
	//@}
};

#include "Plane.inl"

}

#endif 
