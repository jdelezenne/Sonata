/*=============================================================================
RTBoxShape.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "BoxShape.h"

namespace Raytracer
{
	SE_IMPLEMENT_CLASS(RTBoxShape);
	SE_IMPLEMENT_REFLECTION(RTBoxShape);

	RTBoxShape::RTBoxShape() :
		RTSceneObject()
	{
	}

	RTBoxShape::~RTBoxShape()
	{
	}

	AABB RTBoxShape::GetAABB()
	{
		return AABB(_Box.GetMinimum(), _Box.GetMaximum());
	}

	void RTBoxShape::Intersect(RenderState& state, const Ray3& ray, TraceResult& result)
	{
		// Instead of computing all the intersections of the 6 planes,
		// we do the computations per dimension

		// For each dimension:
		// Test if parallel
		//   if ray.dx = 0 and ray.ox < x1 or ray.ox x2, no intersection
		// Find the intersections t1 and t2
		//   t1 = (x1 - ray.ox) / ray.dx
		//   t2 = (x2 - ray.ox) / ray.dx

		// We maintain tnar and tfar the closest and farthest intersections
		//   if t1 > tnear, tnear = t1
		//   if t2 > tfar, tfar = t2

		// Check if there is an intersection
		//   if tfar > tnear, no intersection

		// Check if the box is hebind the eye
		//   if tfar < tmin, box is behind

		// Check if the box is hebind the eye
		//   if tnear > tmin,
		//     closest intersection at tnear
		//   else
		//     closest intersection at tfar

		int i;
		real dist[6];
		Vector3 points[6];
		result._Hit = false;

		for (i = 0; i < 6; i++)
			dist[i] = -1.0f;

		Vector3 p1 = _Box.Center;
		Vector3 p2 = _Box.Center + _Box.Extents;

		if (ray.Direction.x > 0.0f)
		{
			real invDir = 1.0f / ray.Direction.x;
			dist[0] = (p1.x - ray.Origin.x) * invDir;
			dist[3] = (p2.x - ray.Origin.x) * invDir;
		}
		if (ray.Direction.y > 0.0f)
		{
			real invDir = 1.0f / ray.Direction.y;
			dist[1] = (p1.y - ray.Origin.y) * invDir;
			dist[4] = (p2.y - ray.Origin.y) * invDir;
		}
		if (ray.Direction.z > 0.0f)
		{
			real invDir = 1.0f / ray.Direction.z;
			dist[2] = (p1.z - ray.Origin.z) * invDir;
			dist[5] = (p2.z - ray.Origin.z) * invDir;
		}
		for (i = 0; i < 6; i++)
		{
			if (dist[i] > 0.0f)
			{
				points[i] = ray.Origin + dist[i] * ray.Direction;
				if ((points[i].x > (p1.x - Math::Epsilon)) && (points[i].x < (p2.x + Math::Epsilon)) && 
					(points[i].y > (p1.y - Math::Epsilon)) && (points[i].y < (p2.y + Math::Epsilon)) &&
					(points[i].z > (p1.z - Math::Epsilon)) && (points[i].z < (p2.z + Math::Epsilon)))
				{
					result._Distance = dist[i];
					result._Hit = true;
				}
			}
		}
	}

	Vector3 RTBoxShape::GetNormal(const Vector3& p)
	{
		Vector3 p1 = _Box.Center;
		Vector3 p2 = _Box.Center + _Box.Extents;
		Vector3 normal = Vector3::Zero;
		Vector3 a, b, c;
		real32 t;

		c.x = (p1.x + p2.x) / 2.0;
		c.y = (p1.y + p2.y) / 2.0;
		c.z = (p1.z + p2.z) / 2.0;

		b = (p - c);

		a.x = Math::Abs(b.x);
		a.y = Math::Abs(b.y);
		a.z = Math::Abs(b.z);

		t = Math::Max(a.x, Math::Max(a.y, a.z));  

		if (t == a.x)
			normal.x = b.x;  

		if (t == a.y)
			normal.y = b.y; 

		if (t == a.z)
			normal.z = b.z;

		return Vector3::Normalize(normal);
	}

	Vector2 RTBoxShape::GetUV(const Vector3& p)
	{
		return Vector2::Zero;
	}
}
