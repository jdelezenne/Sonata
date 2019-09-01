/*=============================================================================
RTPlaneShape.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "PlaneShape.h"

namespace Raytracer
{
	SE_IMPLEMENT_CLASS(RTPlaneShape);
	SE_IMPLEMENT_REFLECTION(RTPlaneShape);

	RTPlaneShape::RTPlaneShape() :
		RTSceneObject()
	{
	}
	
	RTPlaneShape::~RTPlaneShape()
	{
	}

	AABB RTPlaneShape::GetAABB()
	{
		Vector3 infinite(SE_MAX_R32);
		return AABB(-infinite, infinite);
	}

	void RTPlaneShape::Intersect(RenderState& state, const Ray3& ray, TraceResult& result)
	{
		real d = Vector3::Dot(_Plane.Normal, ray.Direction);
		if (d < (real)0.0)
		{
			result._Distance = -(Vector3::Dot(_Plane.Normal, ray.Origin) + _Plane.Distance) / d;
			result._Hit = true;
		}
		else
		{
			result._Hit = false;
		}
	}

	Vector3 RTPlaneShape::GetNormal(const Vector3& p)
	{
		return _Plane.Normal;
	}

	Vector2 RTPlaneShape::GetUV(const Vector3& p)
	{
		Vector3 _UAxis = Vector3(_Plane.Normal.y, _Plane.Normal.z, -_Plane.Normal.x);
		Vector3 _VAxis = Vector3::Cross(_UAxis, _Plane.Normal);

		return Vector2(Vector3::Dot(p, _UAxis), Vector3::Dot(p, _VAxis));
	}
}
