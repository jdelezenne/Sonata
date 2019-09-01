/*=============================================================================
Plane.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Plane.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "Frustum.h"

namespace SonataEngine
{

PlaneIntersectionType Plane::Intersects(const BoundingSphere& sphere) const
{
	real cosAngle = Vector3::Dot(sphere.Center, Normal);
	real distance = cosAngle + D;
	if (distance > sphere.Radius)
	{
		return PlaneIntersectionType_Front;
	}
	else if (distance < -sphere.Radius)
	{
		return PlaneIntersectionType_Back;
	}
	else
	{
		return PlaneIntersectionType_Intersecting;
	}
}

PlaneIntersectionType Plane::Intersects(const BoundingBox& box) const
{
	Vector3 vector1;
	Vector3 vector2;
	real cosAngle1;
	real cosAngle2;
	vector1.X = (Normal.X >= (real)0.0) ? box.Min.X : box.Max.X;
	vector1.Y = (Normal.Y >= (real)0.0) ? box.Min.Y : box.Max.Y;
	vector1.Z = (Normal.Z >= (real)0.0) ? box.Min.Z : box.Max.Z;
	vector2.X = (Normal.X >= (real)0.0) ? box.Max.X : box.Min.X;
	vector2.Y = (Normal.Y >= (real)0.0) ? box.Max.Y : box.Min.Y;
	vector2.Z = (Normal.Z >= (real)0.0) ? box.Max.Z : box.Min.Z;

	cosAngle1 = Vector3::Dot(Normal, vector1);
	if ((cosAngle1 + D) > (real)0.0)
	{
		return PlaneIntersectionType_Front;
	}

	cosAngle2 = Vector3::Dot(Normal, vector2);
	if ((cosAngle2 + D) < (real)0.0)
	{
		return PlaneIntersectionType_Back;
	}

	return PlaneIntersectionType_Intersecting;
}

PlaneIntersectionType Plane::Intersects(const Frustum& frustum) const
{
	return frustum.Intersects(*this);
}

}
