/*=============================================================================
Matrix4.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Matrix4.h"

namespace SonataEngine
{

const Matrix4 Matrix4::Zero(
	(real)0.0, (real)0.0, (real)0.0, (real)0.0,
	(real)0.0, (real)0.0, (real)0.0, (real)0.0,
	(real)0.0, (real)0.0, (real)0.0, (real)0.0,
	(real)0.0, (real)0.0, (real)0.0, (real)0.0);

const Matrix4 Matrix4::Identity(
	(real)1.0, (real)0.0, (real)0.0, (real)0.0,
	(real)0.0, (real)1.0, (real)0.0, (real)0.0,
	(real)0.0, (real)0.0, (real)1.0, (real)0.0,
	(real)0.0, (real)0.0, (real)0.0, (real)1.0);

real Matrix4::Determinant() const
{
	real mA0 = (M00 * M11) - (M01 * M10);
	real mA1 = (M00 * M12) - (M02 * M10);
	real mA2 = (M00 * M13) - (M03 * M10);
	real mA3 = (M01 * M12) - (M02 * M11);
	real mA4 = (M01 * M13) - (M03 * M11);
	real mA5 = (M02 * M13) - (M03 * M12);
	real mB0 = (M20 * M31) - (M21 * M30);
	real mB1 = (M20 * M32) - (M22 * M30);
	real mB2 = (M20 * M33) - (M23 * M30);
	real mB3 = (M21 * M32) - (M22 * M31);
	real mB4 = (M21 * M33) - (M23 * M31);
	real mB5 = (M22 * M33) - (M23 * M32);

	return (mA0*mB5 - mA1*mB4 + mA2*mB3 + mA3*mB2 - mA4*mB1 + mA5*mB0);
}

Matrix4 Matrix4::Invert(const Matrix4& value)
{
	Matrix4 result;

	real mA0 = value.Data[ 0]*value.Data[ 5] - value.Data[ 1]*value.Data[ 4];
	real mA1 = value.Data[ 0]*value.Data[ 6] - value.Data[ 2]*value.Data[ 4];
	real mA2 = value.Data[ 0]*value.Data[ 7] - value.Data[ 3]*value.Data[ 4];
	real mA3 = value.Data[ 1]*value.Data[ 6] - value.Data[ 2]*value.Data[ 5];
	real mA4 = value.Data[ 1]*value.Data[ 7] - value.Data[ 3]*value.Data[ 5];
	real mA5 = value.Data[ 2]*value.Data[ 7] - value.Data[ 3]*value.Data[ 6];
	real mB0 = value.Data[ 8]*value.Data[13] - value.Data[ 9]*value.Data[12];
	real mB1 = value.Data[ 8]*value.Data[14] - value.Data[10]*value.Data[12];
	real mB2 = value.Data[ 8]*value.Data[15] - value.Data[11]*value.Data[12];
	real mB3 = value.Data[ 9]*value.Data[14] - value.Data[10]*value.Data[13];
	real mB4 = value.Data[ 9]*value.Data[15] - value.Data[11]*value.Data[13];
	real mB5 = value.Data[10]*value.Data[15] - value.Data[11]*value.Data[14];

	result.M[0][0] = + value.Data[ 5]*mB5 - value.Data[ 6]*mB4 + value.Data[ 7]*mB3;
	result.M[1][0] = - value.Data[ 4]*mB5 + value.Data[ 6]*mB2 - value.Data[ 7]*mB1;
	result.M[2][0] = + value.Data[ 4]*mB4 - value.Data[ 5]*mB2 + value.Data[ 7]*mB0;
	result.M[3][0] = - value.Data[ 4]*mB3 + value.Data[ 5]*mB1 - value.Data[ 6]*mB0;
	result.M[0][1] = - value.Data[ 1]*mB5 + value.Data[ 2]*mB4 - value.Data[ 3]*mB3;
	result.M[1][1] = + value.Data[ 0]*mB5 - value.Data[ 2]*mB2 + value.Data[ 3]*mB1;
	result.M[2][1] = - value.Data[ 0]*mB4 + value.Data[ 1]*mB2 - value.Data[ 3]*mB0;
	result.M[3][1] = + value.Data[ 0]*mB3 - value.Data[ 1]*mB1 + value.Data[ 2]*mB0;
	result.M[0][2] = + value.Data[13]*mA5 - value.Data[14]*mA4 + value.Data[15]*mA3;
	result.M[1][2] = - value.Data[12]*mA5 + value.Data[14]*mA2 - value.Data[15]*mA1;
	result.M[2][2] = + value.Data[12]*mA4 - value.Data[13]*mA2 + value.Data[15]*mA0;
	result.M[3][2] = - value.Data[12]*mA3 + value.Data[13]*mA1 - value.Data[14]*mA0;
	result.M[0][3] = - value.Data[ 9]*mA5 + value.Data[10]*mA4 - value.Data[11]*mA3;
	result.M[1][3] = + value.Data[ 8]*mA5 - value.Data[10]*mA2 + value.Data[11]*mA1;
	result.M[2][3] = - value.Data[ 8]*mA4 + value.Data[ 9]*mA2 - value.Data[11]*mA0;
	result.M[3][3] = + value.Data[ 8]*mA3 - value.Data[ 9]*mA1 + value.Data[10]*mA0;

	real determinant = mA0*mB5-mA1*mB4+mA2*mB3+mA3*mB2-mA4*mB1+mA5*mB0;
	if (Math::Abs(determinant) <= Math::Epsilon)
	{
		return Matrix4::Zero;
	}

	real invDeterminant = (real)1.0 / determinant;
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			result.M[j][i] *= invDeterminant;
		}
	}

	return result;
}

Matrix4 Matrix4::CreateLookAt(const Vector3& position, const Vector3& target, const Vector3& upVector)
{
	Matrix4 result;
	Vector3 direction = Vector3::Normalize(position - target);
	Vector3 left = Vector3::Normalize(Vector3::Cross(upVector, direction));
	Vector3 up = Vector3::Cross(direction, left);
	result.M00 = left.X;
	result.M10 = up.X;
	result.M20 = direction.X;
	result.M30 = (real)0.0;
	result.M01 = left.Y;
	result.M11 = up.Y;
	result.M21 = direction.Y;
	result.M31 = (real)0.0;
	result.M02 = left.Z;
	result.M12 = up.Z;
	result.M22 = direction.Z;
	result.M32 = (real)0.0;
	result.M03 = -Vector3::Dot(left, position);
	result.M13 = -Vector3::Dot(up, position);
	result.M23 = -Vector3::Dot(direction, position);
	result.M33 = (real)1.0;
	return result;
}

Matrix4 Matrix4::CreatePerspectiveLH(real32 fov, real32 aspect, real32 znear, real32 zfar)
{
	Matrix4 result;
	real32 height = (real)1.0 / Math::Tan(Math::ToRadians(fov) * (real)0.5);
	real32 width = height / aspect;

	result.M00 = width;
	result.M01 = (real)0.0;
	result.M02 = (real)0.0;
	result.M03 = (real)0.0;

	result.M10 = (real)0.0;
	result.M11 = height;
	result.M12 = (real)0.0;
	result.M13 = (real)0.0;

	result.M20 = (real)0.0;
	result.M21 = (real)0.0;
	result.M22 = zfar / (zfar - znear);
	result.M23 = -znear * zfar / (zfar - znear);

	result.M30 = (real)0.0;
	result.M31 = (real)0.0;
	result.M32 = (real)1.0;
	result.M33 = (real)0.0;

	return result;
}

Matrix4 Matrix4::CreatePerspectiveRH(real32 fov, real32 aspect, real32 znear, real32 zfar)
{
	Matrix4 result;
	real32 height = (real)1.0 / Math::Tan(Math::ToRadians(fov) * (real)0.5);
	real32 width = (height / aspect);

	result.M00 = width;
	result.M01 = (real)0.0;
	result.M02 = (real)0.0;
	result.M03 = (real)0.0;

	result.M10 = (real)0.0;
	result.M11 = height;
	result.M12 = (real)0.0;
	result.M13 = (real)0.0;

	result.M20 = (real)0.0;
	result.M21 = (real)0.0;
	result.M22 = zfar / (znear - zfar);
	result.M23 = znear * zfar / (znear - zfar);

	result.M30 = (real)0.0;
	result.M31 = (real)0.0;
	result.M32 = (real)-1.0;
	result.M33 = (real)0.0;

	return result;
}

Matrix4 Matrix4::CreateOrthographicLH(real32 width, real32 height, real32 znear, real32 zfar)
{
	Matrix4 result;

	result.M00 = (real)2.0 / width;
	result.M01 = (real)0.0;
	result.M02 = (real)0.0;
	result.M03 = (real)0.0;

	result.M10 = (real)0.0;
	result.M11 = (real)2.0 / height;
	result.M12 = (real)0.0;
	result.M13 = (real)0.0;

	result.M20 = (real)0.0;
	result.M21 = (real)0.0;
	result.M22 = (real)1.0 / (zfar - znear);
	result.M23 = (real)0.0;

	result.M30 = (real)0.0;
	result.M31 = (real)0.0;
	result.M32 = znear / (zfar - znear);
	result.M33 = (real)1.0;

	return result;
}

Matrix4 Matrix4::CreateOrthographicRH(real32 width, real32 height, real32 znear, real32 zfar)
{
	Matrix4 result;

	result.M00 = (real)2.0 / width;
	result.M01 = (real)0.0;
	result.M02 = (real)0.0;
	result.M03 = (real)0.0;

	result.M10 = (real)0.0;
	result.M11 = (real)2.0 / -height;
	result.M12 = (real)0.0;
	result.M13 = (real)0.0;

	result.M20 = (real)0.0;
	result.M21 = (real)0.0;
	result.M22 = (real)2.0 / (znear - zfar);//1.0 / (znear - zfar);
	result.M23 = (znear + zfar) / (znear - zfar);//znear / (znear - zfar);

	result.M30 = (real)0.0;
	result.M31 = (real)0.0;
	result.M32 = (real)0.0;
	result.M33 = (real)1.0;

	return result;
}

Matrix4 Matrix4::CreateOrthographicOffCenterLH(real32 left, real32 right, real32 bottom, real32 top, real32 znear, real32 zfar)
{
	Matrix4 result;

	result.M00 = (real)2.0 / (right - left);
	result.M01 = (real)0.0;
	result.M02 = (real)0.0;
	result.M03 = (left + right) / (left - right);

	result.M10 = (real)0.0;
	result.M11 = (real)2.0 / (bottom - top);
	result.M12 = (real)0.0;
	result.M13 = (bottom + top) / (bottom - top);

	result.M20 = (real)0.0;
	result.M21 = (real)0.0;
	result.M22 = (real)1.0 / (zfar - znear);
	result.M23 = (real)0.0;

	result.M30 = (real)0.0;
	result.M31 = (real)0.0;
	result.M32 = znear / (zfar - znear);
	result.M33 = (real)1.0;

	return result;
}

Matrix4 Matrix4::CreateOrthographicOffCenterRH(real32 left, real32 right, real32 bottom, real32 top, real32 znear, real32 zfar)
{
	Matrix4 result;

	result.M00 = (real)2.0 / (right - left);
	result.M01 = (real)0.0;
	result.M02 = (real)0.0;
	result.M03 = (left + right) / (left - right);

	result.M10 = (real)0.0;
	result.M11 = (real)2.0 / -(bottom - top);
	result.M12 = (real)0.0;
	result.M13 = (bottom + top) / (bottom - top);

	result.M20 = (real)0.0;
	result.M21 = (real)0.0;
	result.M22 = (real)2.0 / (znear - zfar);//1.0 / (znear - zfar);
	result.M23 = (znear + zfar) / (znear - zfar);//znear / (znear - zfar);

	result.M30 = (real)0.0;
	result.M31 = (real)0.0;
	result.M32 = (real)0.0;
	result.M33 = (real)1.0;

	return result;
}

Matrix4 Matrix4::CreateViewport(int32 x, int32 y, int32 width, int32 height, real32 znear, real32 zfar)
{
	Matrix4 result;

	result.M00 = width * (real)0.5;
	result.M01 = (real)0.0;
	result.M02 = (real)0.0;
	result.M03 = (real)0.0;

	result.M10 = (real)0.0;
	result.M11 = height * -(real)0.5;
	result.M12 = (real)0.0;
	result.M13 = (real)0.0;

	result.M20 = (real)0.0;
	result.M21 = (real)0.0;
	result.M22 = zfar - znear;
	result.M23 = (real)0.0;

	result.M30 = (real)x + width * (real)0.5;
	result.M31 = (real)y + height * (real)0.5;
	result.M32 = znear;
	result.M33 = (real)1.0;

	return result;
}

}
