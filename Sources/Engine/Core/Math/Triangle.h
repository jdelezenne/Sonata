/*=============================================================================
Triangle.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_TRIANGLE_H_
#define _SE_TRIANGLE_H_

#include "Core/Common.h"
#include "Core/Math/Math.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Plane.h"

namespace SonataEngine
{

/**
	@brief Triangle.

	A triangle is defined by three vertices.
*/
class SE_CORE_EXPORT Triangle
{
public:
	/** @name Constructors. */
	//@{
	/** Default constructor. */
	Triangle();

	/**
		Constructor with initial values.
		@param p0 Vertex of triangle.
		@param p1 Vertex of triangle.
		@param p2 Vertex of triangle.
	*/
	Triangle(const Vector3& p0, const Vector3& p1, const Vector3& p2);

	/**
		Constructor with initial values.
		@param p Vertices of triangle.
	*/
	Triangle(const Vector3 p[3]);

	/** Copy constructor. */
	Triangle(const Triangle& value);
	//@}

	/** @name Binary Operators. */
	//@{
	/** Compares this instance to another instance to determine whether they are the same. */
	bool operator==(const Triangle& value) const;

	/** Compares this instance to another instance to determine whether they are different. */
	bool operator!=(const Triangle& value) const;
	//@}

	/** @name Operations. */
	//@{
	/**
		Gets the area of the triangle.
		@return The area of the triangle.
		@remarks area = len((p0 - p1) ^ (p0 - p2)) / 2
	*/
	real GetArea() const;

	/**
		Gets the perimeter of the triangle.
		@return The perimeter of the triangle.
	*/
	real GetPerimeter() const;

	/**
		Returns a point in barycentric coordinates, using the points of the triangle.
		@param u Weighting factor.
		@param v Weighting factor.
		@remarks v0 + u(v1 - v0) + v(v2 - v0)
	*/
	Vector3 GetBaryCentric(real u, real v) const;

	/**
		Gets the normal of the triangle.
		@return The normal of the triangle.
		@remarks normal = (p0 - p1) ^ (p0 - p2)
	*/
	Vector3 GetNormal() const;

	/**
		Gets the center of the triangle.
		@return The center of the triangle.
		@remarks center = (p0 + p1 + p2) / 3
	*/
	Vector3 GetCenter() const;

	/**
		Gets the plane defined by the points of the triangle.
		@return The plane defined by the points of the triangle.
	*/
	Plane GetPlane() const;

	/** Flips the order of the points of the triangle. */
	void Flip();

	/** Returns the minimum distance to a point.
		@todo
	*/
	real GetDistance(const Vector3& value) const;

	/** @todo */
	bool ContainsPoint(const Vector3& value) const;
	//@}

public:
	/** Points of the triangle. */
	Vector3 p0, p1, p2;        
};

#include "Triangle.inl"

}

#endif 
