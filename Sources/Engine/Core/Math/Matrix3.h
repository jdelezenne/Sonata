/*=============================================================================
Matrix3.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_MATRIX3_H_
#define _SE_MATRIX3_H_

#include "Core/Common.h"
#include "Core/System/Memory.h"
#include "Core/Math/Math.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Vector4.h"
#include "Core/Math/Quaternion.h"

namespace SonataEngine
{

class Matrix4;

/**
	@brief Matrix 3x3.

	This matrix is used to represent a 3D orientation. The matrix contains 3
	vectors to represent the orientation along the 3 axis.
	Euler angles and Quaternions are more useful for of describing an
	orientation but this matrix can easily be extended to a transform matrix.
*/
class SE_CORE_EXPORT Matrix3
{
public:
	static const Matrix3 Zero;
	static const Matrix3 Identity;

public:
	union
	{
		struct
		{
			real M00, M01, M02;
			real M10, M11, M12;
			real M20, M21, M22;
		};
		real M[3][3];
		real Data[9];
	};

public:
	/** @name Constructors. */
	//@{
	/** Default constructor. */
	Matrix3();

	/** Initialize a new matrix with initial values. */
	Matrix3(
		real m00, real m01, real m02,
		real m10, real m11, real m12,
		real m20, real m21, real m22);
	
	/** Initialize a new matrix with initial values. */
	Matrix3(const real value[9]);

	/** Copy constructor. */
	Matrix3(const Matrix3& value);
	//@}

	/** @name Casting Operators. */
	//@{
	/** Retrieves the matrix elements as an array of scalars. */
	operator const real*() const;
	operator real*();
	//@}

	/** @name Access Operators. */
	//@{
	/** Retrieves a given matrix element by its row and column indices. */
	real operator()(int row, int col) const;
	real& operator()(int row, int col);
	//@}

	/** @name Assignment Operators. */
	//@{
	/** Adds this instance to another matrix. */
	Matrix3& operator+=(const Matrix3& value);

	/** Substracts this instance by another matrix. */
	Matrix3& operator-=(const Matrix3& value);

	/** Multiplies this instance by another matrix. */
	Matrix3& operator*=(const Matrix3& value);

	/** Multiplies this instance by a scalar. */
	Matrix3& operator*=(real value);

	/** Divides this instance by a scalar. */
	Matrix3& operator/=(real value);
	//@}

	/** @name Unary Operators. */
	//@{
	Matrix3 operator+() const;
	Matrix3 operator-() const;
	//@}

	/** @name Binary Operators. */
	//@{
	/** Adds two matrices. */
	Matrix3 operator+(const Matrix3& value) const;

	/** Substracts two matrices. */
	Matrix3 operator-(const Matrix3& value) const;

	/** Multiplies two matrices. */
	Matrix3 operator*(const Matrix3& value) const;

	/** Multiplies a matrice by a scalar. */
	Matrix3 operator*(real value) const;

	/** Divides a matrice by a scalar. */
	Matrix3 operator/(real value) const;

	/** Compares this instance to another instance to determine whether they are the same. */
	bool operator==(const Matrix3& value) const;

	/** Compares this instance to another instance to determine whether they are different. */
	bool operator!=(const Matrix3& value) const;
	//@}

	/** @name Operations. */
	//@{
	/**
		Gets a row.
		@param row The index of row.
		@return The row.
	*/
	Vector3 GetRow(int row) const;

	/**
		Sets a row.
		@param row The index of row.
		@param value The row.
	*/
	void SetRow(int row, const Vector3& value);

	/**
		Gets a column.
		@param column The index of column.
		@return The column.
	*/
	Vector3 GetColumn(int column) const;

	/**
		Sets a column.
		@param column The index of column.
		@param value The column.
	*/
	void SetColumn(int column, const Vector3& value);

	/** Makes this instance the identity matrix. */
	void SetIdentity();

	/** Returns whether the matrix is the identity matrix. */
	bool IsIdentity() const;

	/** Retrieves the determinant of the matrix. */
	real Determinant() const;

	/** Transposes the matrix. */
	void Transpose();

	/**
		Transposes a matrix.
		@param value The matrix to transpose.
		@return The transposed matrix.
	*/
	static Matrix3 Transpose(const Matrix3& value);

	/**
		Adds two matrices.
		@param left Source matrix.
		@param right Source matrix.
	*/
	void Add(const Matrix3& value);
	static Matrix3 Add(const Matrix3& left, const Matrix3& right);

	/**
		Subtracts one matrix from another.
		@param left Source matrix.
		@param right Source matrix.
	*/
	void Substract(const Matrix3& value);
	static Matrix3 Substract(const Matrix3& left, const Matrix3& right);

	/**
		Multiplies two matrices.
		@param value Source matrix.
		@remarks out = this * value.
	*/
	void Multiply(const Matrix3& value);

	/**
		Multiplies two matrices.
		@param left Source matrix.
		@param right Source matrix.
		@return The result of the multiplication.
		@remarks out = left * right.
	*/
	static Matrix3 Multiply(const Matrix3& left, const Matrix3& right);

	/**
		Matrix multiplication, followed by a transpose.
		@param value Source matrix.
		@remarks out = T(this * value).
	*/
	void MultiplyTranspose(const Matrix3& value);

	/**
		Matrix multiplication, followed by a transpose.
		@param left Source matrix.
		@param right Source matrix.
		@return The result.
		@remarks out = T(left * right).
	*/
	static Matrix3 MultiplyTranspose(const Matrix3& left, const Matrix3& right);

	/**
		Computes the inverse of the matrix.
	*/
	void Invert();

	/**
		Computes the inverse of a matrix.
		@patam value The matrix to inverse.
		@return The inverse of the matrix.
	*/
	static Matrix3 Invert(const Matrix3& value);
	//@}

	/** Transformations. */
	//@{
	/**
		Transforms a 3D vector (x, y, z, 1) by the matrix.
		@param value The vector.
		@return The transformed vector.
		@remarks
			(_00*x + _01*y + _02*z + _03) / (_30*x + _31*y + _32*z + _33)
			(_10*x + _11*y + _12*z + _13) / (_30*x + _31*y + _32*z + _33)
			(_20*x + _21*y + _22*z + _23) / (_30*x + _31*y + _32*z + _33)
	*/
	Vector3 Transform(const Vector3& value) const;

	/**
		Rotates a matrix around the x-axis.
		@param angle The angle of the rotation.
		@remarks
			_00 _01*cos+_02*sin _01*-sin+_02*cos
			_10 _11*cos+_12*sin _11*-sin+_12*cos
			_20 _21*cos+_22*sin _21*-sin+_22*cos
	*/
	void RotateX(real angle);

	/**
		Rotates a matrix around the y-axis.
		@param angle The angle of the rotation.
	*/
	void RotateY(real angle);

	/**
		Rotates a matrix around the z-axis.
		@param angle The angle of the rotation.
	*/
	void RotateZ(real angle);

	/**
		Scales the matrix along the x-axis, y-axis, and z-axis.
		@param x The scale along the x-axis.
		@param y The scale along the y-axis.
		@param z The scale along the z-axis.
		@remarks
			_00*x _01*y _02*z
			_10*x _11*y _12*z
			_20*x _21*y _22*z
	*/
	void Scale(real x, real y, real z);

	/**
		Scales the matrix along the x-axis, y-axis, and z-axis.
		@param value The scale vector.
		@remarks
			_00*x _01*y _02*z
			_10*x _11*y _12*z
			_20*x _21*y _22*z
	*/
	void Scale(const Vector3& value);

	/**
		Builds a matrix that scales along the x-axis, y-axis, and z-axis.
		@param x The scale along the x-axis.
		@param y The scale along the y-axis.
		@param z The scale along the z-axis.
		@return The scale matrix.
		@remarks
			x 0 0
			0 y 0
			0 0 z
	*/
	static Matrix3 CreateScale(real x, real y, real z);

	/**
		Builds a matrix that scales along the x-axis, y-axis, and z-axis.
		@param value The scale vector.
		@return The scale matrix.
		@remarks
			x 0 0
			0 y 0
			0 0 z
	*/
	static Matrix3 CreateScale(const Vector3& value);

	/**
		Builds a matrix that rotates around the x-axis.
		@param angle The angle of the rotation.
		@return The rotation matrix.
		@remarks
			1  0    0
			0 cos -sin
			0 sin  cos
	*/
	static Matrix3 CreateRotationX(real angle);

	/**
		Builds a matrix that rotates around the y-axis.
		@param angle The angle of the rotation.
		@return The rotation matrix.
		@remarks
			 cos 0 sin
			  0  1  0
			-sin 0 cos
	*/
	static Matrix3 CreateRotationY(real angle);

	/**
		Builds a matrix that rotates around the z-axis.
		@param angle The angle of the rotation.
		@return The rotation matrix.
		@remarks
			cos -sin 0
			sin  cos 0
			 0    0  1
	*/
	static Matrix3 CreateRotationZ(real angle);

	/**
		Builds a matrix with a specified yaw, pitch, and roll.
		@param yaw The angle of the rotation around the x-axis.
		@param pitch The angle of the rotation around the y-axis.
		@param roll The angle of the rotation around the z-axis.
		@return The rotation matrix.
	*/
	static Matrix3 CreateFromYawPitchRoll(real yaw, real pitch, real roll);

	/**
		Builds a matrix that rotates around an arbitrary axis.
		@param axis The axis of the rotation.
		@param angle The angle of the rotation.
		@return The rotation matrix.
	*/
	static Matrix3 CreateFromAxisAngle(const Vector3& axis, real angle);

	/**
		Builds a matrix from a quaternion.
		@param value The quaternion.
		@return The rotation matrix.
	*/
	static Matrix3 CreateFromQuaternion(const Quaternion& value);

	/**
		Builds a matrix from Matrix4.
		@param value The Matrix4.
		@return The rotation matrix.
	*/
	static Matrix3 CreateFromMatrix4(const Matrix4& value);
	//@}
};

#include "Matrix3.inl"

}

#endif
