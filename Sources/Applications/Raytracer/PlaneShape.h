/*=============================================================================
RTPlaneShape.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _RAYTRACER_PLANESHAPE_H_
#define _RAYTRACER_PLANESHAPE_H_

#include "Common.h"
#include "SceneObject.h"

namespace Raytracer
{
	class RTPlaneShape : public RTSceneObject
	{
		SE_DECLARE_CLASS(RTPlaneShape, RTSceneObject);

		SE_BEGIN_REFLECTION(RTPlaneShape);
			SE_Field(_Plane.Normal, Vector3, Public);
			SE_Field(_Plane.Distance, real32, Public);
		SE_END_REFLECTION(RTPlaneShape);

	public:
		//@{
		RTPlaneShape();
		virtual ~RTPlaneShape();
		//@}

		//@{
		Plane& GetPlane() { return _Plane; }

		Vector3 GetNormal() const { return _Plane.Normal; }
		void SetNormal(const Vector3& value) { _Plane.Normal = value; }

		real32 GetDistance() const { return _Plane.Distance; }
		void SetDistance(real32 value) { _Plane.Distance = value; }
		//@}

		//@{
		virtual AABB GetAABB();

		virtual void Intersect(RenderState& state, const Ray3& ray, TraceResult& result);

		virtual Vector3 GetNormal(const Vector3& p);

		virtual Vector2 GetUV(const Vector3& p);
		//@}

	protected:
		Plane _Plane;
	};
}

#endif 
