/*=============================================================================
Camera.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Camera.h"
#include "Graphics/Scene/Scene.h"

namespace SonataEngine
{

SE_IMPLEMENT_CLASS(Camera);
SE_IMPLEMENT_REFLECTION(Camera);

Camera::Camera() :
	SceneObject(),
	_scene(NULL),
	_projectionType(ProjectionType_Perspective),
	_fov(45.0f),
	_aspectRatio(1.0f),
	_leftPlane(0),
	_topPlane(0),
	_rightPlane(1),
	_bottomPlane(1),
	_nearPlane(1.0f),
	_farPlane(10000.0f),
	_needProjectionUpdate(true),
	_projection(Matrix4::Identity),
	_view(Matrix4::Identity)
{
}

Camera::~Camera()
{
}

void Camera::SetScene(Scene* value)
{
	_scene = value;
}

void Camera::SetViewport(const Viewport& value)
{
	_viewport = value;
}

void Camera::SetProjectionType(ProjectionType value)
{
	_projectionType = value;
	_NotifyProjectionUpdate();
}

void Camera::SetFOV(real32 value)
{
	_fov = value;
	_NotifyProjectionUpdate();
}

void Camera::SetAspectRatio(real32 value)
{
	_aspectRatio = Math::Max(0.0f, value);
	_NotifyProjectionUpdate();
}

void Camera::SetLeftPlane(int32 value)
{
	_leftPlane = value;
	_NotifyProjectionUpdate();
}

void Camera::SetTopPlane(int32 value)
{
	_topPlane = value;
	_NotifyProjectionUpdate();
}

void Camera::SetRightPlane(int32 value)
{
	_rightPlane = value;
	_NotifyProjectionUpdate();
}

void Camera::SetBottomPlane(int32 value)
{
	_bottomPlane = value;
	_NotifyProjectionUpdate();
}

void Camera::SetNearPlane(real32 value)
{
	_nearPlane = value;
	_NotifyProjectionUpdate();
}

void Camera::SetFarPlane(real32 value)
{
	_farPlane = value;
	_NotifyProjectionUpdate();
}

void Camera::SetPerspective(real32 fov, real32 aspect, real32 nearPlane, real32 farPlane)
{
	SetProjectionType(ProjectionType_Perspective);
	SetFOV(fov);
	SetAspectRatio(aspect);
	SetNearPlane(nearPlane);
	SetFarPlane(farPlane);
}

void Camera::SetOrthographic(int32 leftPlane, int32 topPlane, int32 rightPlane, int32 bottomPlane, real32 nearPlane, real32 farPlane)
{
	SetProjectionType(ProjectionType_Orthographic);
	SetLeftPlane(leftPlane);
	SetTopPlane(topPlane);
	SetRightPlane(rightPlane);
	SetBottomPlane(bottomPlane);
	SetNearPlane(nearPlane);
	SetFarPlane(farPlane);
}

void Camera::SetDirection(const Vector3& value)
{
	_UpdateLocalTransform();

	// The camera looks down the z-axis
	Vector3 dirNorm = -value;
	dirNorm.Normalize();

	// Get axes from current quaternion
	Vector3 xaxis, yaxis, zAxis;
	_localOrientation.ToAxes(xaxis, yaxis, zAxis);

	Quaternion rotation;
	if ((zAxis+dirNorm).LengthSquared() <  Math::Epsilon) 
	{
		// 180 degree turn (infinite possible rotation axes), default to yaw
		rotation = Quaternion::CreateFromAxisAngle(yaxis, Math::Pi);
	}
	else
	{
		// Derive shortest arc to new direction
		//rotation = zAxis.GetRotationTo(dirNorm);

		// Based on Stan Melax's article in Game Programming Gems
		Quaternion q;
		// Copy, since cannot modify local
		Vector3 v0 = zAxis;
		Vector3 v1 = dirNorm;
		v0.Normalize();
		v1.Normalize();

		Vector3 c = v0.Cross(v1);

		// NB if the crossProduct approaches zero, we get unstable because ANY axis will do
		// when v0 == -v1
		real d = v0.Dot(v1);
		// If dot == 1, vectors are the same
		if (d >= 1.0f)
		{
			rotation = Quaternion::Identity;
		}
		else
		{
			real s = sqrt((1+d)*2);
			real invs = 1 / s;

			q.X = c.X * invs;
			q.Y = c.Y * invs;
			q.Z = c.Z * invs;
			q.W = s * 0.5;
			rotation = q;
		}
	}

	Rotate(rotation);
}

void Camera::LookAt(const Vector3& value)
{
	SetDirection(value - _localPosition);
}

const Matrix4& Camera::GetProjection()
{
	_UpdateProjection();
	return _projection;
}

const Matrix4& Camera::GetView()
{
	_UpdateWorldTransform();
	return _view;
}

const Frustum& Camera::GetFrustum()
{
	_UpdateProjection();
	return _frustum;
}

void Camera::Update(const TimeValue& timeValue)
{
}

void Camera::_NotifyProjectionUpdate()
{
	_needProjectionUpdate = true;
}

void Camera::_UpdateWorldTransform()
{
	super::_UpdateWorldTransform();

	_view = Matrix4::Invert(_worldTransform);
}

void Camera::_UpdateProjection()
{
	if (!_needProjectionUpdate)
	{
		return;
	}

	if (_projectionType == ProjectionType_Perspective)
	{
		_projection = Matrix4::CreatePerspective(_fov, _aspectRatio, _nearPlane, _farPlane);
	}
	else if (_projectionType == ProjectionType_Orthographic)
	{
		_projection = Matrix4::CreateOrthographicOffCenter(_leftPlane, _rightPlane,
			_bottomPlane, _topPlane, _nearPlane, _farPlane);
	}

	_frustum.SetMatrix(_projection);

	_needProjectionUpdate = false;
}

}
