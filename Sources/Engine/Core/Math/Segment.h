/*=============================================================================
Segment.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SEGMENT_H_
#define _SE_SEGMENT_H_

#include "Core/Common.h"
#include "Core/Math/Math.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Matrix4.h"

namespace SonataEngine
{

/**
	@brief Line segment.

	A segment is defined by two points.
*/
class SE_CORE_EXPORT Segment
{
public:
	/** @name Constructors. */
	//@{
	/** Default constructor. */
	Segment();

	/**
		Initialize a new segment from two points.
		@param p0 The start point.
		@param p1 The end point.
	*/
	Segment(const Vector3& p0, const Vector3& p1);

	/** Copy constructor. */
	Segment(const Segment& value);
	//@}

	/** @name Binary Operators. */
	//@{
	/** Compares this instance to another instance to determine whether they are the same. */
	bool operator==(const Segment& value) const;

	/** Compares this instance to another instance to determine whether they are different. */
	bool operator!=(const Segment& value) const;
	//@}

	/** @name Properties. */
	//@{
	/**
		Gets the origin of the segment.
		@return The origin of the segment.
	*/
	Vector3 GetOrigin() const;

	/**
		Gets the direction of the segment.
		@return The direction of the segment.
	*/
	Vector3 GetDirection() const;

	/**
		Gets the origin and the direction of the segment.
		@param origin [out] The origin of the segment.
		@param direction [out] The direction of the segment.
	*/
	void GetOriginDirection(Vector3& origin, Vector3& direction) const;

	/**
		Sets the origin and the direction of the segment.
		@param origin The origin of the segment.
		@param direction The direction of the segment.
	*/
	void SetOriginDirection(const Vector3& origin, const Vector3& direction);

	/**
		Gets the length of the segment.
		@return The length of the segment.
	*/
	real GetLength() const;

	/**
		Gets the square length of the segment.
		@return The square length of the segment.
	*/
	real GetLengthSquared() const;
	//@}

	/** @name Operations. */
	//@{
	/**
		Computes a point on the segment.
		@param t Parameter [t=0 => p = p0, t=1 => p = p1].
		@return The point on the segment.
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
		Gets the distance to the specified point.
		@param value The point.
		@param t [out] The parameter.
		@return The distance to the point.
		@todo
	*/
	real GetSquareDistance(const Vector3& value, real* t = NULL) const;

	/**
		Transforms the segment by the specified matrix.
		@param transform The matrix.
	*/
	void Transform(const Matrix4& transform);

	/**
		Transforms the specified segment by the specified matrix.
		@param segment The segment.
		@param transform The matrix.
		@return The result of the transformation.
	*/
	static Segment Transform(const Segment& segment, const Matrix4& transform);
	//@}

public:
	/** Start point. */
	Vector3 p0;

	/** End point. */
	Vector3 p1;
};

#include "Segment.inl"

}

#endif 
