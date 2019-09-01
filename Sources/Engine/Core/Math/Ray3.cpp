/*=============================================================================
Ray3.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Ray3.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "Frustum.h"
#include "Plane.h"

namespace SonataEngine
{

real Ray3::GetDistanceSquared(const Vector3& value, real* t) const
{
	Vector3 diff = value - this->Origin;
	real ft = Vector3::Dot(this->Direction, diff);
	if (ft > (real)0.0)
	{
		diff -= ft * this->Direction;
	}
	else
	{
		ft = (real)0.0;
	}

	if (t != NULL)
		*t = ft;

	return diff.LengthSquared();
}

bool Ray3::Intersects(const BoundingSphere& sphere, real& result) const
{
	Vector3 diff = sphere.Center - Origin;
	real lengthSq = diff.LengthSquared();
	real radiusSq = sphere.Radius * sphere.Radius;
	if (lengthSq <= radiusSq)
	{
		return false;
	}
	real a0 = Vector3::Dot(diff, Direction);
	if (a0 < (real)0.0)
	{
		return false;
	}
	real a1 = lengthSq - (a0 * a0);
	if (a1 > radiusSq)
	{
		return false;
	}
	real root = (real)Math::Sqrt(radiusSq - a1);
	result = (a0 - root);
	return true;
}

bool Ray3::Intersects(const BoundingBox& box, real& result) const
{
	return box.Intersects(*this, result);
}

bool Ray3::Intersects(const Frustum& frustum, real& result) const
{
	return frustum.Intersects(*this, result);
}

bool Ray3::Intersects(const Plane& plane, real& result) const
{
	Vector3 point = plane.D * plane.Normal;
	real dotNormalDir = Vector3::Dot(plane.Normal, Direction);
	if (dotNormalDir == 0)
	{
		return false;
	}
	result = Vector3::Dot(plane.Normal, point - Origin) / dotNormalDir;
	return true;
}

}
