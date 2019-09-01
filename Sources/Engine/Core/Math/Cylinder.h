/*=============================================================================
Cylinder.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_CYLINDER_H_
#define _SE_CYLINDER_H_

#include "Core/Common.h"
#include "Core/Math/Math.h"
#include "Core/Math/Segment.h"

namespace SonataEngine
{

/**
	@brief Cylinder.

	A cylinder is defined by a center and a radius.
*/
class SE_CORE_EXPORT Cylinder
{
public:
	/** @name Constructors. */
	//@{
	/** Default constructor. */
	Cylinder();

	/**
		Initialize a new cylinder from a segment and a radius.
		@param segment The segment of the cylinder.
		@param radius The radius of the cylinder.
	*/
	Cylinder(const Segment& segment, real radius);

	/**
		Initialize a new cylinder from two points and a radius.
		@param p0 The first point.
		@param p1 The second point.
		@param radius The radius of the cylinder.
	*/
	Cylinder(const Vector3& p0, const Vector3& p1, real radius);

	/**
		Initialize a new cylinder from a center, a height and a radius.
		@param center The center of the cylinder.
		@param axis The axis of the cylinder.
		@param height The height of the cylinder.
		@param radius The radius of the cylinder.
	*/
	Cylinder(const Vector3& center, const Vector3& axis, real height, real radius);

	/** Copy constructor. */
	Cylinder(const Cylinder& value);
	//@}

	/** @name Binary Operators. */
	//@{
	/** Compares this instance to another instance to determine whether they are the same. */
	bool operator==(const Cylinder& value) const;

	/** Compares this instance to another instance to determine whether they are different. */
	bool operator!=(const Cylinder& value) const;
	//@}

	/** @name Properties. */
	//@{
	/**
		Gets the center of the cylinder.
		@return The center of the cylinder.
	*/
	Vector3 GetCenter() const;

	/**
		Gets the height of the cylinder.
		@return The height of the cylinder.
	*/
	real GetHeight() const;

	/**
		Gets the first point of the cylinder.
		@return The first point of the cylinder.
	*/
	const Vector3& GetPoint0() const;

	/**
		Sets the first point of the cylinder.
		@param value The first point of the cylinder.
	*/
	void SetPoint0(const Vector3& value);

	/**
		Gets the second point of the cylinder.
		@return The second point of the cylinder.
	*/
	const Vector3& GetPoint1() const;

	/**
		Sets the second point of the cylinder.
		@param value The second point of the cylinder.
	*/
	void SetPoint1(const Vector3& value);

	/**
		Gets the segment of the cylinder.
		@return The segment of the cylinder.
	*/
	const Segment& GetSegment() const;

	/**
		Sets the segment of the cylinder.
		@param value The segment of the cylinder.
	*/
	void SetSegment(const Segment& value);

	/**
		Gets the radius of the cylinder.
		@return The radius of the cylinder.
	*/
	real GetRadius() const;

	/**
		Sets the radius of the cylinder.
		@param value The radius of the cylinder.
	*/
	void SetRadius(real value);

	/**
		Gets the area of the cylinder.
		@return The area of the cylinder.
		@remark area = Pi Radius Height
	*/
	real GetArea() const;

	/**
		Gets the volume of the cylinder.
		@return The volume of the cylinder.
		@remark volume = 2 Pi Radius Height + 2 Pi Radius^2
	*/
	real GetVolume() const;
	//@}

	/** @name Operations. */
	//@{
	/**
		Returns whteher the cylinder contains the specified point.
		@param value The point.
		@return true if the cylinder contains the specified point; otherwise, false.
	*/
	bool ContainsPoint(const Vector3& value) const;

	/**
		Transforms the cylinder by the specified matrix.
		@param transform The matrix.
	*/
	void Transform(const Matrix4& transform);

	/**
		Transforms the specified cylinder by the specified matrix.
		@param cylinder The cylinder.
		@param transform The matrix.
		@return The result of the transformation.
	*/
	static Cylinder Transform(const Cylinder& cylinder, const Matrix4& transform);
	//@}

protected:
	Segment _Segment;
	real _Radius;
};

#include "Cylinder.inl"

}

#endif 
