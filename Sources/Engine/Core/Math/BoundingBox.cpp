/*=============================================================================
BoundingBox.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "BoundingBox.h"
#include "Core/Exception/ArgumentNullException.h"
#include "Core/Math/BoundingSphere.h"
#include "Core/Math/Frustum.h"
#include "Core/Math/Ray3.h"

namespace SonataEngine
{

const BoundingBox BoundingBox::Empty = BoundingBox(Vector3::Zero, Vector3::Zero);

void BoundingBox::GetCorners(Vector3* corners) const
{
	if (corners == NULL)
	{
		SEthrow(ArgumentNullException("corners"));
		return;
	}

	corners[0] = Min;
	corners[1].X = Min.X; corners[1].Y = Max.Y; corners[1].Z = Min.Z;
	corners[2].X = Max.X; corners[2].Y = Max.Y; corners[2].Z = Min.Z;
	corners[3].X = Max.X; corners[3].Y = Min.Y; corners[3].Z = Min.Z;            

	corners[4] = Max;
	corners[5].X = Min.X; corners[5].Y = Max.Y; corners[5].Z = Max.Z;
	corners[6].X = Min.X; corners[6].Y = Min.Y; corners[6].Z = Max.Z;
	corners[7].X = Max.X; corners[7].Y = Min.Y; corners[7].Z = Max.Z;            
}

BoundingBox BoundingBox::CreateMerged(const BoundingBox& original, const BoundingBox& additional)
{
	Vector3 min = Vector3::Min(original.Min, additional.Min);
	Vector3 max = Vector3::Max(original.Max, additional.Max);
    return BoundingBox(min, max);
}

BoundingBox BoundingBox::CreateFromPoints(Vector3* points, int count)
{
    if (points == NULL)
    {
        SEthrow(ArgumentNullException("points"));
		return BoundingBox();
    }
    if (count == 0)
    {
		SEthrow(ArgumentException("count"));
		return BoundingBox();
    }

    bool hasPoints = false;
	Vector3 min = Vector3(SE_MAX_R32);
    Vector3 max = Vector3(SE_MIN_R32);
	Vector3 pt;
	for (int i = 0; i < count; ++i)
    {
		pt = points[i];
		min = Vector3::Min(min, pt);
        max = Vector3::Max(max, pt);
        hasPoints = true;
    }
    if (!hasPoints)
    {
        SEthrow(ArgumentException("BoundingBoxZeroPoints"));
		return BoundingBox();
    }
    return BoundingBox(min, max);
}

BoundingBox BoundingBox::CreateFromSphere(const BoundingSphere& sphere)
{
    BoundingBox result;
    result.Min.X = sphere.Center.X - sphere.Radius;
    result.Min.Y = sphere.Center.Y - sphere.Radius;
    result.Min.Z = sphere.Center.Z - sphere.Radius;
    result.Max.X = sphere.Center.X + sphere.Radius;
    result.Max.Y = sphere.Center.Y + sphere.Radius;
    result.Max.Z = sphere.Center.Z + sphere.Radius;
    return result;
}

void BoundingBox::Transform(const Matrix4& transform)
{
	Vector3 min, max, temp;
	bool first = true;
	min =  max = Vector3::Zero;

	Vector3 corners[8];
	GetCorners(corners);

	for (int i = 0; i < 8; ++i)
	{
		temp = Vector3::Transform(corners[i], transform);
		if (first || temp.X > max.X)
			max.X = temp.X;
		if (first || temp.Y > max.Y)
			max.Y = temp.Y;
		if (first || temp.Z > max.Z)
			max.Z = temp.Z;
		if (first || temp.X < min.X)
			min.X = temp.X;
		if (first || temp.Y < min.Y)
			min.Y = temp.Y;
		if (first || temp.Z < min.Z)
			min.Z = temp.Z;

		first = false;
	}

	Min = min;
	Max = max;
}

bool BoundingBox::Contains(const Vector3& point) const
{
	if ((Min.X <= point.X) && (point.X <= Max.X) && (Min.Y <= point.Y) &&
		(point.Y <= Max.Y) && (Min.Z <= point.Z) && (point.Z <= Max.Z))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool BoundingBox::Contains(const BoundingSphere& sphere) const
{
	Vector3 center = Vector3::Clamp(sphere.Center, Min, Max);
	real distanceSq = Vector3::DistanceSquared(sphere.Center, center);
	real radius = sphere.Radius;
	if (distanceSq > (radius * radius))
	{
		return false;
	}
	else
	{
		if (((Min.X + radius) <= sphere.Center.X) && (sphere.Center.X <= (Max.X - radius)) &&
			((Max.X - Min.X) > radius) && ((Min.Y + radius) <= sphere.Center.Y) &&
			(sphere.Center.Y <= (Max.Y - radius)) && ((Max.Y - Min.Y) > radius) &&
			((Min.Z + radius) <= sphere.Center.Z) && (sphere.Center.Z <= (Max.Z - radius)) &&
			((Max.X - Min.X) > radius))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool BoundingBox::Contains(const BoundingBox& box) const
{
	if ((Max.X >= box.Min.X) && (Min.X <= box.Max.X) && (Max.Y >= box.Min.Y) &&
		(Min.Y <= box.Max.Y) && (Max.Z >= box.Min.Z) && (Min.Z <= box.Max.Z))
	{
		if ((Min.X <= box.Min.X) && (box.Max.X <= Max.X) && (Min.Y <= box.Min.Y) &&
			(box.Max.Y <= Max.Y) && (Min.Z <= box.Min.Z) && (box.Max.Z <= Max.Z))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool BoundingBox::Intersects(const BoundingSphere& sphere) const
{
	Vector3 center = Vector3::Clamp(sphere.Center, Min, Max);
	real distanceSq = Vector3::DistanceSquared(sphere.Center, center);
	if (distanceSq > (sphere.Radius * sphere.Radius))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool BoundingBox::Intersects(const BoundingBox& box) const
{
    if ((Max.X >= box.Min.X) && (Min.X <= box.Max.X))
    {
        if ((Max.Y < box.Min.Y) || (Min.Y > box.Max.Y))
        {
            return false;
        }
        if ((Max.Z >= box.Min.Z) && (Min.Z <= box.Max.Z))
        {
            return true;
        }
    }
    return false;
}

bool BoundingBox::Intersects(const Ray3& ray, real& result) const
{
    real min = (real)0.0;
	real max = SE_MAX_R32;
	if (Math::Abs(ray.Direction.X) < Math::Epsilon)
    {
        if ((ray.Origin.X < Min.X) || (ray.Origin.X > Max.X))
        {
            return false;
        }
    }
    else
    {
        real invDirX = (real)1.0 / ray.Direction.X;
        real minX = (Min.X - ray.Origin.X) * invDirX;
        real maxX = (Max.X - ray.Origin.X) * invDirX;
        if (minX > maxX)
        {
            real swap = minX;
            minX = maxX;
            maxX = swap;
        }
        min = Math::Max(minX, min);
        max = Math::Min(maxX, max);
        if (min > max)
        {
            return false;
        }
    }
    if (Math::Abs(ray.Direction.Y) < Math::Epsilon)
    {
        if ((ray.Origin.Y < Min.Y) || (ray.Origin.Y > Max.Y))
        {
            return false;
        }
    }
    else
    {
        real invDirY = (real)1.0 / ray.Direction.Y;
        real minY = (Min.Y - ray.Origin.Y) * invDirY;
        real maxY = (Max.Y - ray.Origin.Y) * invDirY;
        if (minY > maxY)
        {
            real swap = minY;
            minY = maxY;
            maxY = swap;
        }
        min = Math::Max(minY, min);
        max = Math::Min(maxY, max);
        if (min > max)
        {
            return false;
        }
    }
    if (Math::Abs(ray.Direction.Z) < Math::Epsilon)
    {
        if ((ray.Origin.Z < Min.Z) || (ray.Origin.Z > Max.Z))
        {
            return false;
        }
    }
    else
    {
        real invDirZ = (real)1.0 / ray.Direction.Z;
        real minZ = (Min.Z - ray.Origin.Z) * invDirZ;
        real maxZ = (Max.Z - ray.Origin.Z) * invDirZ;
        if (minZ > maxZ)
        {
            real swap = minZ;
            minZ = maxZ;
            maxZ = swap;
        }
        min = Math::Max(minZ, min);
        max = Math::Min(maxZ, max);
        if (min > max)
        {
            return false;
        }
    }
	result = min;
    return true;
}

PlaneIntersectionType BoundingBox::Intersects(const Plane& plane) const
{
	return plane.Intersects(*this);
}

}
