/*=============================================================================
LookAtCameraController.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "LookAtCameraController.h"
#include "SampleFramework/System/Application.h"

namespace SampleFramework
{
	real32 DegAngDiffAndRange(real32& newVal, real32 baseVal, real32 lastVal)
	{
		real32 temp;

		// First off, need to range the new value, so it matches the lastVal range. Assume that
		// they cannot be out be a linear value of greater than 180 degrees.
		temp = lastVal - newVal;

		if (Math::Abs(temp) > 180)
		{
			int32 turns;

			// Out of range, so adjust it...
			if (temp > 0)
			{
				turns = static_cast<int32>((temp + 180.0f) / 360.0f);
			}
			else
			{
				turns = static_cast<int32>((temp - 180.0f) / 360.0f);
			}

			newVal += 360.0f * static_cast<real32>(turns);
			temp = lastVal - newVal;
		}

		// Return the difference
		return (baseVal - newVal);
	}

	real32 RadAngDiff(real32 a0, real32 a1)
	{
		real32 a2;

		a2 = a1 - a0;
		while (a2 < 0.0f) a2 += (Math::Pi*2.0f);
		while (a2 >= (Math::Pi*2.0f)) a2 -= (Math::Pi*2.0f);
		if (a2 >= Math::Pi) a2 -= (Math::Pi*2.0f);

		return a2;
	}

	SE_IMPLEMENT_CLASS(LookAtCameraController);
	SE_IMPLEMENT_REFLECTION(LookAtCameraController);

	LookAtCameraController::LookAtCameraController() :
		CameraController(),
		_target(NULL),
		_distance(100.0f),
		_elevation(10.0f),
		_angle(0.0f),
		_lastAngle(0.0f)
	{
	}

	LookAtCameraController::~LookAtCameraController()
	{
	}

	void LookAtCameraController::HandleMessage(Message& msg)
	{
		super::HandleMessage(msg);

		if (_camera == NULL || _target == NULL)
			return;

		real64 elapsed = Application::Current()->GetElapsedTime();

		FW_CHECK_MESSAGE(Update)
		{
			Quaternion invRot = Quaternion::Invert(_target->GetWorldOrientation());
			Vector3 v = _camera->GetWorldPosition() - _target->GetWorldPosition();
			v = invRot.Rotate(v);

			real32 distance = Vector2(v.X, v.Z).Length(); // Distance X/Z only
			distance += (_distance - distance) * 0.1f;
			//Clamp

			real32 elevation = v.Y;
			elevation += (_elevation - elevation) * 0.1f;
			//Clamp

			real32 angle =  Math::ToDegrees(static_cast<real32>(Math::Atan2(v.X, v.Z)));

			// Head towards preferred angle
			real32 d_angle = DegAngDiffAndRange(angle, _angle, _lastAngle);

			angle += d_angle * 0.025f;
			_lastAngle = angle;

			// Generate a position vector relative to the target
			v.X = distance * static_cast<real32>(Math::Sin(Math::ToRadians(angle)));
			v.Y = elevation;
			v.Z = distance * static_cast<real32>(Math::Cos(Math::ToRadians(angle)));

			// Apply the target rotation
			v = _target->GetWorldOrientation().Rotate(v);

			real32 y_angle = Math::Pi + static_cast<real32>(Math::Atan2(v.X, v.Z));

			real32 temp = Vector2(v.X, v.Z).Length();

			real32 x_angle = static_cast<real32>(Math::Atan2(v.Y, temp));

			// Add back to original position, to create our new camera position
			v += _target->GetWorldPosition();
			Quaternion qX = Quaternion::CreateFromAxisAngle(Vector3::UnitX, x_angle);
			Quaternion qY = Quaternion::CreateFromAxisAngle(Vector3::UnitY, y_angle);

			_camera->SetLocalPosition(v);
			//_camera->SetLocalOrientation(qX * qY);

			v = Vector3(0, 0, -distance);
			v = _target->GetWorldPosition() + _target->GetWorldOrientation().Rotate(v);
			v.Y += _elevation;
			//_camera->SetLocalPosition(v);
			Quaternion qRot = Quaternion::CreateFromRotationMatrix(
				Matrix3::CreateFromMatrix4(Matrix4::CreateLookAt(
				_camera->GetWorldPosition(), _target->GetWorldPosition(), Vector3::UnitY)));
			_camera->SetLocalOrientation(Quaternion::Invert(qRot));

			return;
		}
	}
}
