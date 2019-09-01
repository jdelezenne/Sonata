/*=============================================================================
OrbitCameraController.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "OrbitCameraController.h"
#include "SampleFramework/System/Application.h"

namespace SampleFramework
{
	SE_IMPLEMENT_CLASS(OrbitCameraController);
	SE_IMPLEMENT_REFLECTION(OrbitCameraController);

	OrbitCameraController::OrbitCameraController() :
		CameraController(),
		_target(NULL),
		_distance(100.0f),
		_azimuthAngle(10.0f),
		_elevationAngle(0.0f)
	{
		_lastDistance = 0.0f;
		_lastAzimuthAngle = 0.0f;
		_lastElevationAngle = 0.0f;
	}

	OrbitCameraController::~OrbitCameraController()
	{
	}

	void OrbitCameraController::SetDistance(real32 value)
	{
		_distance = Math::Max(0.1f, value);
	}

	void OrbitCameraController::HandleMessage(Message& msg)
	{
		super::HandleMessage(msg);

		if (_camera == NULL || _target == NULL)
			return;

		real64 elapsed = Application::Current()->GetElapsedTime();

		FW_CHECK_MESSAGE(Update)
		{
			_camera->Translate(_camera->GetLocalOrientation().Rotate(Vector3::Forward) * (_lastDistance - _distance));
			_lastDistance = _distance;

            real32 azimuthDelta = _lastAzimuthAngle - _azimuthAngle;
            real32 elevationDelta = _lastElevationAngle - _elevationAngle;

			Matrix4 viewTransform = Matrix4::Invert(_camera->GetView());
            Vector3 center = Vector3::Zero;
			viewTransform = Matrix4::CreateFromAxisAngle(
				_camera->GetWorldRight(), Math::ToRadians(elevationDelta)) * viewTransform;
			viewTransform = Matrix4::CreateFromAxisAngle(
				_camera->GetWorldUp(), Math::ToRadians(azimuthDelta)) * viewTransform;

            Vector3 initialPosition = _camera->GetLocalPosition();
            _camera->SetLocalPosition(Vector3(viewTransform.M03, viewTransform.M13, viewTransform.M23));
            Vector3 upDirection = Vector3::Transform(Vector3::UnitY, viewTransform);
            upDirection.Normalize();
            Vector3 lookDirection = _camera->GetWorldDirection();
            lookDirection = (center - _camera->GetLocalPosition());
			_camera->SetDirection(Vector3::Normalize(lookDirection));

			_lastAzimuthAngle = _azimuthAngle;
			_lastElevationAngle = _elevationAngle;

			return;
		}
	}
}
