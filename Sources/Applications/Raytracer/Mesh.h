/*=============================================================================
Mesh.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _RAYTRACER_MESH_H_
#define _RAYTRACER_MESH_H_


#include "Common.h"
#include "SceneObject.h"

namespace Raytracer
{
	class RTMesh : public RTSceneObject
	{
		SE_DECLARE_CLASS(RTMesh, RTSceneObject);

		SE_BEGIN_REFLECTION(RTMesh);
		SE_END_REFLECTION(RTMesh);

	public:
		typedef Array<Vector3> VertexArray;
		typedef Array<Vector3> NormalArray;
		typedef Array<Vector2> TexCoordArray;
		typedef Array<Colour32> ColourArray;
		typedef Array<int32> IndexArray;

		//@{
		RTMesh();
		virtual ~RTMesh();
		//@}

		//@{
		VertexArray& GetVertices() { return _Vertices; }
		void SetVertices(const VertexArray& value) { _Vertices = value; }

		NormalArray& GetNormals() { return _Normals; }
		void SetNormals(const NormalArray& value) { _Normals = value; }

		ColourArray& GetColours() { return _Colours; }
		void SetColours(const ColourArray& value) { _Colours = value; }

		TexCoordArray& GetTexCoords() { return _TexCoords; }
		void SetTexCoords(const TexCoordArray& value) { _TexCoords = value; }

		IndexArray& GetIndices() { return _Indices; }
		void SetIndices(const IndexArray& value) { _Indices = value; }
		//@}

		//@{
		virtual AABB GetAABB();

		virtual void Intersect(RenderState& state, const Ray3& ray, TraceResult& result);

		virtual Vector3 GetNormal(const Vector3& p);

		virtual Vector2 GetUV(const Vector3& p);

		void GenerateNormals();

		void UpdateBoundingBox();
		//@}

	protected:
		VertexArray _Vertices;
		NormalArray _Normals;
		ColourArray _Colours;
		TexCoordArray _TexCoords;
		IndexArray _Indices;
		AABB _BoundingBox;
		int32 _LastTriangle;
	};
}

#endif 
