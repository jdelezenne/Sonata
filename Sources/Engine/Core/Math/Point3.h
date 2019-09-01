/*=============================================================================
Point3.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_POINT3_H_
#define _SE_POINT3_H_

#include "Core/Common.h"
#include "Core/String.h"
#include "Core/ValueType.h"
#include "Core/Math/Math.h"
#include "Core/Math/Vector3.h"

namespace SonataEngine
{

/** Vector 3D. */
class SE_CORE_EXPORT Point3
{
public:
	/** Returns a Point3 with all of its components set to zero. */
	static const Point3 Zero;

	/** Returns a Point3 with ones in all of its components. */
	static const Point3 One;

public:
	union
	{
		struct
		{
			real X, Y, Z;
		};
		real Data[3];
	};

public:
	/** @name Constructors. */
	//@{
	Point3();
	Point3(real x, real y, real z);
	Point3(real value);
	Point3(const real* value);
	Point3(const Vector3& value);
	Point3(const Point3& value);
	//@}

	/** @name Casting Operators. */
	//@{
	/** Retrieves the components as an array of scalars. */
	operator const real*() const;
	operator real*();
	//@}

	/** @name Access Operators. */
	//@{
	/** Retrieves a component by its index. */
	real operator[](int index) const;
	real& operator[](int index);
	//@}

	/** @name Unary Operators. */
	//@{
	Point3 operator+() const;
	Point3 operator-() const;
	//@}

	/** @name Assignment Operators. */
	//@{
	/** Adds this instance to another value. */
	Point3& operator+=(const Point3& value);

	/** Substracts this instance by another value. */
	Point3& operator-=(const Point3& value);

	/** Multiplies this instance by another value. */
	Point3& operator*=(const Point3& value);

	/** Multiplies this instance by a scalar. */
	Point3& operator*=(real value);

	/** Divides this instance by another value. */
	Point3& operator/=(const Point3& value);

	/** Divides this instance by a scalar. */
	Point3& operator/=(real value);
	//@}

	/** @name Binary Operators. */
	//@{
	/** Adds two points. */
	Point3 operator+(const Point3& value) const;

	/** Substracts two points. */
	Point3 operator-(const Point3& value) const;

	/** Multiplies two points. */
	Point3 operator*(const Point3& value) const;

	/** Multiplies a point by a scalar. */
	Point3 operator*(real value) const;

	/** Divides two points. */
	Point3 operator/(const Point3& value) const;

	/** Divides a point by a scalar. */
	Point3 operator/(real value) const;

	/** Compares this instance to another instance to determine whether they are the same. */
	bool operator==(const Point3& value) const;

	/** Compares this instance to another instance to determine whether they are different. */
	bool operator!=(const Point3& value) const;
	//@}

	friend Point3 operator+(real left, const Point3& right);
	friend Point3 operator-(real left, const Point3& right);
	friend Point3 operator*(real left, const Point3& right);
	friend Point3 operator/(real left, const Point3& right);

	/** @name Operations. */
	//@{
	/** Returns a point pointing in the opposite direction. */
	static Point3 Negate(const Point3& value);

	/** Adds two points. */
	static Point3 Add(const Point3& value1, const Point3& value2);

	/** Substracts two points. */
	static Point3 Substract(const Point3& value1, const Point3& value2);

	/** Multiplies two points. */
	static Point3 Multiply(const Point3& value1, const Point3& value2);

	/** Multiplies a point by a scalar. */
	static Point3 Multiply(const Point3& value1, real value2);

	/** Divides two points. */
	static Point3 Divide(const Point3& value1, const Point3& value2);

	/** Divides a point by a scalar. */
	static Point3 Divide(const Point3& value1, real value2);

	/** Returns the sum of the elements. */
	real Sum() const;

	/**
		Returns a point that is made up of the smallest components of two points.
	*/
	void Min(const Point3& value);
	static Point3 Min(const Point3& left, const Point3& right);

	/**
		Returns a point that is made up of the largest components of two points.
	*/
	void Max(const Point3& value);
	static Point3 Max(const Point3& left, const Point3& right);

	/**
		Clamp.
	*/
	static Point3 Clamp(const Point3& value, const Point3& min, const Point3& max);
	//@}

	/** Interpolations. */
	//@{
	/**
		Performs a linear interpolation between two points.
	*/
	static Point3 Lerp(const Point3& value1, const Point3& value2, real amount);

	/**
		Performs an interpolation between two points using a cubic equation.
	*/
	static Point3 SmoothStep(const Point3& value1, const Point3& value2, real amount);

	/**
		Performs a Catmull-Rom interpolation using the specified positions.
	*/
	static Point3 CatmullRom(const Point3& value1, const Point3& value2, const Point3& value3, const Point3& value4, real amount);

	/**
		Performs a Hermite spline interpolation between two points using the specified tangents.
	*/
	static Point3 Hermite(const Point3& value1, const Point3& tangent1, const Point3& value2, const Point3& tangent2, real amount);
	//@}

	Point3 Barycentric(const Point3& value1, const Point3& value2, const Point3& value3, real amount1, real amount2);

	bool Equals(const Point3& value) const;
	bool Equals(const Point3& value, real epsilon) const;

	int32 GetHashCode() const;

	String ToString() const;
};

#include "Point3.inl"

}

#endif
