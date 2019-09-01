/*=============================================================================
Box.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_BOX_H_
#define _SE_BOX_H_

#include "Core/Common.h"
#include "Core/Math/Math.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Matrix4.h"
#include "Core/Math/AABB.h"

namespace SonataEngine
{

/**
	@brief Oriented Bounding Box (OBB).

	An OBB is defined by a center position, a size vector and a rotation matrix.
*/
class SE_CORE_EXPORT OBB
{
public:
	static const OBB Empty;

public:
	/** Center of the OBB. */
	Vector3 Center;

	/** Extents of the OBB. */
	Vector3 Extents;

	/** Orientation of the OBB. */
	Matrix3 Rotation;

public:
	/** @name Constructors. */
	//@{
	/** Default constructor. */
	OBB();

	/** Constructor. */
	OBB(const Vector3& center, const Vector3& extents, const Matrix3& rotation);

	/** Constructor. */
	OBB(const AABB& aabb, const Matrix3& rotation);

	/** Copy constructor. */
	OBB(const OBB& value);
	//@}

	/** @name Properties. */
	//@{
	/**
		Returns whether the OBB is valid.
		@return true if the OBB is valid; otherwise, false.
	*/
	bool IsValid() const;

	/** Gets the center of the OBB. */
	const Vector3 GetCenter() const;

	/** Gets the minimum corner of the OBB. */
	const Vector3 GetMinimum() const;

	/** Gets the maximum corner of the OBB. */
	const Vector3 GetMaximum() const;

	/** Gets the extents (half of width, height, depth) of the OBB. */
	const Vector3 GetExtents() const;

	/** Gets the rotation of the OBB. */
	const Matrix3 GetRotation() const;

	/**
		Gets the area of the OBB.
		@return Area of the OBB.
		@remark area = 2 (sx * sy + sy * sz + sz * sx)
	*/
	real GetArea() const;

	/**
		Gets the volume of the OBB.
		@return Volume of the OBB.
		@remark volume = sx * sy * sz
	*/
	real GetVolume() const;

	/** Computes the 8 corners of the OBB. */
	void GetCorners(Vector3* corners) const;
	//@}

	/** @name Operations. */
	//@{
	/** Expands the OBB to contain the given vector. */
	void ExpandTo(const Vector3& value);

	/** Transforms the OBB by the given matrix. */
	void Transform(const Matrix4& value);

	/** Returns wether the OBB intersects with the given vector. */
	bool Intersects(const Vector3& value) const;
	//@}
};

#include "OBB.inl"

}

#endif 
