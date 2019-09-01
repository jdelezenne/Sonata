/*=============================================================================
Quaternion.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Quaternion.h"
#include "Matrix3.h"

namespace SonataEngine
{

const Quaternion Quaternion::Zero((real)0.0, (real)0.0, (real)0.0, (real)0.0);
const Quaternion Quaternion::Identity((real)0.0, (real)0.0, (real)0.0, (real)1.0);

void Quaternion::Normalize()
{
	real len, inv;

	len = Length();
	if (Math::Abs(len) < Math::Epsilon)
	{
		X = Y = Z = (real)0.0;
		W = (real)1.0; 
	}
	else
	{	
		inv = (real)1.0 / len;
		X *= inv;
		Y *= inv;
		Z *= inv;
		W *= inv;
	}
}

void Quaternion::Invert()
{
	real len, inv;

	len = Length();
	if (Math::Abs(len) < Math::Epsilon)
	{
		X = Y = Z = (real)0.0;
		W = (real)1.0;
	}
	else
	{
		inv = (real)1.0 / len;
		X *= -inv;
		Y *= -inv;
		Z *= -inv;
		W *= inv;
	}
}

void Quaternion::Log()
{
	real om, len, c;

	len = Math::Sqrt(X * X + Y * Y + Z * Z);
	om = Math::Atan2(len, W);
	if (Math::Abs(len) < Math::Epsilon)
	{
		c = (real)0.0;
	}
	else
	{
		c = om / len;
	}
	X = X*c;
	Y = Y*c;
	Z = Z*c;
	W = 0.0;
}

void Quaternion::Exp()
{
	real len, sinom;

	len = Math::Sqrt(X * X + Y * Y + Z * Z);
	if (Math::Abs(len) < Math::Epsilon)
	{
		sinom = (real)1.0;
	}
	else
	{
		sinom = Math::Sin(len) / len;
	}
	X *= sinom;
	Y *= sinom;
	Z *= sinom;
	W = Math::Cos(len);
}

Quaternion Quaternion::Lerp(const Quaternion& left, const Quaternion& right, real amount)
{
	Quaternion result;
	real remaining = (real)1.0 - amount;
	real dot = Quaternion::Dot(left, right);
	if (dot >= (real)0.0)
	{
		result.X = (remaining * left.X) + (amount * right.X);
		result.Y = (remaining * left.Y) + (amount * right.Y);
		result.Z = (remaining * left.Z) + (amount * right.Z);
		result.W = (remaining * left.W) + (amount * right.W);
	}
	else
	{
		result.X = (remaining * left.X) - (amount * right.X);
		result.Y = (remaining * left.Y) - (amount * right.Y);
		result.Z = (remaining * left.Z) - (amount * right.Z);
		result.W = (remaining * left.W) - (amount * right.W);
	}

	real invLength = result.Length();
	result.X *= invLength;
	result.Y *= invLength;
	result.Z *= invLength;
	result.W *= invLength;
	return result;
}

Quaternion Quaternion::Slerp(const Quaternion& left, const Quaternion& right, real amount)
{
	Quaternion result;
	real cosom, omega, sinom;
	real scale0, scale1;

	cosom = Quaternion::Dot(left, right);
	if (((real)1.0 + cosom) > Math::Epsilon)
	{
		if (Math::Abs(cosom) > (real)1.0)
		{
			cosom /= Math::Abs(cosom);
		}
		omega = Math::Acos(cosom);
		sinom = Math::Sin(omega);
		if (Math::Abs(sinom) > Math::Epsilon)
		{
			scale0 = Math::Sin(((real)1.0 - amount) * omega) / sinom;
			scale1 = Math::Sin(amount * omega) / sinom;
		}
		else
		{
			scale0 = (real)1.0 - amount;
			scale1 = amount;
		}
		result.X = scale0 * left.X + scale1 * right.X;
		result.Y = scale0 * left.Y + scale1 * right.Y;
		result.Z = scale0 * left.Z + scale1 * right.Z;
		result.W = scale0 * left.W + scale1 * right.W;
	}
	else
	{
		result.X = -left.Y;
		result.Y = left.X;
		result.Z = -left.W;
		result.W = left.Z;
		scale0 = Math::Sin(((real)1.0 - amount) * Math::InvPi);
		scale1 = Math::Sin(amount * Math::InvPi);
		result.X = scale0 * left.X + scale1 * result.X;
		result.Y = scale0 * left.Y + scale1 * result.Y;
		result.Z = scale0 * left.Z + scale1 * result.Z;
		result.W = scale0 * left.W + scale1 * result.W;
	}

	return result;
}

Quaternion Quaternion::Squad(const Quaternion& q1, const Quaternion& tangent1, const Quaternion& tangent2, const Quaternion& q2, real amount)
{
	real slerpT = (real)2.0 * amount * ((real)1.0 - amount);
	Quaternion slerpP = Quaternion::Slerp(q1, q2, amount);
	Quaternion slerpQ = Quaternion::Slerp(tangent1, tangent2, amount);
	return Quaternion::Slerp(slerpP, slerpQ, slerpT);
}

Quaternion Quaternion::CreateFromAxisAngle(const Vector3& axis, real angle)
{
	Quaternion result;
	real halfAngle = angle * (real)0.5;
	real halfSin = Math::Sin(halfAngle);
	real halfCos = Math::Cos(halfAngle);
	result.X = axis.X * halfSin;
	result.Y = axis.Y * halfSin;
	result.Z = axis.Z * halfSin;
	result.W = halfCos;
	return result;
}

Quaternion Quaternion::CreateFromRotationMatrix(const Matrix3& value)
{
	Quaternion result;
	real trace = value.M[0][0] + value.M[1][1] + value.M[2][2];
	real root;

	if (trace > (real)0.0)
	{
		// |w| > 1/2, may as well choose w > 1/2
		root = Math::Sqrt(trace + (real)1.0);  // 2w
		result.W = (real)0.5 * root;
		root = (real)0.5 / root; // 1/(4w)
		result.X = (value.M[2][1] - value.M[1][2]) * root;
		result.Y = (value.M[0][2] - value.M[2][0]) * root;
		result.Z = (value.M[1][0] - value.M[0][1]) * root;
	}
	else
	{
		// |w| <= 1/2
		static int next[3] = { 1, 2, 0 };
		int i = 0;
		if (value.M[1][1] > value.M[0][0])
			i = 1;
		if (value.M[2][2] > value.M[i][i])
			i = 2;
		int j = next[i];
		int k = next[j];

		root = Math::Sqrt(value.M[i][i] - value.M[j][j] - value.M[k][k] + (real)1.0);
		real* temp[3] = { &result.X, &result.Y, &result.Z };
		*temp[i] = ((real)0.5) * root;
		root = ((real)0.5) / root;
		result.W = (value.M[k][j] - value.M[j][k]) * root;
		*temp[j] = (value.M[j][i] + value.M[i][j]) * root;
		*temp[k] = (value.M[k][i] + value.M[i][k]) * root;
	}

	return result;
}

void Quaternion::ToAxisAngle(Vector3& axis, real& angle) const
{
	real lengthSq, invLen;

	lengthSq = (X * X + Y * Y + Z * Z);
	if (lengthSq < Math::Epsilon)
	{
		axis.X = (real)1.0;
		axis.Y = (real)0.0;
		axis.Z = (real)0.0;
		angle = (real)1.0;
	}
	else
	{
		angle = (real)2.0 * Math::Acos(W);
		invLen = (real)1.0 / Math::Sqrt(lengthSq);
		axis.X = X * invLen;
		axis.Y = Y * invLen;
		axis.Z = Z * invLen;
	}
}

void Quaternion::ToAxes(Vector3& xAxis, Vector3& yAxis, Vector3& zAxis) const
{
	Matrix3 matrix;
	ToRotationMatrix(matrix);

	xAxis.X = matrix.M[0][0];
	xAxis.Y = matrix.M[1][0];
	xAxis.Z = matrix.M[2][0];

	yAxis.X = matrix.M[0][1];
	yAxis.Y = matrix.M[1][1];
	yAxis.Z = matrix.M[2][1];

	zAxis.X = matrix.M[0][2];
	zAxis.Y = matrix.M[1][2];
	zAxis.Z = matrix.M[2][2];
}

void Quaternion::ToRotationMatrix(Matrix3& result) const
{
	real fTx  = (real)2.0 * X;
	real fTy  = (real)2.0 * Y;
	real fTz  = (real)2.0 * Z;
	real fTwx = fTx * W;
	real fTwy = fTy * W;
	real fTwz = fTz * W;
	real fTxx = fTx * X;
	real fTxy = fTy * X;
	real fTxz = fTz * X;
	real fTyy = fTy * Y;
	real fTyz = fTz * Y;
	real fTzz = fTz * Z;

	result.M[0][0] = (real)1.0 - (fTyy + fTzz);
	result.M[0][1] = fTxy - fTwz;
	result.M[0][2] = fTxz + fTwy;
	result.M[1][0] = fTxy + fTwz;
	result.M[1][1] = (real)1.0 - (fTxx + fTzz);
	result.M[1][2] = fTyz - fTwx;
	result.M[2][0] = fTxz - fTwy;
	result.M[2][1] = fTyz + fTwx;
	result.M[2][2] = (real)1.0 - (fTxx + fTyy);
}

}
