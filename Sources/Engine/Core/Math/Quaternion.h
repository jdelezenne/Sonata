/*=============================================================================
Quaternion.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_QUATERNION_H_
#define _SE_QUATERNION_H_

#include "Core/Common.h"
#include "Core/Math/Math.h"
#include "Core/Math/Vector3.h"

namespace SonataEngine
{

/**
	@brief Quaternion.

	@remarks
		A quaternnion contains a scalar component w and a 3D vector component v.
		The following notation is used: q = [w v]. A rotation of theta around a
		unit axis n is noted as follow: q = [cos(theta/2) sin(theta/2)*n].
		The main use for quaternions is slerp (spherical linear interpolation).
*/
class SE_CORE_EXPORT Quaternion
{
public:
	static const Quaternion Zero;
	static const Quaternion Identity;

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
	Quaternion();
	Quaternion(real x, real y, real z, real w);
	Quaternion(const Vector3& axis, real angle);
	Quaternion(const Quaternion& value);
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
	Quaternion operator+() const;
	Quaternion operator-() const;
	//@}

	/** @name Assignment Operators. */
	//@{
	/** Adds this instance to another quaternion. */
	Quaternion& operator+=(const Quaternion& value);

	/** Substracts this instance by another quaternion. */
	Quaternion& operator-=(const Quaternion& value);

	/** Multiplies this instance by another quaternion. */
	Quaternion& operator*=(const Quaternion& value);

	/** Multiplies this instance by a scalar. */
	Quaternion& operator*=(real value);

	/** Divides this instance by another quaternion. */
	Quaternion& operator/=(const Quaternion& value);

	/** Divides this instance by a scalar. */
	Quaternion& operator/=(real value);
	//@}

	/** @name Binary Operators. */
	//@{
	/** Adds two quaternions. */
	Quaternion operator+(const Quaternion& value) const;

	/** Substracts two quaternions. */
	Quaternion operator-(const Quaternion& value) const;

	/** Multiplies two quaternions. */
	Quaternion operator*(const Quaternion& value) const;

	/** Multiplies a quaternion by a scalar. */
	Quaternion operator*(real value) const;

	/** Divides two quaternions. */
	Quaternion operator/(const Quaternion& value) const;

	/** Divides a quaternion by a scalar. */
	Quaternion operator/(real value) const;

	/** Compares this instance to another instance to determine whether they are the same. */
	bool operator==(const Quaternion& value) const;

	/** Compares this instance to another instance to determine whether they are different. */
	bool operator!=(const Quaternion& value) const;
	//@}

	/** @name Operations. */
	//@{
	/** Makes this instance the identity quaternion. */
	void SetIdentity();

	/** Returns whether the matrix is the identity quaternion. */
	bool IsIdentity() const;

	/** Flips the sign of each component of the quaternion. */
	static Quaternion Negate(const Quaternion& value);

	/**
		Adds two quaternions.
		@remarks out = this + value.
	*/
	void Add(const Quaternion& value);

	/**
		Adds two quaternions.
		@remarks out = left + right.
	*/
	static Quaternion Add(const Quaternion& left, const Quaternion& right);

	/**
		Subtracts two quaternions.
		@remarks out = this - value.
	*/
	void Substract(const Quaternion& value);

	/**
		Subtracts two quaternions.
		@remarks out = left - right.
	*/
	static Quaternion Substract(const Quaternion& left, const Quaternion& right);

	/**
		Multiplies two quaternions.
		@remarks out = this * value.
	*/
	void Multiply(const Quaternion& value);

	/**
		Multiplies two quaternions.
		@remarks out = left * right.
	*/
	static Quaternion Multiply(const Quaternion& left, const Quaternion& right);

	/**
		Divides two quaternions.
		@remarks out = this * value.
	*/
	void Divide(const Quaternion& value);

	/**
		Divides two quaternions.
		@remarks out = left * right.
	*/
	static Quaternion Divide(const Quaternion& left, const Quaternion& right);

	/** Returns the length of a quaternion. */
	real Length();

	/** Returns the square of the length of a quaternion. */
	real LengthSquared();

	/** Returns the normal of a quaternion. */
	void Normalize();
	static Quaternion Normalize(const Quaternion& value);

	/**
		Returns the dot product of a quaternion.
		@remarks
			x1 * x2 + y1 * y2 + z1 * z2 + w1 * w2
	*/
	real Dot(const Quaternion& value) const;
	static real Dot(const Quaternion& left, const Quaternion& right);

	/**
		Returns the conjugate of a quaternion.
		@remarks 
			(-x, -y, -z, w)
	*/
	void Conjugate();
	static Quaternion Conjugate(const Quaternion& value);

	/** Conjugates and re-normalizes a quaternion. */
	void Invert();
	static Quaternion Invert(const Quaternion& value);

	/**
		Calculates the natural logarithm.
		@remarks
			Q == (cos(theta), sin(theta) * v) where |v| = 1
			The natural logarithm of Q is ln(Q) = (0, theta * v)
	*/
	void Log();
	static Quaternion Log(const Quaternion& value);

	/** Calculates the exponential. */
	void Exp();
	static Quaternion Exp(const Quaternion& value);
	//@}

	/** Interpolations. */
	//@{
	/** Interpolates between two quaternions, using linear interpolation. */
	static Quaternion Lerp(const Quaternion& left, const Quaternion& right, real amount);

	/** Interpolates between two quaternions, using spherical linear interpolation. */
	static Quaternion Slerp(const Quaternion& left, const Quaternion& right, real amount);

	/**
		Interpolates between two quaternions, using spherical cubic interpolation.
		@remarks
			This function uses the following sequence of spherical linear interpolation operations:
			Slerp(Slerp(q1, q2, t), Slerp(tangent1, tangent2, t), 2t(1-t))
	*/
	static Quaternion Squad(const Quaternion& q1, const Quaternion& tangent1, const Quaternion& tangent2, const Quaternion& q2, real amount);
	//@}

	/** Rotates the specified vector. */
	Vector3 Rotate(const Vector3& vector) const;
	static Vector3 Rotate(const Quaternion& quaternion, const Vector3& vector);

	/** Conversions. */
	//@{
	/**
		Creates a Quaternion from a vector and an angle to rotate about the vector.
		@remarks
			q = cos(angle/2)+sin(angle/2)*(x*i+y*j+z*k)
	*/
	static Quaternion CreateFromAxisAngle(const Vector3& axis, real angle);

	/**
		Creates a Quaternion from a rotation Matrix.
		@remarks
			q = cos(angle/2)+sin(angle/2)*(x*i+y*j+z*k)
	*/
	static Quaternion CreateFromRotationMatrix(const Matrix3& value);

	/**
		Retrieves the axis and angle of rotation from a Quaternion.
		@remarks
			q = cos(angle/2)+sin(angle/2)*(x*i+y*j+z*k)
			From Wild Magic
	*/
	void ToAxisAngle(Vector3& axis, real& angle) const;

	void ToAxes(Vector3& xAxis, Vector3& yAxis, Vector3& zAxis) const;

	/**
		Creates a rotation Matrix from a Quaternion.
		@remarks
			q = cos(angle/2)+sin(angle/2)*(x*i+y*j+z*k)
			From Wild Magic
	*/
	void ToRotationMatrix(Matrix3& result) const;
	//@}
};

#include "Quaternion.inl"

}

#endif
