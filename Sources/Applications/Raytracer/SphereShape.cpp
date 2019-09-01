/*=============================================================================
RTSphereShape.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "SphereShape.h"

namespace Raytracer
{
	SE_IMPLEMENT_CLASS(RTSphereShape);
	SE_IMPLEMENT_REFLECTION(RTSphereShape);

	RTSphereShape::RTSphereShape() :
		RTSceneObject()
	{
	}

	RTSphereShape::~RTSphereShape()
	{
	}

	AABB RTSphereShape::GetAABB()
	{
		Vector3 length(_Sphere.Radius);
		return AABB(_Sphere.Center - length, _Sphere.Center + length);
	}

	void RTSphereShape::Intersect(RenderState& state, const Ray3& ray, TraceResult& result)
	{
		// Get the direction of the ray to the sphere
		Vector3 diff = ray.Origin - _Sphere.Center;

		real b = -Vector3::Dot(diff, ray.Direction);
		real det = (b * b) - Vector3::Dot(diff, diff) + (_Sphere.Radius * _Sphere.Radius);

		result._Hit = false;
		if (det > (real)0.0)
		{
			det = Math::Sqrt(det);
			real a1 = b - det;
			real a2 = b + det;
			if (a2 > (real)0.0)
			{
				if (a1 < (real)0.0) 
				{
					result._Distance = a2;
					result._Hit = true;
				}
				else
				{
					result._Distance = a1;
					result._Hit = true;
				}
			}
		}
	}

	Vector3 RTSphereShape::GetNormal(const Vector3& p)
	{
		real32 inverseRadius = 1.0f / _Sphere.Radius;
		return (p - _Sphere.Center) * inverseRadius;
	}

	Vector2 RTSphereShape::GetUV(const Vector3& p)
	{
		Vector2 uv;
		real32 inverseRadius = 1.0f / _Sphere.Radius;
		Vector3 vp = (p - _Sphere.Center) * inverseRadius;
		real32 phi = Math::Acos( -Vector3::Dot(vp, Vector3::YAxis));
		uv.x = uv.y = phi * (1.0f / Math::PI);
		real32 theta = (Math::Acos(Vector3::Dot(Vector3::XAxis, vp) / Math::Sin(phi))) * (2.0f / Math::PI);
		if (Vector3::Dot(Vector3::ZAxis, vp) >= 0)
			uv.x = (1.0f - theta);
		else
			uv.x = theta;
		return uv;
	}
}
