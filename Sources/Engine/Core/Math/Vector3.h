/*=============================================================================
Vector3.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_VECTOR3_H_
#define _SE_VECTOR3_H_

#include "Core/Common.h"
#include "Core/String.h"
#include "Core/ValueType.h"
#include "Core/Math/Math.h"
#include "Core/Math/Vector2.h"

namespace SonataEngine
{

/** Vector 3D. */
class SE_CORE_EXPORT Vector3
{
public:
	/** Returns a Vector3 with all of its components set to zero. */
	static const Vector3 Zero;

	/** Returns a Vector3 with ones in all of its components. */
	static const Vector3 One;

	/** Returns the x unit Vector3 (1, 0, 0). */
	static const Vector3 UnitX;

	/** Returns the y unit Vector3 (0, 1, 0). */
	static const Vector3 UnitY;

	/** Returns the z unit Vector3 (0, 0, 1). */
	static const Vector3 UnitZ;

	/** Returns a unit Vector3 pointing up (0, 1, 0). */
	static const Vector3 Up;

	/** Returns a unit Vector3 pointing down (0, -1, 0). */
	static const Vector3 Down;

	/** Returns a unit Vector3 pointing left (-1, 0, 0). */
	static const Vector3 Left;

	/** Returns a unit Vector3 pointing right (1, 0, 0). */
	static const Vector3 Right;

	/** Returns a unit Vector3 pointing forward (0, 0, -1). */
	static const Vector3 Forward;

	/** Returns a unit Vector3 pointing backwards (0, 0, 1). */
	static const Vector3 Backward;

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
	Vector3();
	Vector3(real x, real y, real z);
	Vector3(real value);
	Vector3(const real* value);
	Vector3(const Vector2& value, real z);
	Vector3(const Vector3& value);
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
	Vector3 operator+() const;
	Vector3 operator-() const;
	//@}

	/** @name Assignment Operators. */
	//@{
	/** Adds this instance to another value. */
	Vector3& operator+=(const Vector3& value);

	/** Substracts this instance by another value. */
	Vector3& operator-=(const Vector3& value);

	/** Multiplies this instance by another value. */
	Vector3& operator*=(const Vector3& value);

	/** Multiplies this instance by a scalar. */
	Vector3& operator*=(real value);

	/** Divides this instance by another value. */
	Vector3& operator/=(const Vector3& value);

	/** Divides this instance by a scalar. */
	Vector3& operator/=(real value);
	//@}

	/** @name Binary Operators. */
	//@{
	/** Adds two vectors. */
	Vector3 operator+(const Vector3& value) const;

	/** Substracts two vectors. */
	Vector3 operator-(const Vector3& value) const;

	/** Multiplies two vectors. */
	Vector3 operator*(const Vector3& value) const;

	/** Multiplies a vector by a scalar. */
	Vector3 operator*(real value) const;

	/** Divides two vectors. */
	Vector3 operator/(const Vector3& value) const;

	/** Divides a vector by a scalar. */
	Vector3 operator/(real value) const;

	/** Compares this instance to another instance to determine whether they are the same. */
	bool operator==(const Vector3& value) const;

	/** Compares this instance to another instance to determine whether they are different. */
	bool operator!=(const Vector3& value) const;
	//@}

	friend Vector3 operator+(real left, const Vector3& right);
	friend Vector3 operator-(real left, const Vector3& right);
	friend Vector3 operator*(real left, const Vector3& right);
	friend Vector3 operator/(real left, const Vector3& right);

	/** @name Operations. */
	//@{
	/** Returns a vector pointing in the opposite direction. */
	static Vector3 Negate(const Vector3& value);

	/** Adds two vectors. */
	static Vector3 Add(const Vector3& value1, const Vector3& value2);

	/** Substracts two vectors. */
	static Vector3 Substract(const Vector3& value1, const Vector3& value2);

	/** Multiplies two vectors. */
	static Vector3 Multiply(const Vector3& value1, const Vector3& value2);

	/** Multiplies a vector by a scalar. */
	static Vector3 Multiply(const Vector3& value1, real value2);

	/** Divides two vectors. */
	static Vector3 Divide(const Vector3& value1, const Vector3& value2);

	/** Divides a vector by a scalar. */
	static Vector3 Divide(const Vector3& value1, real value2);

	/** Returns the length. */
	real Length() const;

	/** Returns the square of the length. */
	real LengthSquared() const;

	/** Returns the distance to a point. */
	real Distance(const Vector3& value) const;
	static real Distance(const Vector3& left, const Vector3& right);

	/** Returns the square of the distance to a point. */
	real DistanceSquared(const Vector3& value) const;
	static real DistanceSquared(const Vector3& left, const Vector3& right);

	/**
		Normalize.
		@remarks
			x / Length()
			y / Length()
			z / Length()
	*/
	void Normalize();
	static Vector3 Normalize(const Vector3& value);

	/** Scale. */
	void Scale(const Vector3& value);
	static Vector3 Scaling(const Vector3& left, const Vector3& right);

	/**
		Returns the dot product.
		@remarks Out = x1 * x2 + y1 * y2 + z1 * z2
	*/
	real Dot(const Vector3& value) const;
	static real Dot(const Vector3& left, const Vector3& right);

	/**
		Returns the cross product.
		@remarks
				y * v.z - z * v.y
				z * v.x - x * v.z
				x * v.y - y * v.x
	*/
	Vector3 Cross(const Vector3& value) const;
	static Vector3 Cross(const Vector3& left, const Vector3& right);

	/**
		Returns the angle between two vectors.
	*/
	real Angle(const Vector3& value) const;
	static real Angle(const Vector3& left, const Vector3& right);

	/**
		Returns whether the two vectors have the same direction.
	*/
	bool SameDirection(const Vector3& value) const;
	static bool SameDirection(const Vector3& left, const Vector3& right);

	/**
		Returns a vector that is made up of the smallest components of two vectors.
	*/
	void Min(const Vector3& value);
	static Vector3 Min(const Vector3& left, const Vector3& right);

	/**
		Returns a vector that is made up of the largest components of two vectors.
	*/
	void Max(const Vector3& value);
	static Vector3 Max(const Vector3& left, const Vector3& right);

	/**
		Clamp.
	*/
	static Vector3 Clamp(const Vector3& value, const Vector3& min, const Vector3& max);
	//@}

	/** Interpolations. */
	//@{
	/**
		Performs a linear interpolation between two vectors.
	*/
	static Vector3 Lerp(const Vector3& value1, const Vector3& value2, real amount);

	/**
		Performs an interpolation between two vectors using a cubic equation.
	*/
	static Vector3 SmoothStep(const Vector3& value1, const Vector3& value2, real amount);

	/**
		Performs a Catmull-Rom interpolation using the specified positions.
	*/
	static Vector3 CatmullRom(const Vector3& value1, const Vector3& value2, const Vector3& value3, const Vector3& value4, real amount);

	/**
		Performs a Hermite spline interpolation between two vectors using the specified tangents.
	*/
	static Vector3 Hermite(const Vector3& value1, const Vector3& tangent1, const Vector3& value2, const Vector3& tangent2, real amount);
	//@}

	/**
		Transforms a vector by a given matrix.
		@param value The vector.
		@return The transformed vector.
		@remarks
			(_00*x + _01*y + _02*z + _03)
			(_10*x + _11*y + _12*z + _13)
			(_20*x + _21*y + _22*z + _23)
	*/
	static Vector3 Transform(const Vector3& position, const Matrix4& matrix);

	/**
		Transforms a vector normal by a given matrix.
		@param value The vector.
		@return The transformed vector.
		@remarks
			(_00*x + _01*y + _02*z)
			(_10*x + _11*y + _12*z)
			(_20*x + _21*y + _22*z)
	*/
	static Vector3 TransformNormal(const Vector3& normal, const Matrix4& matrix);

	/**
		Transforms a vector by a given matrix including homogenous division.
		@param value The vector.
		@return The transformed vector.
		@remarks
			(_00*x + _01*y + _02*z + _03) / (_30*x + _31*y + _32*z + _33)
			(_10*x + _11*y + _12*z + _13) / (_30*x + _31*y + _32*z + _33)
			(_20*x + _21*y + _22*z + _23) / (_30*x + _31*y + _32*z + _33)
	*/
	static Vector3 Project(const Vector3& vector, const Matrix4& matrix);

	/**
		Returns a point in Barycentric coordinates, using the specified values.
		@remarks
			value1 + (amount1 * (value2 - value1)) + (amount2 * (value3 - value1))
	*/
	static Vector3 Barycentric(const Vector3& value1, const Vector3& value2, const Vector3& value3, real amount1, real amount2);

	/**
		Computes the reflected vector of the specified vector at the specified normal.
		@param value Source vector.
		@param normal Normal.
		@return Reflected vector.
		@remarks v - 2 * n * (v.n)
	*/
	static Vector3 Reflect(const Vector3& value, const Vector3& normal);

	/**
		Computes the refracted vector of the specified vector at the specified normal.
		@param value Source vector.
		@param normal Normal.
		@param incident Incident indice of refraction.
		@param transmitted Transmitted indice of refraction.
		@return Refracted vector.
		@remarks see formula in Mathematics for Game Developers at p489.
	*/
	static Vector3 Refract(const Vector3& value, const Vector3& normal, real incident, real transmitted);

	bool Equals(const Vector3& value) const;
	bool Equals(const Vector3& value, real epsilon) const;

	int32 GetHashCode() const;

	String ToString() const;
};

#include "Vector3.inl"

}

#endif
