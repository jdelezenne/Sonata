/*=============================================================================
Vector2.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_VECTOR2_H_
#define _SE_VECTOR2_H_

#include "Core/Common.h"
#include "Core/String.h"
#include "Core/ValueType.h"
#include "Core/Math/Math.h"

namespace SonataEngine
{

/** Vector 2D. */
class SE_CORE_EXPORT Vector2
{
public:
	/** Returns a Vector2 with all of its components set to zero. */
	static const Vector2 Zero;

	/** Returns a Vector2 with ones in all of its components. */
	static const Vector2 One;

	/** Returns the x unit Vector2 (1, 0). */
	static const Vector2 UnitX;

	/** Returns the y unit Vector2 (0, 1). */
	static const Vector2 UnitY;

public:
	union
	{
		struct
		{
			real X, Y;
		};
		real Data[2];
	};

public:
	/** @name Constructors. */
	//@{
	Vector2();
	Vector2(real x, real y);
	Vector2(real value);
	Vector2(const real* value);
	Vector2(const Vector2& value);
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
	Vector2 operator+() const;
	Vector2 operator-() const;
	//@}

	/** @name Assignment Operators. */
	//@{
	/** Adds this instance to another vector. */
	Vector2& operator+=(const Vector2& value);

	/** Substracts this instance by another vector. */
	Vector2& operator-=(const Vector2& value);

	/** Multiplies this instance by another value. */
	Vector2& operator*=(const Vector2& value);

	/** Multiplies this instance by a scalar. */
	Vector2& operator*=(real value);

	/** Divides this instance by another value. */
	Vector2& operator/=(const Vector2& value);

	/** Divides this instance by a scalar. */
	Vector2& operator/=(real value);
	//@}

	/** @name Binary Operators. */
	//@{
	/** Adds two vectors. */
	Vector2 operator+(const Vector2& value) const;

	/** Substracts two vectors. */
	Vector2 operator-(const Vector2& value) const;

	/** Multiplies two vectors. */
	Vector2 operator*(const Vector2& value) const;

	/** Multiplies a vector by a scalar. */
	Vector2 operator*(real value) const;

	/** Divides two vectors. */
	Vector2 operator/(const Vector2& value) const;

	/** Divides a vector by a scalar. */
	Vector2 operator/(real value) const;

	/** Compares this instance to another instance to determine whether they are the same. */
	bool operator==(const Vector2& value) const;

	/** Compares this instance to another instance to determine whether they are different. */
	bool operator!=(const Vector2& value) const;
	//@}

	friend Vector2 operator+(real left, const Vector2& right);
	friend Vector2 operator-(real left, const Vector2& right);
	friend Vector2 operator*(real left, const Vector2& right);
	friend Vector2 operator/(real left, const Vector2& right);

	/** @name Operations. */
	//@{
	/** Returns a vector pointing in the opposite direction. */
	static Vector2 Negate(const Vector2& value);

	/** Adds two vectors. */
	static Vector2 Add(const Vector2& value1, const Vector2& value2);

	/** Substracts two vectors. */
	static Vector2 Substract(const Vector2& value1, const Vector2& value2);

	/** Multiplies two vectors. */
	static Vector2 Multiply(const Vector2& value1, const Vector2& value2);

	/** Multiplies a vector by a scalar. */
	static Vector2 Multiply(const Vector2& value1, real value2);

	/** Divides two vectors. */
	static Vector2 Divide(const Vector2& value1, const Vector2& value2);

	/** Divides a vector by a scalar. */
	static Vector2 Divide(const Vector2& value1, real value2);

	/** Returns the length. */
	real Length() const;

	/** Returns the square of the length. */
	real LengthSquared() const;

	/** Returns the distance to a point. */
	real Distance(const Vector2& value) const;
	static real Distance(const Vector2& left, const Vector2& right);

	/** Returns the square of the distance to a point. */
	real DistanceSquared(const Vector2& value) const;
	static real DistanceSquared(const Vector2& left, const Vector2& right);

	/**
		Normalize.
		@remarks
			x / Length()
			y / Length()
	*/
	void Normalize();
	static Vector2 Normalize(const Vector2& value);

	/** Scale. */
	void Scale(const Vector2& value);
	static Vector2 Scaling(const Vector2& left, const Vector2& right);

	/**
		Returns the dot product.
		@remarks x1 * x2 + y1 * y2
	*/
	real Dot(const Vector2& value) const;
	static real Dot(const Vector2& left, const Vector2& right);

	/**
		Returns the angle between two vectors.
	*/
	real Angle(const Vector2& value) const;
	static real Angle(const Vector2& left, const Vector2& right);

	/**
		Returns whether the two vectors have the same direction.
	*/
	bool SameDirection(const Vector2& value) const;
	static bool SameDirection(const Vector2& left, const Vector2& right);

	/**
		Returns a vector that is made up of the smallest components of two vectors.
	*/
	void Min(const Vector2& value);
	static Vector2 Min(const Vector2& left, const Vector2& right);

	/**
		Returns a vector that is made up of the largest components of two vectors.
	*/
	void Max(const Vector2& value);
	static Vector2 Max(const Vector2& left, const Vector2& right);

	/**
		Clamp.
	*/
	static Vector2 Clamp(const Vector2& value, const Vector2& min, const Vector2& max);
	//@}

	/** Interpolations. */
	//@{
	/**
		Performs a linear interpolation between two vectors.
	*/
	static Vector2 Lerp(const Vector2& value1, const Vector2& value2, real amount);

	/**
		Performs an interpolation between two vectors using a cubic equation.
	*/
	static Vector2 SmoothStep(const Vector2& value1, const Vector2& value2, real amount);

	/**
		Performs a Catmull-Rom interpolation using the specified positions.
	*/
	static Vector2 CatmullRom(const Vector2& value1, const Vector2& value2, const Vector2& value3, const Vector2& value4, real amount);

	/**
		Performs a Hermite spline interpolation between two vectors using the specified tangents.
	*/
	static Vector2 Hermite(const Vector2& value1, const Vector2& tangent1, const Vector2& value2, const Vector2& tangent2, real amount);
	//@}

	/**
		Transforms a vector by a given matrix.
		@param value The vector.
		@return The transformed vector.
		@remarks
			(_00*x + _01*y + _03)
			(_10*x + _11*y + _13)
			(_20*x + _21*y + _23)
	*/
	static Vector2 Transform(const Vector2& position, const Matrix4& matrix);

	/**
		Transforms a vector normal by a given matrix.
		@param value The vector.
		@return The transformed vector.
		@remarks
			(_00*x + _01*y + _02*z)
			(_10*x + _11*y + _12*z)
			(_20*x + _21*y + _22*z)
	*/
	static Vector2 TransformNormal(const Vector2& normal, const Matrix4& matrix);

	/**
		Transforms a vector by a given matrix including homogenous division.
		@param value The vector.
		@return The transformed vector.
		@remarks
			(_00*x + _01*y + _03) / (_30*x + _31*y + _33)
			(_10*x + _11*y + _13) / (_30*x + _31*y + _33)
			(_20*x + _21*y + _23) / (_30*x + _31*y + _33)
	*/
	static Vector2 Project(const Vector2& vector, const Matrix4& matrix);

	/**
		Returns a point in Barycentric coordinates, using the specified values.
		@remarks
			value1 + (amount1 * (value2 - value1)) + (amount2 * (value3 - value1))
	*/
	static Vector2 Barycentric(const Vector2& value1, const Vector2& value2, const Vector2& value3, real amount1, real amount2);

	/**
		Computes the reflected vector of the specified vector at the specified normal.
		@param value Source vector.
		@param normal Normal.
		@return Reflected vector.
		@remarks v - 2 * n * (v.n)
	*/
	static Vector2 Reflect(const Vector2& value, const Vector2& normal);

	/**
		Computes the refracted vector of the specified vector at the specified normal.
		@param value Source vector.
		@param normal Normal.
		@param incident Incident indice of refraction.
		@param transmitted Transmitted indice of refraction.
		@return Refracted vector.
		@see Vector3::Refract
	*/
	static Vector2 Refract(const Vector2& value, const Vector2& normal, real incident, real transmitted);

	bool Equals(const Vector2& value) const;
	bool Equals(const Vector2& value, real epsilon) const;

	int32 GetHashCode() const;

	String ToString() const;
};

#include "Vector2.inl"

}

#endif
