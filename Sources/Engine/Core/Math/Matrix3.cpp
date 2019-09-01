/*=============================================================================
Matrix3.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Matrix3.h"
#include "Core/Math/Matrix4.h"

namespace SonataEngine
{

const Matrix3 Matrix3::Zero(
	(real)0.0, (real)0.0, (real)0.0,
	(real)0.0, (real)0.0, (real)0.0,
	(real)0.0, (real)0.0, (real)0.0);

const Matrix3 Matrix3::Identity(
	(real)1.0, (real)0.0, (real)0.0,
	(real)0.0, (real)1.0, (real)0.0,
	(real)0.0, (real)0.0, (real)1.0);

real Matrix3::Determinant() const
{
	real m00 = Data[4]*Data[8] - Data[5]*Data[7];
	real m10 = Data[5]*Data[6] - Data[3]*Data[8];
	real m20 = Data[3]*Data[7] - Data[4]*Data[6];

	return (Data[0]*m00 + Data[1]*m10 + Data[2]*m20);
}

Matrix3 Matrix3::Invert(const Matrix3& value)
{
	Matrix3 result;

	result.M[0][0] = value.Data[4]*value.Data[8] - value.Data[5]*value.Data[7];
	result.M[0][1] = value.Data[2]*value.Data[7] - value.Data[1]*value.Data[8];
	result.M[0][2] = value.Data[1]*value.Data[5] - value.Data[2]*value.Data[4];
	result.M[1][0] = value.Data[5]*value.Data[6] - value.Data[3]*value.Data[8];
	result.M[1][1] = value.Data[0]*value.Data[8] - value.Data[2]*value.Data[6];
	result.M[1][2] = value.Data[2]*value.Data[3] - value.Data[0]*value.Data[5];
	result.M[2][0] = value.Data[3]*value.Data[7] - value.Data[4]*value.Data[6];
	result.M[2][1] = value.Data[1]*value.Data[6] - value.Data[0]*value.Data[7];
	result.M[2][2] = value.Data[0]*value.Data[4] - value.Data[1]*value.Data[3];

	real determinant = (value.Data[0]*result.M[0][0] + value.Data[1]*result.M[1][0]+ value.Data[2]*result.M[2][0]);
	if (Math::Abs(determinant) <= Math::Epsilon)
	{
		return Matrix3::Zero;
	}
	else
	{
		result /= determinant;
		return result;
	}
}

Matrix3 Matrix3::CreateFromMatrix4(const Matrix4& value)
{
	return Matrix3(
		value.M00, value.M01, value.M02,
		value.M10, value.M11, value.M12,
		value.M20, value.M21, value.M22);
}

}
