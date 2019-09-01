/*=============================================================================
Vector4.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Vector4.h"
#include "Core/Math/Matrix4.h"

namespace SonataEngine
{

const Vector4 Vector4::Zero((real)0.0, (real)0.0, (real)0.0, (real)0.0);
const Vector4 Vector4::One((real)1.0, (real)1.0, (real)1.0, (real)1.0);
const Vector4 Vector4::UnitX((real)1.0, (real)0.0, (real)0.0, (real)0.0);
const Vector4 Vector4::UnitY((real)0.0, (real)1.0, (real)0.0, (real)0.0);
const Vector4 Vector4::UnitZ((real)0.0, (real)0.0, (real)1.0, (real)0.0);
const Vector4 Vector4::UnitW((real)0.0, (real)0.0, (real)0.0, (real)1.0);

Vector4 Vector4::Transform(const Vector2& vector, const Matrix4& matrix)
{
	Vector4 result;
	result.X = (matrix.M00 * vector.X + matrix.M01 * vector.Y + matrix.M03);
	result.Y = (matrix.M10 * vector.X + matrix.M11 * vector.Y + matrix.M13);
	result.Z = (matrix.M20 * vector.X + matrix.M21 * vector.Y + matrix.M23);
	result.W = (matrix.M30 * vector.X + matrix.M31 * vector.Y + matrix.M33);
	return result;
}

Vector4 Vector4::Transform(const Vector3& vector, const Matrix4& matrix)
{
	Vector4 result;
	result.X = (matrix.M00 * vector.X + matrix.M01 * vector.Y + matrix.M02 * vector.Z + matrix.M03);
	result.Y = (matrix.M10 * vector.X + matrix.M11 * vector.Y + matrix.M12 * vector.Z + matrix.M13);
	result.Z = (matrix.M20 * vector.X + matrix.M21 * vector.Y + matrix.M22 * vector.Z + matrix.M23);
	result.W = (matrix.M30 * vector.X + matrix.M31 * vector.Y + matrix.M32 * vector.Z + matrix.M33);
	return result;
}

Vector4 Vector4::Transform(const Vector4& vector, const Matrix4& matrix)
{
	Vector4 result;
	result.X = (matrix.M00 * vector.X + matrix.M01 * vector.Y + matrix.M02 * vector.Z + matrix.M03 * vector.W);
	result.Y = (matrix.M10 * vector.X + matrix.M11 * vector.Y + matrix.M12 * vector.Z + matrix.M13 * vector.W);
	result.Z = (matrix.M20 * vector.X + matrix.M21 * vector.Y + matrix.M22 * vector.Z + matrix.M23 * vector.W);
	result.W = (matrix.M30 * vector.X + matrix.M31 * vector.Y + matrix.M32 * vector.Z + matrix.M33 * vector.W);
	return result;
}

}
