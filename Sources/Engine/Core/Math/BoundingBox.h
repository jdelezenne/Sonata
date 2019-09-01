/*=============================================================================
BoundingBox.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_BOUNDINGBOX_H_
#define _SE_BOUNDINGBOX_H_

#include "Core/Common.h"
#include "Core/Math/Math.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Matrix4.h"
#include "Core/Math/Plane.h"

namespace SonataEngine
{

/**
	@brief Axis Aligned Bounding Box (AABB).

	An AABB is a bounding volume defined by a minimum vector and a maximum vector.
*/
class SE_CORE_EXPORT BoundingBox
{
public:
	static const BoundingBox Empty;

public:
	/** Minimum corner of the BoundingBox. */
	Vector3 Min;

	/** Maximum corner of the BoundingBox. */
	Vector3 Max;

public:
	/** @name Constructors. */
	//@{
	/** Default constructor. */
	BoundingBox();

	/** Constructor with extents. */
	BoundingBox(const Vector3& min, const Vector3& max);

	/** Copy constructor. */
	BoundingBox(const BoundingBox& value);
	//@}

	/** @name Properties. */
	//@{
	/** Returns whether the BoundingBox is empty. */
	bool IsEmpty() const;

	/** Gets the center of the BoundingBox. */
	const Vector3 GetCenter() const;

	/** Gets the dimensions (width, height, depth) of the BoundingBox. */
	const Vector3 GetDimensions() const;

	/** Gets the extents (half of width, height, depth) of the BoundingBox. */
	const Vector3 GetExtents() const;

	/** Gets the 8 corners of the BoundingBox. */
	void GetCorners(Vector3* corners) const;
	//@}

	/** @name Creation. */
	//@{
	/** Creates a BoundingBox that contains the two specified BoundingBox instances. */
	static BoundingBox CreateMerged(const BoundingBox& original, const BoundingBox& additional);

	/** Creates a BoundingBox that can contain a specified list of points. */
	static BoundingBox CreateFromPoints(Vector3* points, int count);

	/** Creates the smallest BoundingBox that can contain a specified BoundingSphere. */
	static BoundingBox CreateFromSphere(const BoundingSphere& sphere);
	//@}

	/** @name Operations. */
	//@{
	/** Merges the BoundingBox with another BoundingBox. */
	void Merge(const BoundingBox& value);

	/** Scales the BoundingBox by a given scalar. */
	void Scale(real value);

	/** Expands the BoundingBox to contain the given vector. */
	void ExpandTo(const Vector3& value);

	/**
		Transforms the BoundingBox by the given matrix.
		@param value The transform matrix.
	*/
	void Transform(const Matrix4& transform);

	/**
		Transforms the specified sphere by the specified matrix.
		@param box The box.
		@param matrix The transform matrix.
		@return The result of the transformation.
	*/
	static BoundingBox Transform(const BoundingBox& box, const Matrix4& transform);
	//@}

	/** @name Containment. */
	//@{
	/** Checks whether the current BoundingBox contains a point. */
	bool Contains(const Vector3& point) const;

	/** Checks whether the current BoundingBox contains a BoundingSphere. */
	bool Contains(const BoundingSphere& sphere) const;

	/** Checks whether the current BoundingBox contains a BoundingBox. */
	bool Contains(const BoundingBox& box) const;
	//@}

	/** @name Intersection. */
	//@{
	/** Checks whether the current BoundingBox intersects a BoundingSphere. */
	bool Intersects(const BoundingSphere& sphere) const;

	/** Checks whether the current BoundingBox intersects a BoundingBox. */
	bool Intersects(const BoundingBox& box) const;

	/** Checks whether the current BoundingBox intersects a Ray. */
	bool Intersects(const Ray3& ray, real& result) const;

	/** Checks whether the current BoundingBox intersects a Plane. */
	PlaneIntersectionType Intersects(const Plane& plane) const;
	//@}
};

#include "BoundingBox.inl"

}

#endif 
