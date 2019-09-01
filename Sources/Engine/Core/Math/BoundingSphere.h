/*=============================================================================
BoundingSphere.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_BOUNDINGSPHERE_H_
#define _SE_BOUNDINGSPHERE_H_

#include "Core/Common.h"
#include "Core/Math/Math.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Plane.h"

namespace SonataEngine
{

/**
	@brief BoundingSphere.

	A sphere is defined by a center and a radius.
*/
class SE_CORE_EXPORT BoundingSphere
{
public:
	static const BoundingSphere Empty;

public:
	/** Center of the sphere. */
	Vector3 Center;

	/** Radius of the sphere. */
	real Radius;

public:
	/** @name Constructors. */
	//@{
	/** Default constructor. */
	BoundingSphere();

	/**
		Initialize a new sphere from a center and a radius.
		@param center The center of the sphere.
		@param radius The radius of the sphere.
	*/
	BoundingSphere(const Vector3& center, real radius);

	/** Copy constructor. */
	BoundingSphere(const BoundingSphere& value);
	//@}

	/** @name Binary Operators. */
	//@{
	/** Compares this instance to another instance to determine whether they are the same. */
	bool operator==(const BoundingSphere& value) const;

	/** Compares this instance to another instance to determine whether they are different. */
	bool operator!=(const BoundingSphere& value) const;
	//@}

	/** @name Properties. */
	//@{
	/**
		Returns whether the sphere is valid.
		@return true if the sphere is valid; otherwise, false.
	*/
	bool IsValid() const;

	/**
		Gets the area of the sphere.
		@return Area of the sphere.
		@remark area = 4 Pi Radius^2
	*/
	real GetArea() const;

	/**
		Gets the volume of the sphere.
		@return Volume of the sphere.
		@remark volume = 4/3 Pi Radius^3
	*/
	real GetVolume() const;
	//@}

	/** @name Operations. */
	//@{
	/**
		Gets the distance to the specified point.
		@param value The point.
		@return The distance to the point.
	*/
	real GetDistance(const Vector3& value) const;

	/** @name Creation. */
	//@{
	/** Creates a BoundingSphere that contains the two specified BoundingSphere instances. */
	static BoundingSphere CreateMerged(const BoundingSphere& original, const BoundingSphere& additional);

	/** Creates a BoundingSphere that can contain a specified list of points. */
	static BoundingSphere CreateFromPoints(Vector3* points, int count);

	/** Creates the smallest BoundingSphere that can contain a specified BoundingBox. */
	static BoundingSphere CreateFromBox(const BoundingBox& box);

	/** Creates the smallest BoundingSphere that can contain a specified Frustum. */
	static BoundingSphere CreateFromFrustum(const Frustum& frustum);
	//@}

	/** @name Operations. */
	//@{
	/**
		Transforms the sphere by the specified matrix.
		@param value The transform matrix.
	*/
	void Transform(const Matrix4& transform);

	/**
		Transforms the specified sphere by the specified matrix.
		@param sphere The sphere.
		@param matrix The transform matrix.
		@return The result of the transformation.
	*/
	static BoundingSphere Transform(const BoundingSphere& sphere, const Matrix4& transform);
	//@}

	/** @name Containment. */
	//@{
	/** Checks whether the current BoundingSphere contains a point. */
	bool Contains(const Vector3& point) const;

	/** Checks whether the current BoundingSphere contains a BoundingSphere. */
	bool Contains(const BoundingSphere& sphere) const;

	/** Checks whether the current BoundingSphere contains a BoundingBox. */
	bool Contains(const BoundingBox& box) const;
	//@}

	/** @name Intersection. */
	//@{
	/** Checks whether the current BoundingSphere intersects a BoundingSphere. */
	bool Intersects(const BoundingSphere& sphere) const;

	/** Checks whether the current BoundingSphere intersects a BoundingBox. */
	bool Intersects(const BoundingBox& box) const;

	/** Checks whether the current BoundingSphere intersects a Ray. */
	bool Intersects(const Ray3& ray, real& result) const;

	/** Checks whether the current BoundingSphere intersects a Plane. */
	PlaneIntersectionType Intersects(const Plane& plane) const;
	//@}
};

#include "BoundingSphere.inl"

}

#endif 
