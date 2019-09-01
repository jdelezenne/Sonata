/*=============================================================================
Mesh.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "Mesh.h"
#include "TriangleShape.h"

namespace Raytracer
{
	SE_IMPLEMENT_CLASS(RTMesh);
	SE_IMPLEMENT_REFLECTION(RTMesh);

	RTMesh::RTMesh() :
		RTSceneObject()
	{
		_LastTriangle = 0;
	}

	RTMesh::~RTMesh()
	{
	}

	AABB RTMesh::GetAABB()
	{
		return _BoundingBox;
	}

	void RTMesh::Intersect(RenderState& state, const Ray3& ray, TraceResult& result)
	{
		int i;
		int count;
		bool indexed;

		if (_Indices.Count() == 0)
		{
			count = _Vertices.Count() / 3;
			indexed = false;
		}
		else
		{
			count = _Indices.Count() / 3;
			indexed = true;
		}

		TraceResult tr;
		int32 minTriangle = -1;
		real32 minDistance = SE_MAX_R32;

		result._Hit = false;

		for (i = 0; i < count; ++i)
		{
			const Vector3& v0 = (indexed ? _Vertices[i*3] : _Vertices[_Indices[i*3]]);
			const Vector3& v1 = (indexed ? _Vertices[i*3+1] : _Vertices[_Indices[i*3+1]]);
			const Vector3& v2 = (indexed ? _Vertices[i*3+2] : _Vertices[_Indices[i*3+2]]);

			RTTriangleShape triangle;
			triangle.SetVertices(v0, v1, v2);

			triangle.Intersect(state, state._Ray, tr);

			if (tr._Hit && tr._Distance < minDistance)
			{
				minDistance = tr._Distance;
				minTriangle = i;
			}
		}

		if (minTriangle != -1)
		{
			result._Distance = minDistance;
			result._Hit = true;
			_LastTriangle = minTriangle;
		}
	}

	Vector3 RTMesh::GetNormal(const Vector3& p)
	{
		return _Normals[_LastTriangle*3];
	}

	Vector2 RTMesh::GetUV(const Vector3& p)
	{
		if (_TexCoords.Count() > 0)
			return _TexCoords[_LastTriangle*3];
		else
			return Vector2::Zero;
	}

	void RTMesh::GenerateNormals()
	{
		int i;
		int count;
		bool indexed;

		if (_Indices.Count() == 0)
		{
			count = _Vertices.Count() / 3;
			indexed = false;
		}
		else
		{
			count = _Indices.Count() / 3;
			indexed = true;
		}

		_Normals.Clear();
		_Normals.SetSize(count*3);

		for (i = 0; i < count; ++i)
		{
			const Vector3& v0 = (indexed ? _Vertices[i*3] : _Vertices[_Indices[i*3]]);
			const Vector3& v1 = (indexed ? _Vertices[i*3+1] : _Vertices[_Indices[i*3+1]]);
			const Vector3& v2 = (indexed ? _Vertices[i*3+2] : _Vertices[_Indices[i*3+2]]);

			Vector3 normal = Vector3::Cross(v1 - v0, v2 - v1);
			normal = Vector3::Normalize(normal);
			_Normals[i*3] = _Normals[i*3+1] = _Normals[i*3+2] = normal;
		}
	}

	void RTMesh::UpdateBoundingBox()
	{
		int count = _Vertices.Count();

		// If there are no vertices, the AABB is invalid
		_BoundingBox = AABB::Empty;

		// Take the first vertex
		VertexArray::Iterator it = _Vertices.GetIterator();
		if (it.Next())
		{
			_BoundingBox.Minimum = _BoundingBox.Maximum = it.Current();
		}

		// Iterate all the vertices
		while (it.Next())
		{
			const Vector3& v = it.Current();
			_BoundingBox.Minimum = Vector3::Min(_BoundingBox.Minimum, v);
			_BoundingBox.Maximum = Vector3::Max(_BoundingBox.Maximum, v);
		}
	}
}
