/*=============================================================================
Vector4.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_VECTOR4_H_
#define _SE_VECTOR4_H_

#include "Core/Common.h"
#include "Core/String.h"
#include "Core/ValueType.h"
#include "Core/Math/Math.h"
#include "Core/Math/Vector2.h"
#include "Core/Math/Vector3.h"

namespace SonataEngine
{

/** Vector 4D. */
class SE_CORE_EXPORT Vector4
{
public:
	/** Returns a Vector4 with all of its components set to zero. */
	static const Vector4 Zero;

	/** Returns a Vector4 with ones in all of its components. */
	static const Vector4 One;

	/** Returns the x unit Vector4 (1, 0, 0, 0). */
	static const Vector4 UnitX;

	/** Returns the y unit Vector4 (0, 1, 0, 0). */
	static const Vector4 UnitY;

	/** Returns the z unit Vector4 (0, 0, 1, 0). */
	static const Vector4 UnitZ;

	/** Returns the w unit Vector4 (0, 0, 0, 1). */
	static const Vector4 UnitW;

public:
	union
	{
		struct
		{
			real X, Y, Z, W;
		};
		real Data[4];
	};

public:
	/** @name Constructors. */
	//@{
	Vector4();
	Vector4(real x, real y, real z, real w);
	Vector4(real value);
	Vector4(const real* value);
	Vector4(const Vector2& value, real z, real w = 1.0);
	Vector4(const Vector3& value, real w = 1.0);
	Vector4(const Vector4& value);
	//@}

	/** @name Casting Operators. */
	//@{
	/** Retrieves the components as an array of scalars. */
	operator const real*() const;
	operator real*();

	/** Retrieves the 3D part of the vector. */
	operator const Vector3() const;
	operator Vector3();
	//@}

	/** @name Access Operators. */
	//@{
	/** Retrieves a component by its index. */
	real operator[](int index) const;
	real& operator[](int index);
	//@}

	/** @name Unary Operators. */
	//@{
	Vector4 operator+() const;
	Vector4 operator-() const;
	//@}

	/** @name Assignment Operators. */
	//@{
	/** Adds this instance to another value. */
	Vector4& operator+=(const Vector4& value);

	/** Substracts this instance by another value. */
	Vector4& operator-=(const Vector4& value);

	/** Multiplies this instance by another value. */
	Vector4& operator*=(const Vector4& value);

	/** Multiplies this instance by a scalar. */
	Vector4& operator*=(real value);

	/** Divides this instance by another value. */
	Vector4& operator/=(const Vector4& value);

	/** Divides this instance by a scalar. */
	Vector4& operator/=(real value);
	//@}

	/** @name Binary Operators. */
	//@{
	/** Adds two vectors. */
	Vector4 operator+(const Vector4& value) const;

	/** Substracts two vectors. */
	Vector4 operator-(const Vector4& value) const;

	/** Multiplies two vectors. */
	Vector4 operator*(const Vector4& value) const;

	/** Multiplies a vector by a scalar. */
	Vector4 operator*(real value) const;

	/** Divides two vectors. */
	Vector4 operator/(const Vector4& value) const;

	/** Divides a vector by a scalar. */
	Vector4 operator/(real value) const;

	/** Compares this instance to another instance to determine whether they are the same. */
	bool operator==(const Vector4& value) const;

	/** Compares this instance to another instance to determine whether they are different. */
	bool operator!=(const Vector4& value) const;
	//@}

	friend Vector4 operator+(real left, const Vector4& right);
	friend Vector4 operator-(real left, const Vector4& right);
	friend Vector4 operator*(real left, const Vector4& right);
	friend Vector4 operator/(real left, const Vector4& right);

	/** @name Operations. */
	//@{
	/** Returns a vector pointing in the opposite direction. */
	static Vector4 Negate(const Vector4& value);

	/** Adds two vectors. */
	static Vector4 Add(const Vector4& value1, const Vector4& value2);

	/** Substracts two vectors. */
	static Vector4 Substract(const Vector4& value1, const Vector4& value2);

	/** Multiplies two vectors. */
	static Vector4 Multiply(const Vector4& value1, const Vector4& value2);

	/** Multiplies a vector by a scalar. */
	static Vector4 Multiply(const Vector4& value1, real value2);

	/** Divides two vectors. */
	static Vector4 Divide(const Vector4& value1, const Vector4& value2);

	/** Divides a vector by a scalar. */
	static Vector4 Divide(const Vector4& value1, real value2);

	/** Returns the length. */
	real Length() const;

	/** Returns the square of the length. */
	real LengthSquared() const;

	/** Returns the distance to a point. */
	real Distance(const Vector4& value) const;
	static real Distance(const Vector4& left, const Vector4& right);

	/** Returns the square of the distance to a point. */
	real DistanceSquared(const Vector4& value) const;
	static real DistanceSquared(const Vector4& left, const Vector4& right);

	/**
		Normalize.
		@remarks
			x / Length()
			y / Length()
			z / Length()
			w / Length()
	*/
	void Normalize();
	static Vector4 Normalize(const Vector4& value);

	/** Scale. */
	void Scale(const Vector4& value);
	static Vector4 Scaling(const Vector4& left, const Vector4& right);

	/**
		Returns the dot product.
		@remarks Out = x1 * x2 + y1 * y2 + z1 * z2 + w1 * w2
	*/
	real Dot(const Vector4& value) const;
	static real Dot(const Vector4& left, const Vector4& right);

	/**
		Returns the angle between two vectors.
	*/
	real Angle(const Vector4& value) const;
	static real Angle(const Vector4& left, const Vector4& right);

	/**
		Returns whether the two vectors have the same direction.
	*/
	bool SameDirection(const Vector4& value) const;
	static bool SameDirection(const Vector4& left, const Vector4& right);

	/**
		Returns a vector that is made up of the smallest components of two vectors.
	*/
	void Min(const Vector4& value);
	static Vector4 Min(const Vector4& left, const Vector4& right);

	/**
		Returns a vector that is made up of the largest components of two vectors.
	*/
	void Max(const Vector4& value);
	static Vector4 Max(const Vector4& left, const Vector4& right);

	/**
		Clamp.
	*/
	static Vector4 Clamp(const Vector4& value, const Vector4& min, const Vector4& max);
	//@}

	/** Interpolations. */
	//@{
	/**
		Performs a linear interpolation between two vectors.
	*/
	static Vector4 Lerp(const Vector4& value1, const Vector4& value2, real amount);

	/**
		Performs an interpolation between two vectors using a cubic equation.
	*/
	static Vector4 SmoothStep(const Vector4& value1, const Vector4& value2, real amount);

	/**
		Performs a Catmull-Rom interpolation using the specified positions.
	*/
	static Vector4 CatmullRom(const Vector4& value1, const Vector4& value2, const Vector4& value3, const Vector4& value4, real amount);

	/**
		Performs a Hermite spline interpolation between two vectors using the specified tangents.
	*/
	static Vector4 Hermite(const Vector4& value1, const Vector4& tangent1, const Vector4& value2, const Vector4& tangent2, real amount);
	//@}

	/**
		Transforms a vector by a given matrix.
		@param value The vector.
		@return The transformed vector.
		@remarks
			(_00*x + _01*y + _03)
			(_10*x + _11*y + _13)
			(_20*x + _21*y + _23)
			(_30*x + _31*y + _33)
	*/
	static Vector4 Transform(const Vector2& vector, const Matrix4& matrix);

	/**
		Transforms a vector by a given matrix.
		@param value The vector.
		@return The transformed vector.
		@remarks
			(_00*x + _01*y + _02*z + _03)
			(_10*x + _11*y + _12*z + _13)
			(_20*x + _21*y + _22*z + _23)
			(_30*x + _31*y + _32*z + _33)
	*/
	static Vector4 Transform(const Vector3& vector, const Matrix4& matrix);

	/**
		Transforms a vector by a given matrix.
		@param value The vector.
		@return The transformed vector.
		@remarks
			(_00*x + _01*y + _02*z + _03*w)
			(_10*x + _11*y + _12*z + _13*w)
			(_20*x + _21*y + _22*z + _23*w)
			(_30*x + _31*y + _32*z + _33*w)
	*/
	static Vector4 Transform(const Vector4& vector, const Matrix4& matrix);

	/**
		Returns a point in Barycentric coordinates, using the specified values.
		@remarks
			value1 + (amount1 * (value2 - value1)) + (amount2 * (value3 - value1))
	*/
	static Vector4 Barycentric(const Vector4& value1, const Vector4& value2, const Vector4& value3, real amount1, real amount2);

	/**
		Computes the reflected vector of the specified vector at the specified normal.
		@param value Source vector.
		@param normal Normal.
		@return Reflected vector.
		@remarks v - 2 * n * (v.n)
	*/
	static Vector4 Reflect(const Vector4& value, const Vector4& normal);

	/**
		Computes the refracted vector of the specified vector at the specified normal.
		@param value Source vector.
		@param normal Normal.
		@param incident Incident indice of refraction.
		@param transmitted Transmitted indice of refraction.
		@return Refracted vector.
		@remarks see formula in Mathematics for Game Developers at p489.
	*/
	static Vector4 Refract(const Vector4& value, const Vector4& normal, real incident, real transmitted);

	bool Equals(const Vector4& value) const;
	bool Equals(const Vector4& value, real epsilon) const;

	int32 GetHashCode() const;

	String ToString() const;
};

#include "Vector4.inl"

}

#endif
