/*=============================================================================
Vector3.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Vector3.h"
#include "Core/Math/Matrix4.h"

namespace SonataEngine
{

const Vector3 Vector3::Zero((real)0.0, (real)0.0, (real)0.0);
const Vector3 Vector3::One((real)1.0, (real)1.0, (real)1.0);
const Vector3 Vector3::UnitX((real)1.0, (real)0.0, (real)0.0);
const Vector3 Vector3::UnitY((real)0.0, (real)1.0, (real)0.0);
const Vector3 Vector3::UnitZ((real)0.0, (real)0.0, (real)1.0);
const Vector3 Vector3::Up((real)0.0, (real)1.0, (real)0.0);
const Vector3 Vector3::Down((real)0.0, (real)-1.0, (real)0.0);
const Vector3 Vector3::Left((real)-1.0, (real)0.0, (real)0.0);
const Vector3 Vector3::Right((real)1.0, (real)0.0, (real)0.0);
const Vector3 Vector3::Forward((real)0.0, (real)0.0, (real)-1.0);
const Vector3 Vector3::Backward((real)0.0, (real)0.0, (real)1.0);

Vector3 Vector3::Transform(const Vector3& position, const Matrix4& matrix)
{
	Vector3 result;
	result.X = (matrix.M00 * position.X + matrix.M01 * position.Y + matrix.M02 * position.Z + matrix.M03);
	result.Y = (matrix.M10 * position.X + matrix.M11 * position.Y + matrix.M12 * position.Z + matrix.M13);
	result.Z = (matrix.M20 * position.X + matrix.M21 * position.Y + matrix.M22 * position.Z + matrix.M23);
	return result;
}

Vector3 Vector3::TransformNormal(const Vector3& normal, const Matrix4& matrix)
{
	Vector3 result;
	result.X = (matrix.M00 * normal.X + matrix.M01 * normal.Y + matrix.M02 * normal.Z);
	result.Y = (matrix.M10 * normal.X + matrix.M11 * normal.Y + matrix.M12 * normal.Z);
	result.Z = (matrix.M20 * normal.X + matrix.M21 * normal.Y + matrix.M22 * normal.Z);
	return result;
}

Vector3 Vector3::Project(const Vector3& vector, const Matrix4& matrix)
{
	Vector3 result;
	real invW = (real)1.0 / (matrix.M30 * vector.X + matrix.M31 * vector.Y + matrix.M32 * vector.Z + matrix.M33);
	result.X = (matrix.M00 * vector.X + matrix.M01 * vector.Y + matrix.M02 * vector.Z + matrix.M03) * invW;
	result.Y = (matrix.M10 * vector.X + matrix.M11 * vector.Y + matrix.M12 * vector.Z + matrix.M13) * invW;
	result.Z = (matrix.M20 * vector.X + matrix.M21 * vector.Y + matrix.M22 * vector.Z + matrix.M23) * invW;
	return result;
}

Vector3 Vector3::Refract(const Vector3& value, const Vector3& normal, real incident, real transmitted)
{
	Vector3 v = Vector3::Normalize(value);
	Vector3 n = Vector3::Normalize(normal);
	real eta;

	if (Vector3::Dot(v, n) > (real)0.0)
	{
		n = -n;
		eta = transmitted / incident;
	}
	else
	{
		eta = incident / transmitted;
	}

	real VdotN = Vector3::Dot(-v, n);

	real k = (real)1.0 - eta * eta * ((real)1.0 - VdotN * VdotN);
	if (k < (real)0.0)
	{
		// Total internal refraction
		return Vector3::Zero;
	}

	Vector3 refracted;
	//refracted = eta * (v + VdotN * n) - Math::Sqrt(k) * n;
	refracted = eta * -v + (eta * VdotN - Math::Sqrt(k)) * n;
	refracted.Normalize();

	return refracted;
}

}
