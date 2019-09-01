/*=============================================================================
Matrix4.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_MATRIX4_H_
#define _SE_MATRIX4_H_

#include "Core/Common.h"
#include "Core/Math/Math.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Vector4.h"
#include "Core/Math/Matrix3.h"
#include "Core/Math/Quaternion.h"

namespace SonataEngine
{

/**
	@brief Matrix 4x4.

	This matrix is used to represent a 3D transformation: the combination
	of a translation, a rotation and a scale.
*/
class SE_CORE_EXPORT Matrix4
{
public:
	static const Matrix4 Zero;
	static const Matrix4 Identity;

public:
	union
	{
		struct
		{
			real M00, M01, M02, M03;
			real M10, M11, M12, M13;
			real M20, M21, M22, M23;
			real M30, M31, M32, M33;
		};
		real M[4][4];
		real Data[16];
	};

public:
	/** @name Constructors. */
	//@{
	/** Default constructor. */
	Matrix4();
	
	/** Initialize a new matrix with initial values. */
	Matrix4(
		real m00, real m01, real m02, real m03,
		real m10, real m11, real m12, real m13,
		real m20, real m21, real m22, real m23,
		real m30, real m31, real m32, real m33);
	
	/** Initialize a new matrix with initial values. */
	Matrix4(const real value[16]);

	/**
		Initialize a new matrix from a rotation matrix and a translation vector.
		@remarks
			r.01 r.02 r.02 t.x
			r.11 r.12 r.12 t.y
			r.21 r.22 r.22 t.z
			0.0  0.0  0.0  1.0
	*/
	Matrix4(const Matrix3& rotation, const Vector3& translation);
	
	/** Copy constructor. */
	Matrix4(const Matrix4& value);
	//@}

	/** @name Casting Operators. */
	//@{
	/** Retrieves the matrix elements as an array of scalars. */
	operator real*();
	operator const real*() const;
	//@}

	/** @name Access Operators. */
	//@{
	/** Retrieves a given matrix element by its row and column indices. */
	real& operator()(int row, int col);
	real operator()(int row, int col) const;
	//@}

	/** @name Assignment Operators. */
	//@{
	/** Adds this instance to another matrix. */
	Matrix4& operator+=(const Matrix4& value);

	/** Substracts this instance by another matrix. */
	Matrix4& operator-=(const Matrix4& value);

	/** Multiplies this instance by another matrix. */
	Matrix4& operator*=(const Matrix4& value);

	/** Multiplies this instance by a scalar. */
	Matrix4& operator*=(real value);

	/** Divides this instance by a scalar. */
	Matrix4& operator/=(real value);
	//@}

	/** @name Unary Operators. */
	//@{
	Matrix4 operator+() const;
	Matrix4 operator-() const;
	//@}

	/** @name Binary Operators. */
	//@{
	/** Adds two matrices. */
	Matrix4 operator+(const Matrix4& value) const;

	/** Substracts two matrices. */
	Matrix4 operator-(const Matrix4& value) const;

	/** Multiplies two matrices. */
	Matrix4 operator*(const Matrix4& value) const;

	/** Multiplies a matrice by a scalar. */
	Matrix4 operator*(real value) const;

	/** Divides a matrice by a scalar. */
	Matrix4 operator/(real value) const;

	/** Compares this instance to another instance to determine whether they are the same. */
	bool operator==(const Matrix4& mat) const;

	/** Compares this instance to another instance to determine whether they are different. */
	bool operator!=(const Matrix4& mat) const;
	//@}

	/** @name Operations. */
	//@{
	/**
		Gets a row.
		@param row The index of row.
		@return The row.
	*/
	Vector4 GetRow(int row) const;

	/**
		Sets a row.
		@param row The index of row.
		@param value The row.
	*/
	void SetRow(int row, const Vector4& value);

	/**
		Gets a column.
		@param column The index of column.
		@return The column.
	*/
	Vector4 GetColumn(int column) const;

	/**
		Sets a column.
		@param column The index of column.
		@param value The column.
	*/
	void SetColumn(int column, const Vector4& value);

	/**
		Gets the translation.
		@return The translation.
	*/
	Vector3 GetTranslation() const;

	/**
		Sets the translation.
		@param value The translation.
	*/
	void SetTranslation(const Vector3& value);

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
	static Matrix4 Transpose(const Matrix4& value);

	/**
		Adds two matrices.
		@param value Source matrix.
		@remarks out = (real)this + value.
	*/
	void Add(const Matrix4& value);

	/**
		Adds two matrices.
		@param left Source matrix.
		@param right Source matrix.
		@remarks out = (real)left + right.
	*/
	static Matrix4 Add(const Matrix4& left, const Matrix4& right);

	/**
		Subtracts one matrix from another.
		@param value Source matrix.
		@remarks out = (real)this - value.
	*/
	void Substract(const Matrix4& value);

	/**
		Subtracts one matrix from another.
		@param left Source matrix.
		@param right Source matrix.
		@remarks out = (real)left - right.
	*/
	static Matrix4 Substract(const Matrix4& left, const Matrix4& right);

	/*
		Multiplies two matrices.
		@param value Source matrix.
		@remarks out = (real)this * value.
	*/
	void Multiply(const Matrix4& value);

	/**
		Multiplies two matrices.
		@param left Source matrix.
		@param right Source matrix.
		@return The result of the multiplication.
		@remarks out = (real)left * right.
	*/
	static Matrix4 Multiply(const Matrix4& left, const Matrix4& right);

	/**
		Matrix multiplication, followed by a transpose.
		@param value Source matrix.
		@remarks out = (real)T(this * value).
	*/
	void MultiplyTranspose(const Matrix4& value);

	/**
		Matrix multiplication, followed by a transpose.
		@param left Source matrix.
		@param right Source matrix.
		@return The result.
		@remarks out = (real)T(left * right).
	*/
	static Matrix4 MultiplyTranspose(const Matrix4& left, const Matrix4& right);

	/**
		Computes the inverse of the matrix.
	*/
	void Invert();

	/**
		Computes the inverse of a matrix.
		@patam value The matrix to inverse.
		@return The inverse of the matrix.
	*/
	static Matrix4 Invert(const Matrix4& value);
	//@}

	/** Transformations. */
	//@{
	/**
		Translates the matrix using specified offsets.
		@param x The x-coordinate offset.
		@param y The y-coordinate offset.
		@param z The z-coordinate offset.
		@remarks
			_00 _01 _02 _03+_00*x+_01*y+_02*z
			_10 _11 _12 _13+_10*x+_11*y+_12*z
			_20 _21 _22 _23+_20*x+_21*y+_22*z
			_30 _31 _32 _33
	*/
	void Translate(real x, real y, real z);

	/**
		Translates the matrix using specified offsets.
		@param value The offsets.
		@remarks
			_00 _01 _02 _03+_00*x+_01*y+_02*z
			_10 _11 _12 _13+_10*x+_11*y+_12*z
			_20 _21 _22 _23+_20*x+_21*y+_22*z
			_30 _31 _32 _33
	*/
	void Translate(const Vector3& value);

	/**
		Rotates a matrix around the x-axis.
		@param angle The angle of the rotation.
		@remarks
			_00 _01*cos+_02*sin _01*-sin+_02*cos _03
			_10 _11*cos+_12*sin _11*-sin+_12*cos _13
			_20 _21*cos+_22*sin _21*-sin+_22*cos _23
			_30 _31             _32              _33
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
			_00*x _01*y _02*z _03
			_10*x _11*y _12*z _13
			_20*x _21*y _22*z _23
			_30   _31   _32   _33
	*/
	void Scale(real x, real y, real z);

	/**
		Scales the matrix along the x-axis, y-axis, and z-axis.
		@param value The scale vector.
		@remarks
			_00*x _01*y _02*z _03
			_10*x _11*y _12*z _13
			_20*x _21*y _22*z _23
			_30   _31   _32   _33
	*/
	void Scale(const Vector3& value);

	/** Builds a matrix using specified offsets.
		@param x The x-coordinate offset.
		@param y The y-coordinate offset.
		@param z The z-coordinate offset.
		@return The translation matrix.
		@remarks
			1 0 0 x
			0 1 0 y
			0 0 1 z
			0 0 0 1
	*/
	static Matrix4 CreateTranslation(real x, real y, real z);

	/**
		Builds a matrix using specified offsets.
		@param value The offsets.
		@return The translation matrix.
		@remarks
			1 0 0 x
			0 1 0 y
			0 0 1 z
			0 0 0 1
	*/
	static Matrix4 CreateTranslation(const Vector3& value);

	/**
		Builds a matrix that rotates around the x-axis.
		@param angle The angle of the rotation.
		@return The rotation matrix.
		@remarks
			1  0    0  0
			0 cos -sin 0
			0 sin  cos 0
			0  0    0  1
	*/
	static Matrix4 CreateRotationX(real angle);

	/**
		Builds a matrix that rotates around the y-axis.
		@param angle The angle of the rotation.
		@return The rotation matrix.
		@remarks
			 cos 0 sin 0
			  0  1  0  0
			-sin 0 cos 0
			  0  0  0  1
	*/
	static Matrix4 CreateRotationY(real angle);

	/**
		Builds a matrix that rotates around the z-axis.
		@param angle The angle of the rotation.
		@return The rotation matrix.
		@remarks
			cos -sin 0 0
			sin  cos 0 0
			 0    0  1 0
			 0    0  0 1
	*/
	static Matrix4 CreateRotationZ(real angle);

	/**
		Builds a matrix that scales along the x-axis, y-axis, and z-axis.
		@param x The scale along the x-axis.
		@param y The scale along the y-axis.
		@param z The scale along the z-axis.
		@return The scale matrix.
		@remarks
			x 0 0 0
			0 y 0 0
			0 0 z 0
			0 0 0 1
	*/
	static Matrix4 CreateScale(real x, real y, real z);

	/**
		Builds a matrix that scales along the x-axis, y-axis, and z-axis.
		@param value The scale vector.
		@return The scale matrix.
		@remarks
			x 0 0 0
			0 y 0 0
			0 0 z 0
			0 0 0 1
	*/
	static Matrix4 CreateScale(const Vector3& value);

	static Matrix4 CreateLookAt(const Vector3& position, const Vector3& target, const Vector3& upVector);

	/**
		Builds a perspective projection matrix based on a field of view.
		@param fov The field-of-view.
		@param aspect The aspect ratio.
		@param znear The near plane.
		@param zfar The far plane.
		@remarks
			h = (real)1 / tan(fov / 2)
			w = (real)height / aspect

			2*zn/w  0       0              0
			0       2*zn/h  0              0
			0       0       zf/(zn-zf)    -1
			0       0       zn*zf/(zn-zf)  0
	*/
	static Matrix4 CreatePerspective(real32 fov, real32 aspect, real32 znear, real32 zfar);
	static Matrix4 CreatePerspectiveLH(real32 fov, real32 aspect, real32 znear, real32 zfar);
	static Matrix4 CreatePerspectiveRH(real32 fov, real32 aspect, real32 znear, real32 zfar);

	/**
		Builds an orthogonal projection matrix.
		@param width Width of the view volume.
		@param height Height of the view volume.
		@param znear Minimum z-value of the view volume.
		@param zfar Maximum z-value of the view volume.
		@remarks
			2/w  0    0           0
			0    2/h  0           0
			0    0    1/(zn-zf)   0
			0    0    zn/(zn-zf)  l
	*/
	static Matrix4 CreateOrthographic(real32 width, real32 height, real32 znear, real32 zfar);
	static Matrix4 CreateOrthographicLH(real32 width, real32 height, real32 znear, real32 zfar);
	static Matrix4 CreateOrthographicRH(real32 width, real32 height, real32 znear, real32 zfar);

	/**
		Builds an orthogonal projection matrix.
		@param left Left of the view volume.
		@param right Right of the view volume.
		@param bottom Bottom of the view volume.
		@param top Top of the view volume.
		@param znear Minimum z-value of the view volume.
		@param zfar Maximum z-value of the view volume.
		@remarks
			2/(r-l)      0            0           0
			0            2/(b-t)      0           0
			0            0            1/(zn-zf)   0
			(l+r)/(l-r)  (b+t)/(b-t)  zn/(zn-zf)  l
	*/
	static Matrix4 CreateOrthographicOffCenter(real32 left, real32 right, real32 bottom, real32 top, real32 znear, real32 zfar);
	static Matrix4 CreateOrthographicOffCenterLH(real32 left, real32 right, real32 bottom, real32 top, real32 znear, real32 zfar);
	static Matrix4 CreateOrthographicOffCenterRH(real32 left, real32 right, real32 bottom, real32 top, real32 znear, real32 zfar);

	/**
		Builds a viewport matrix.
		@param x X coordinate of the upper-left corner of the viewport.
		@param y Y coordinate of the upper-left corner of the viewport.
		@param width Width dimension of the clip volume.
		@param height Height dimension of the clip volume.
		@param znear Minimum z-value of the view volume.
		@param zfar Maximum z-value of the view volume.
		@remarks
			w/2    0      0      0
			0      -h/2   0      0
			0      0      zf-zn  0
			x+w/2  y+h/2  zn     l
	*/
	static Matrix4 CreateViewport(int32 x, int32 y, int32 width, int32 height, real32 znear, real32 zfar);

	/**
		Builds a matrix with a specified yaw, pitch, and roll.
		@param yaw The angle of the rotation around the x-axis.
		@param pitch The angle of the rotation around the y-axis.
		@param roll The angle of the rotation around the z-axis.
		@return The rotation matrix.
	*/
	static Matrix4 CreateFromYawPitchRoll(real yaw, real pitch, real roll);

	/**
		Builds a matrix that rotates around an arbitrary axis.
		@param axis The axis of the rotation.
		@param angle The angle of the rotation.
		@return The rotation matrix.
	*/
	static Matrix4 CreateFromAxisAngle(const Vector3& axis, real angle);

	/**
		Builds a matrix from a quaternion.
		@param value The quaternion.
		@return The rotation matrix.
	*/
	static Matrix4 CreateFromQuaternion(const Quaternion& value);
	//@}
};

#include "Matrix4.inl"

}

#endif
