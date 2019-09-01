/*=============================================================================
RTTriangleShape.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "TriangleShape.h"

namespace Raytracer
{
	SE_IMPLEMENT_CLASS(RTTriangleShape);
	SE_IMPLEMENT_REFLECTION(RTTriangleShape);

	RTTriangleShape::RTTriangleShape() :
		RTSceneObject()
	{
	}

	RTTriangleShape::~RTTriangleShape()
	{
	}

	AABB RTTriangleShape::GetAABB()
	{
		return AABB(
			Vector3::Min(_Triangle.p0, Vector3::Min(_Triangle.p1, _Triangle.p2)),
			Vector3::Max(_Triangle.p0, Vector3::Max(_Triangle.p1, _Triangle.p2)));
	}

	void RTTriangleShape::Intersect(RenderState& state, const Ray3& ray, TraceResult& result)
	{
		// Barycentric coordinates

		// P(alpha,beta,gamma) = alpha*a + beta*b + gamma*c with alpha + beta + gamma = 1
		//   and 0 < alpha < 1 & 0 < beta < 1 & 0 < gamma < 1

		// alpha + beta + gamma = 1
		// alpha = 1 - beta - gamma

		// P(beta,gamma) = (1 - beta - gamma)*a + beta*b + gamma*c
		// P(beta,gamma) = a + beta*(b-a) + gamma*(c-a)

		// Intersection if
		// P(t) = P(beta,gamma)
		// ray.o + t*ray.d = a + beta*(b-a) + gamma*(c-a)
		//   beta + gamma < 1 & beta > 0 & gamma > 0

		// Can be written as a standard linear equation
		// [ax-bx ax-cx ray.dx] [beta ]   [ax-ray.ox]
		// [ay-by ay-cy ray.dy] [gamma] = [ay-ray.oy]
		// [az-bz az-cz ray.dz] [  t  ]   [az-ray.oz]

		// Solved with Cramer's Rule

		Vector3 edge0, edge1;
		Vector3 vec;
		real det, inverseDet;
		Vector3 vP, vT, vQ;
		real u, v;

		// Edges
		edge0 = _Triangle.p1 - _Triangle.p0;
		edge1 = _Triangle.p2 - _Triangle.p0;

		// Calculate the determinant
		vP = Vector3::Cross(ray.Direction, edge1);

		det = Vector3::Dot(edge0, vP);

		// Check if the ray lies in the plane of the triangle
		if (Math::Equals(det, 0.0, Math::Epsilon))
		{
			result._Hit = false;
			return;
		}

		inverseDet = 1.0 / det;

		// Direction from the first point to the ray orgin
		vT = ray.Origin - _Triangle.p0;

		// Calculate U (beta)
		u = Vector3::Dot(vT, vP) * inverseDet;
		if (u < 0.0 || u > 1.0)
		{
			result._Hit = false;
			return;
		}

		vQ = Vector3::Cross(vT, edge0);

		// Calculate V (gamma)
		v = Vector3::Dot(ray.Direction, vQ) * inverseDet;
		if (v < 0.0 || u + v > 1.0)
		{
			result._Hit = false;
			return;
		}

		// Calculate t, the distance of intersection
		result._Distance = Vector3::Dot(edge1, vQ) * inverseDet;
		result._Hit = true;
	}

	Vector3 RTTriangleShape::GetNormal(const Vector3& p)
	{
		return _Triangle.GetNormal();
	}

	Vector2 RTTriangleShape::GetUV(const Vector3& p)
	{
		Vector2 uv;

		Vector3 edge0, edge1;
		Vector3 normal, vec, temp;
		real sqLength;

		edge0 = _Triangle.p1 - _Triangle.p0;
		edge1 = _Triangle.p2 - _Triangle.p0;

		normal = Vector3::Cross(edge0, edge1);
		sqLength = Vector3::Dot(normal, normal); 

		vec = p - _Triangle.p0;

		temp = Vector3::Cross(vec, edge1);
		uv.x = Vector3::Dot(temp, normal) / sqLength;   

		temp = Vector3::Cross(edge0, vec);
		uv.y = Vector3::Dot(temp, normal) / sqLength;   

		//uv.z = 1.0 - (uv.x + uv.y);

		return uv;
	}
}
