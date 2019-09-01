/*=============================================================================
BoundingSphere.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "BoundingSphere.h"
#include "Core/Exception/ArgumentNullException.h"
#include "Core/Math/BoundingBox.h"
#include "Core/Math/Frustum.h"
#include "Core/Math/Ray3.h"

namespace SonataEngine
{

const BoundingSphere BoundingSphere::Empty = BoundingSphere(Vector3::Zero, (real)0.0);

BoundingSphere BoundingSphere::CreateMerged(const BoundingSphere& original, const BoundingSphere& additional)
{
	Vector3 diff = additional.Center - original.Center;
	real length = diff.Length();
	real originalRadius = original.Radius;
	real additionalRadius = additional.Radius;
	if ((originalRadius + additionalRadius) >= length)
	{
		if ((originalRadius - additionalRadius) >= length)
		{
			return original;
		}
		if ((additionalRadius - originalRadius) >= length)
		{
			return additional;
		}
	}

	BoundingSphere result;
	Vector3 vector = diff * ((real)1.0 / length);
	real min = Math::Min(-originalRadius, length - additionalRadius);
	real max = Math::Max(originalRadius, length + additionalRadius);
	real radius = (max - min) * (real)0.5;
	result.Center = original.Center + ((Vector3) (vector * (radius + min)));
	result.Radius = radius;
	return result;
}

BoundingSphere BoundingSphere::CreateFromPoints(Vector3* points, int count)
{
    if (points == NULL)
    {
        SEthrow(ArgumentNullException("points"));
		return BoundingSphere();
    }
    if (count == 0)
    {
		SEthrow(ArgumentException("count"));
		return BoundingSphere();
    }

	BoundingSphere result;
	real radius;
	Vector3 center;
	Vector3 point0;
	Vector3 point1;
	Vector3 point2;
	Vector3 point3;
	Vector3 point4;
	Vector3 point5;
	real dist01;
	real dist23;
	real dist45;
	Vector3 current;

	point0 = point1 = point2 = point3 = point4 = point5 = points[0];
	for (int i = 0; i < count; ++i)
	{
		current = points[i];
		if (current.X < point0.X)
		{
			point0 = current;
		}
		if (current.X > point1.X)
		{
			point1 = current;
		}
		if (current.Y < point2.Y)
		{
			point2 = current;
		}
		if (current.Y > point3.Y)
		{
			point3 = current;
		}
		if (current.Z < point4.Z)
		{
			point4 = current;
		}
		if (current.Z > point5.Z)
		{
			point5 = current;
		}
	}

	dist01 = Vector3::Distance(point1, point0);
	dist23 = Vector3::Distance(point3, point2);
	dist45 = Vector3::Distance(point5, point4);
	if (dist01 > dist23)
	{
		if (dist01 > dist45)
		{
			center = Vector3::Lerp(point1, point0, (real)0.5);
			radius = dist01 * (real)0.5;
		}
		else
		{
			center = Vector3::Lerp(point5, point4, (real)0.5);
			radius = dist45 * (real)0.5;
		}
	}
	else if (dist23 > dist45)
	{
		center = Vector3::Lerp(point3, point2, (real)0.5);
		radius = dist23 * (real)0.5;
	}
	else
	{
		center = Vector3::Lerp(point5, point4, (real)0.5);
		radius = dist45 * (real)0.5;
	}

	for (int j = 0; j < count; ++j)
	{
		current = points[j];
		Vector3 toCenter = current - center;
		toCenter.X = current.X - center.X;
		toCenter.Y = current.Y - center.Y;
		toCenter.Z = current.Z - center.Z;
		real length = toCenter.Length();
		if (length > radius)
		{
			radius = (radius + length) * (real)0.5;
			center += (Vector3) (((real)1.0 - (radius / length)) * toCenter);
		}
	}

	result.Center = center;
	result.Radius = radius;
	return result;
}

BoundingSphere BoundingSphere::CreateFromBox(const BoundingBox& box)
{
    BoundingSphere result;
	result.Center = Vector3::Lerp(box.Min, box.Max, (real)0.5);
	real length = Vector3::Distance(box.Min, box.Max);
	result.Radius = length * (real)0.5;
    return result;
}

BoundingSphere BoundingSphere::CreateFromFrustum(const Frustum& frustum)
{
	return BoundingSphere::CreateFromPoints(frustum.GetCorners(), 8);
}

void BoundingSphere::Transform(const Matrix4& transform)
{
	Center = Vector3::Transform(Center, transform);
}

bool BoundingSphere::Contains(const Vector3& point) const
{
	real distanceSq = Vector3::DistanceSquared(point, Center);
	if (distanceSq >= (Radius * Radius))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool BoundingSphere::Contains(const BoundingSphere& sphere) const
{
	real distance = Vector3::Distance(Center, sphere.Center);
	if ((Radius + sphere.Radius) < distance)
	{
		return false;
	}
	else if ((Radius - sphere.Radius) < distance)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool BoundingSphere::Contains(const BoundingBox& box) const
{
	if (!box.Intersects(*this))
	{
		return false;
	}

	Vector3 diff;
	real radiusSq = Radius * Radius;
	diff.X = Center.X - box.Min.X;
	diff.Y = Center.Y - box.Max.Y;
	diff.Z = Center.Z - box.Max.Z;
	if (diff.LengthSquared() > radiusSq)
	{
		return false;
	}
	diff.X = Center.X - box.Max.X;
	diff.Y = Center.Y - box.Max.Y;
	diff.Z = Center.Z - box.Max.Z;
	if (diff.LengthSquared() > radiusSq)
	{
		return false;
	}
	diff.X = Center.X - box.Max.X;
	diff.Y = Center.Y - box.Min.Y;
	diff.Z = Center.Z - box.Max.Z;
	if (diff.LengthSquared() > radiusSq)
	{
		return false;
	}
	diff.X = Center.X - box.Min.X;
	diff.Y = Center.Y - box.Min.Y;
	diff.Z = Center.Z - box.Max.Z;
	if (diff.LengthSquared() > radiusSq)
	{
		return false;
	}
	diff.X = Center.X - box.Min.X;
	diff.Y = Center.Y - box.Max.Y;
	diff.Z = Center.Z - box.Min.Z;
	if (diff.LengthSquared() > radiusSq)
	{
		return false;
	}
	diff.X = Center.X - box.Max.X;
	diff.Y = Center.Y - box.Max.Y;
	diff.Z = Center.Z - box.Min.Z;
	if (diff.LengthSquared() > radiusSq)
	{
		return false;
	}
	diff.X = Center.X - box.Max.X;
	diff.Y = Center.Y - box.Min.Y;
	diff.Z = Center.Z - box.Min.Z;
	if (diff.LengthSquared() > radiusSq)
	{
		return false;
	}
	diff.X = Center.X - box.Min.X;
	diff.Y = Center.Y - box.Min.Y;
	diff.Z = Center.Z - box.Min.Z;
	if (diff.LengthSquared() > radiusSq)
	{
		return false;
	}
	return true;
}

bool BoundingSphere::Intersects(const BoundingSphere& sphere) const
{
	real distance = Vector3::Distance(Center, sphere.Center);
	if (((Radius * Radius) + ((real)2.0 * Radius * sphere.Radius) + (sphere.Radius * sphere.Radius)) <= distance)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool BoundingSphere::Intersects(const BoundingBox& box) const
{
	Vector3 center = Vector3::Clamp(Center, box.Min, box.Max);
	real distanceSq = Vector3::DistanceSquared(Center, center);
	if (distanceSq <= (Radius * Radius))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool BoundingSphere::Intersects(const Ray3& ray, real& result) const
{
    return ray.Intersects(*this, result);
}

PlaneIntersectionType BoundingSphere::Intersects(const Plane& plane) const
{
	return plane.Intersects(*this);
}

}
