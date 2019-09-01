/*=============================================================================
Vector2.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Vector2.h"
#include "Core/Math/Matrix4.h"

namespace SonataEngine
{

const Vector2 Vector2::Zero((real)0.0, (real)0.0);
const Vector2 Vector2::One((real)1.0, (real)1.0);
const Vector2 Vector2::UnitX((real)1.0, (real)0.0);
const Vector2 Vector2::UnitY((real)0.0, (real)1.0);

Vector2 Vector2::Transform(const Vector2& position, const Matrix4& matrix)
{
	Vector2 result;
	result.X = (matrix.M00 * position.X + matrix.M01 * position.Y + matrix.M02 + matrix.M03);
	result.Y = (matrix.M10 * position.X + matrix.M11 * position.Y + matrix.M12 + matrix.M13);
	return result;
}

Vector2 Vector2::TransformNormal(const Vector2& normal, const Matrix4& matrix)
{
	Vector2 result;
	result.X = (matrix.M00 * normal.X + matrix.M01 * normal.Y);
	result.Y = (matrix.M10 * normal.X + matrix.M11 * normal.Y);
	return result;
}

Vector2 Vector2::Project(const Vector2& vector, const Matrix4& matrix)
{
	Vector2 result;
	real invW = (real)1.0 / (matrix.M30 * vector.X + matrix.M31 * vector.Y + matrix.M33);
	result.X = (matrix.M00 * vector.X + matrix.M01 * vector.Y + matrix.M03) * invW;
	result.Y = (matrix.M10 * vector.X + matrix.M11 * vector.Y + matrix.M13) * invW;
	return result;
}


Vector2 Vector2::Refract(const Vector2& value, const Vector2& normal, real incident, real transmitted)
{
	Vector2 v = Vector2::Normalize(value);
	Vector2 n = Vector2::Normalize(normal);
	real eta;

	if (Vector2::Dot(v, n) > (real)0.0)
	{
		n = -n;
		eta = transmitted / incident;
	}
	else
	{
		eta = incident / transmitted;
	}

	real VdotN = Vector2::Dot(-v, n);

	real k = (real)1.0 - eta * eta * ((real)1.0 - VdotN * VdotN);
	if (k < (real)0.0)
	{
		// Total internal refraction
		return Vector2::Zero;
	}

	Vector2 refracted;
	//refracted = eta * (v + VdotN * n) - Math::Sqrt(k) * n;
	refracted = eta * -v + (eta * VdotN - Math::Sqrt(k)) * n;
	refracted.Normalize();

	return refracted;
}

}
