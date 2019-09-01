/*=============================================================================
Frustum.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Frustum.h"
#include "Core/Math/BoundingBox.h"
#include "Core/Math/BoundingSphere.h"
#include "Core/Math/OBB.h"
#include "Core/Math/Ray3.h"

namespace SonataEngine
{

Frustum::Frustum(const Frustum& value)
{
	for (int i = 0; i < NumFrustumPlanes; i++)
	{
		_planes[i] = value.GetFrustumPlane((FrustumPlane)i);
	}
}

Vector3 Frustum::ComputeIntersection(const Plane& plane, const Ray3& ray) const
{
	real distance =
		(-plane.D - Vector3::Dot(plane.Normal, ray.Origin)) /
		Vector3::Dot(plane.Normal, ray.Direction);
	return (ray.Origin + (ray.Direction * distance));
}

Ray3 Frustum::ComputeIntersectionLine(const Plane& p1, const Plane& p2) const
{
	Ray3 result;
	result.Direction = Vector3::Cross(p1.Normal, p2.Normal);
	real lengthSq = result.Direction.LengthSquared();
	result.Origin = (Vector3::Cross(
		(-p1.D * p2.Normal) + (p2.D * p1.Normal), result.Direction) / lengthSq);
	return result;
}

void Frustum::SetFrustumMatrix(const Matrix4& value)
{
	// Extract the sides from the clipping planes.

	_planes[FrustumPlane_Right].Normal.X = value.Data[3] - value.Data[0];
	_planes[FrustumPlane_Right].Normal.Y = value.Data[7] - value.Data[4];
	_planes[FrustumPlane_Right].Normal.Z = value.Data[11] - value.Data[8];
	_planes[FrustumPlane_Right].D = value.Data[15] - value.Data[12];
	_planes[FrustumPlane_Right].Normalize();

	_planes[FrustumPlane_Left].Normal.X = value.Data[3] + value.Data[0];
	_planes[FrustumPlane_Left].Normal.Y = value.Data[7] + value.Data[4];
	_planes[FrustumPlane_Left].Normal.Z = value.Data[11] + value.Data[8];
	_planes[FrustumPlane_Left].D = value.Data[15] + value.Data[12];
	_planes[FrustumPlane_Left].Normalize();

	_planes[FrustumPlane_Bottom].Normal.X = value.Data[3] + value.Data[1];
	_planes[FrustumPlane_Bottom].Normal.Y = value.Data[7] + value.Data[5];
	_planes[FrustumPlane_Bottom].Normal.Z = value.Data[11] + value.Data[9];
	_planes[FrustumPlane_Bottom].D = value.Data[15] + value.Data[13];
	_planes[FrustumPlane_Bottom].Normalize();

	_planes[FrustumPlane_Top].Normal.X = value.Data[3] - value.Data[1];
	_planes[FrustumPlane_Top].Normal.Y = value.Data[7] - value.Data[5];
	_planes[FrustumPlane_Top].Normal.Z = value.Data[11] - value.Data[9];
	_planes[FrustumPlane_Top].D = value.Data[15] - value.Data[13];
	_planes[FrustumPlane_Top].Normalize();

	_planes[FrustumPlane_Far].Normal.X = value.Data[3] - value.Data[2];
	_planes[FrustumPlane_Far].Normal.Y = value.Data[7] - value.Data[6];
	_planes[FrustumPlane_Far].Normal.Z = value.Data[11] - value.Data[10];
	_planes[FrustumPlane_Far].D = value.Data[15] - value.Data[14];
	_planes[FrustumPlane_Far].Normalize();

	_planes[FrustumPlane_Near].Normal.X = value.Data[3] + value.Data[2];
	_planes[FrustumPlane_Near].Normal.Y = value.Data[7] + value.Data[6];
	_planes[FrustumPlane_Near].Normal.Z = value.Data[11] + value.Data[10];
	_planes[FrustumPlane_Near].D = value.Data[15] + value.Data[14];
	_planes[FrustumPlane_Near].Normalize();

	_matrix = value;
}

void Frustum::UpdateCorners()
{
	Ray3 ray = ComputeIntersectionLine(_planes[FrustumPlane_Near], _planes[FrustumPlane_Left]);
	_corners[0] = ComputeIntersection(_planes[FrustumPlane_Top], ray);
	_corners[3] = ComputeIntersection(_planes[FrustumPlane_Bottom], ray);
	ray = ComputeIntersectionLine(_planes[FrustumPlane_Right], _planes[FrustumPlane_Near]);
	_corners[1] = ComputeIntersection(_planes[FrustumPlane_Top], ray);
	_corners[2] = ComputeIntersection(_planes[FrustumPlane_Bottom], ray);
	ray = ComputeIntersectionLine(_planes[FrustumPlane_Left], _planes[FrustumPlane_Far]);
	_corners[4] = ComputeIntersection(_planes[FrustumPlane_Top], ray);
	_corners[7] = ComputeIntersection(_planes[FrustumPlane_Bottom], ray);
	ray = ComputeIntersectionLine(_planes[FrustumPlane_Far], _planes[FrustumPlane_Right]);
	_corners[5] = ComputeIntersection(_planes[FrustumPlane_Top], ray);
	_corners[6] = ComputeIntersection(_planes[FrustumPlane_Bottom], ray);
}

bool Frustum::Contains(const Vector3& point) const
{
	for (int i = 0; i < NumFrustumPlanes; i++)
	{
		// Check if the point is behind a side of the frustum
		if (_planes[i].GetDistance(point) <= Math::Epsilon)
		{
			// The point is behind a side, so it is outside of the frustum
			return false;
		}
	}

	// The point is in front of all the sides of the frustum, so it is inside of the frustum
	return true;
}

bool Frustum::Contains(const BoundingSphere& sphere) const
{
	for (int i = 0; i < NumFrustumPlanes; i++)
	{
		// Check if the center of the sphere is farther away from the plane that its radius
		if (_planes[i].GetDistance(sphere.Center) <= -sphere.Radius)
		{
			// The sphere is outside of the frustum
			return false;
		}
	}

	// The sphere is inside of the frustum
	return true;
}

bool Frustum::Contains(const BoundingBox& box) const
{
	Vector3 min = box.Min;
	Vector3 max = box.Max;

	// Check if all the corners of the box are outside of the frustum
	for (int i = 0; i < NumFrustumPlanes; i++)
	{
		if (_planes[i].GetDistance(Vector3(min.X, min.Y, min.Z)) > 0) continue;
		if (_planes[i].GetDistance(Vector3(min.X, min.Y, max.Z)) > 0) continue;
		if (_planes[i].GetDistance(Vector3(min.X, max.Y, min.Z)) > 0) continue;
		if (_planes[i].GetDistance(Vector3(min.X, max.Y, max.Z)) > 0) continue;
		if (_planes[i].GetDistance(Vector3(max.X, min.Y, min.Z)) > 0) continue;
		if (_planes[i].GetDistance(Vector3(max.X, min.Y, max.Z)) > 0) continue;
		if (_planes[i].GetDistance(Vector3(max.X, max.Y, min.Z)) > 0) continue;
		if (_planes[i].GetDistance(Vector3(max.X, max.Y, max.Z)) > 0) continue;

		// The box is outside of the frustum
		return false;
	}

	// The box is inside of the frustum
	return true;
}

bool Frustum::Contains(const OBB& obb) const
{
	Vector3 corners[8];
	obb.GetCorners(corners);

	for (int i = 0; i < NumFrustumPlanes; i++)
	{
		// Check against each plane if one of the corners of the box is behind one of the plane
		for (int c = 0; c < 8; c++ )
		{
			if (_planes[i].GetDistance(corners[c])  + _planes[i].D > 0) continue;
		}

		// The box is outside of the frustum
		return false;
	}

	// The box is inside of the frustum
	return true;
}

int Frustum::Intersects(const BoundingSphere& sphere) const
{
	return 1;
}

int Frustum::Intersects(const BoundingBox& box) const
{
	return 1;
}

int Frustum::Intersects(const OBB& obb) const
{
	return 1;
}

int Frustum::Intersects(const Frustum& frustum) const
{
	return 1;
}

bool Frustum::Intersects(const Ray3& ray, real& result) const
{
	if (Contains(ray.Origin))
	{
		result = (real)0.0;
		return true;
	}
	else
	{
		real min = SE_MIN_R32;
		real max = SE_MAX_R32;
		for (int i = 0; i < NumFrustumPlanes; i++)
		{
			real dotDirNormal;
			real dotPosNormal;
			Vector3 normal = _planes[i].Normal;
			dotDirNormal = Vector3::Dot(ray.Direction, normal);
			dotPosNormal = Vector3::Dot(ray.Origin, normal);
			dotPosNormal += _planes[i].D;
			if (Math::Abs(dotDirNormal) < Math::Epsilon)
			{
				if (dotPosNormal > (real)0.0)
				{
					return false;
				}
			}
			else
			{
				real fraction = -dotPosNormal / dotDirNormal;
				if (dotDirNormal < (real)0.0)
				{
					if (fraction > max)
					{
						return false;
					}
					if (fraction > min)
					{
						min = fraction;
					}
				}
				else
				{
					if (fraction < min)
					{
						return false;
					}
					if (fraction < max)
					{
						max = fraction;
					}
				}
			}
		}
		real distance = (min >= (real)0.0) ? min : max;
		if (distance >= (real)0.0)
		{
			result = distance;
			return true;
		}
		else
		{
			return false;
		}
	}
}

PlaneIntersectionType Frustum::Intersects(const Plane& plane) const
{
	int flag = 0;
	for (int i = 0; i < 8; i++)
	{
		real dotCornerNormal = Vector3::Dot(_corners[i], plane.Normal);
		if ((dotCornerNormal + plane.D) > (real)0.0)
		{
			flag |= 1;
		}
		else
		{
			flag |= 2;
		}
		if (flag == 3)
		{
			return PlaneIntersectionType_Intersecting;
		}
	}
	return (flag == 1) ? PlaneIntersectionType_Front : PlaneIntersectionType_Back;
}

}
