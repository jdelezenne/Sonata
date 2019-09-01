/*=============================================================================
RTCamera.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _RAYTRACER_CAMERA_H_
#define _RAYTRACER_CAMERA_H_

#include "Common.h"

namespace Raytracer
{
	enum ProjectionType
	{
		ProjectionType_Orthographic,
		ProjectionType_Perspective
	};

	class RTCamera : public RefObject
	{
		SE_DECLARE_CLASS(RTCamera, Object);

		SE_BEGIN_REFLECTION(RTCamera);
			SE_Field(_Position, Vector3, Public);
			SE_Field(_LookAt, Vector3, Public);
		SE_END_REFLECTION(RTCamera);

	public:
		//@{
		RTCamera();
		virtual ~RTCamera();
		//@}

		//@{
		Vector3 GetPosition() const { return _Position; }
		void SetPosition(Vector3 value) { _Position = value; }

		Quaternion GetOrientation() const { return _Orientation; }
		void SetOrientation(Quaternion value) { _Orientation = value; }

		Vector3 GetLookAt() const { return _LookAt; }
		void SetLookAt(Vector3 value) { _LookAt = value; }
		//@}

		//@{
		void Update();

		void Raycast(const Vector2& position, Ray3& ray);
		//@}

	protected:
		ProjectionType _ProjectionType;
		real32 _FOV;
		real32 _AspectRatio;
		real32 _NearPlane;
		real32 _FarPlane;

		Vector3 _Position;
		Vector3 _LookAt;
		Quaternion _Orientation;
		Vector3 _Scale;
		Matrix4 _Transform;

		Vector3 _Direction;
		Vector3 _Up;
		Vector3 _Right;

		Matrix4 _Projection;
		Matrix4 _View;
		Matrix4 _InverseView;
	};
}

#endif 
