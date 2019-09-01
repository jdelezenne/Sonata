/*=============================================================================
RTTriangleShape.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _RAYTRACER_TRIANGLESHAPE_H_
#define _RAYTRACER_TRIANGLESHAPE_H_

#include "Common.h"
#include "SceneObject.h"

namespace Raytracer
{
	class RTTriangleShape : public RTSceneObject
	{
		SE_DECLARE_CLASS(RTTriangleShape, RTSceneObject);

		SE_BEGIN_REFLECTION(RTTriangleShape);
			SE_Field(_Triangle.p0, Vector3, Public);
			SE_Field(_Triangle.p1, Vector3, Public);
			SE_Field(_Triangle.p2, Vector3, Public);
		SE_END_REFLECTION(RTTriangleShape);

	public:
		//@{
		RTTriangleShape();
		virtual ~RTTriangleShape();
		//@}

		//@{
		Triangle& GetTriangle() { return _Triangle; }

		void GetVertices(Vector3& p0, Vector3& p1, Vector3& p2) const
		{ p0 = _Triangle.p0; p1 = _Triangle.p1; p2 = _Triangle.p2; }

		void SetVertices(const Vector3& p0, const Vector3& p1, const Vector3& p2)
		{ _Triangle.p0 = p0; _Triangle.p1 = p1; _Triangle.p2 = p2; }
		//@}

		//@{
		virtual AABB GetAABB();

		virtual void Intersect(RenderState& state, const Ray3& ray, TraceResult& result);

		virtual Vector3 GetNormal(const Vector3& p);

		virtual Vector2 GetUV(const Vector3& p);
		//@}

	protected:
		Triangle _Triangle;
	};
}

#endif 
