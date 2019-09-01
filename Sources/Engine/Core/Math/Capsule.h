/*=============================================================================
Capsule.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_CAPSULE_H_
#define _SE_CAPSULE_H_

#include "Core/Common.h"
#include "Core/Math/Math.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Segment.h"

namespace SonataEngine
{

/**
	@brief Capsule.

	A capsule is defined by a center and a radius.
*/
class SE_CORE_EXPORT Capsule
{
public:
	/** @name Constructors. */
	//@{
	/** Default constructor. */
	Capsule();

	/**
		Initialize a new capsule from a segment and a radius.
		@param segment The segment of the capsule.
		@param radius The radius of the capsule.
	*/
	Capsule(const Segment& segment, real radius);

	/**
		Initialize a new capsule from two points and a radius.
		@param p0 The first point.
		@param p1 The second point.
		@param radius The radius of the capsule.
	*/
	Capsule(const Vector3& p0, const Vector3& p1, real radius);

	/**
		Initialize a new capsule from a center, a height and a radius.
		@param center The center of the capsule.
		@param axis The axis of the capsule.
		@param height The height of the capsule.
		@param radius The radius of the capsule.
	*/
	Capsule(const Vector3& center, const Vector3& axis, real height, real radius);

	/** Copy constructor. */
	Capsule(const Capsule& value);
	//@}

	/** @name Binary Operators. */
	//@{
	/** Compares this instance to another instance to determine whether they are the same. */
	bool operator==(const Capsule& value) const;

	/** Compares this instance to another instance to determine whether they are different. */
	bool operator!=(const Capsule& value) const;
	//@}

	/** @name Properties. */
	//@{
	/**
		Gets the center of the capsule.
		@return The center of the capsule.
	*/
	Vector3 GetCenter() const;

	/**
		Gets the height of the capsule.
		@return The height of the capsule.
	*/
	real GetHeight() const;

	/**
		Gets the first point of the capsule.
		@return The first point of the capsule.
	*/
	const Vector3& GetPoint0() const;

	/**
		Sets the first point of the capsule.
		@param value The first point of the capsule.
	*/
	void SetPoint0(const Vector3& value);

	/**
		Gets the second point of the capsule.
		@return The second point of the capsule.
	*/
	const Vector3& GetPoint1() const;

	/**
		Sets the second point of the capsule.
		@param value The second point of the capsule.
	*/
	void SetPoint1(const Vector3& value);

	/**
		Gets the segment of the capsule.
		@return The segment of the capsule.
	*/
	const Segment& GetSegment() const;

	/**
		Sets the segment of the capsule.
		@param value The segment of the capsule.
	*/
	void SetSegment(const Segment& value);

	/**
		Gets the radius of the capsule.
		@return The radius of the capsule.
	*/
	real GetRadius() const;

	/**
		Sets the radius of the capsule.
		@param value The radius of the capsule.
	*/
	void SetRadius(real value);

	/**
		Gets the area of the capsule.
		@return The area of the capsule.
		@remarks area = Radius^2 4 Pi + 2 Pi Radius Height
	*/
	real GetArea() const;

	/**
		Gets the volume of the capsule.
		@return The volume of the capsule.
		@remarks volume = Radius^3 4/3 Pi + Radius^2 Height
	*/
	real GetVolume() const;
	//@}

	/** @name Operations. */
	//@{
	/**
		Returns whteher the capsule contains the specified point.
		@param value The point.
		@return true if the capsule contains the specified point; otherwise, false.
	*/
	bool ContainsPoint(const Vector3& value) const;

	/**
		Transforms the capsule by the specified matrix.
		@param transform The matrix.
	*/
	void Transform(const Matrix4& transform);

	/**
		Transforms the specified capsule by the specified matrix.
		@param capsule The capsule.
		@param transform The matrix.
		@return The result of the transformation.
	*/
	static Capsule Transform(const Capsule& capsule, const Matrix4& transform);
	//@}

protected:
	Segment _Segment;
	real _Radius;
};

#include "Capsule.inl"

}

#endif 
