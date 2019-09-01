/*=============================================================================
RTSphereShape.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _RAYTRACER_SPHERESHAPE_H_
#define _RAYTRACER_SPHERESHAPE_H_

#include "Common.h"
#include "SceneObject.h"

namespace Raytracer
{
	class RTSphereShape : public RTSceneObject
	{
		SE_DECLARE_CLASS(RTSphereShape, RTSceneObject);

		SE_BEGIN_REFLECTION(RTSphereShape);
			SE_Field(_Sphere.Center, Vector3, Public);
			SE_Field(_Sphere.Radius, real32, Public);
		SE_END_REFLECTION(RTSphereShape);

	public:
		//@{
		RTSphereShape();
		virtual ~RTSphereShape();
		//@}

		//@{
		Sphere& GetSphere() { return _Sphere; }

		Vector3 GetCenter() const { return _Sphere.Center; }
		void SetCenter(const Vector3& value) { _Sphere.Center = value; }

		real32 GetRadius() const { return _Sphere.Radius; }
		void SetRadius(real32 value) { _Sphere.Radius = value; }
		//@}

		//@{
		virtual AABB GetAABB();

		virtual void Intersect(RenderState& state, const Ray3& ray, TraceResult& result);

		virtual Vector3 GetNormal(const Vector3& p);

		virtual Vector2 GetUV(const Vector3& p);
		//@}

	protected:
		Sphere _Sphere;
	};
}

#endif 
