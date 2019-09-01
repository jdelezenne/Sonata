/*=============================================================================
RTBoxShape.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _RAYTRACER_BOXSHAPE_H_
#define _RAYTRACER_BOXSHAPE_H_

#include "Common.h"
#include "SceneObject.h"

namespace Raytracer
{
	class RTBoxShape : public RTSceneObject
	{
		SE_DECLARE_CLASS(RTBoxShape, RTSceneObject);

		SE_BEGIN_REFLECTION(RTBoxShape);
			SE_Field(_Box.Center, Vector3, Public);
			SE_Field(_Box.Extents, Vector3, Public);
		SE_END_REFLECTION(RTBoxShape);

	public:
		//@{
		RTBoxShape();
		virtual ~RTBoxShape();
		//@}

		//@{
		Box& GetBox() { return _Box; }

		Vector3 GetCenter() const { return _Box.Center; }
		void SetCenter(const Vector3& value) { _Box.Center = value; }

		Vector3 GetExtent() const { return _Box.Extents; }
		void SetExtent(const Vector3& value) { _Box.Extents = value; }
		//@}

		//@{
		virtual AABB GetAABB();

		virtual void Intersect(RenderState& state, const Ray3& ray, TraceResult& result);

		virtual Vector3 GetNormal(const Vector3& p);

		virtual Vector2 GetUV(const Vector3& p);
		//@}

	protected:
		Box _Box;
	};
}

#endif 
