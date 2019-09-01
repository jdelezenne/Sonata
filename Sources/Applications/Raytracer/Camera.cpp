/*=============================================================================
RTCamera.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "Camera.h"

namespace Raytracer
{
	SE_IMPLEMENT_CLASS(RTCamera);
	SE_IMPLEMENT_REFLECTION(RTCamera);

	RTCamera::RTCamera() :
		RefObject(),
		_FOV(45.0f),
		_AspectRatio(1.0f),
		_NearPlane(1.0f),
		_FarPlane(1000.0f),
		_Position(Vector3(0.0, 0.0, 0.0)),
		_Orientation(Quaternion(0.0, 0.0, 0.0, 1.0)),
		_Scale(Vector3(1.0, 1.0, 1.0)),
		_Transform(Matrix4::Identity),
		_Direction(Vector3(0.0f, 0.0f, 0.0f)),
		_Up(Vector3(0.0f, 0.0f, 0.0f)),
		_Right(Vector3(0.0f, 0.0f, 0.0f)),
		_Projection(Matrix4::Identity),
		_View(Matrix4::Identity)
	{
	}

	RTCamera::~RTCamera()
	{
	}

	void RTCamera::Update()
	{
        /*// Compute the projection matrix
		_Projection = Matrix4::Perspective(_FOV, _AspectRatio, _NearPlane, _FarPlane);

		// Compute the view matrix
		Matrix3 rotation;
		_Orientation.ToRotationMatrix(rotation);

		Matrix3 rotationT = Matrix3::Transpose(rotation);
		Vector3 translation = -rotationT * _Position;

		_View = Matrix4(rotationT, translation);
		_InverseView = Matrix4::Inverse(_View);

		_Up = _Orientation * Vector3::YAxis;
		_Up.Normalize();

		_Right = _Orientation * Vector3::XAxis;
		_Right.Normalize();

		_Direction = _Orientation * -Vector3::ZAxis;
		_Direction.Normalize();*/
	}
	
	void RTCamera::Raycast(const Vector2& position, Ray3& ray)
	{
		switch (_ProjectionType)
		{
		case ProjectionType_Orthographic:
			break;

		case ProjectionType_Perspective:
		default:
			ray.Origin = _InverseView.Transform(_Position);
			ray.Direction = _Right * position.x + _Up * position.y;
			ray.Direction.Normalize();
			break;
		}
	}
}
