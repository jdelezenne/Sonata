/*=============================================================================
FirstPersonCameraController.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "FirstPersonCameraController.h"
#include "SampleFramework/System/Application.h"

namespace SampleFramework
{
	SE_IMPLEMENT_CLASS(FirstPersonCameraController);

	FW_IMPLEMENT_MESSAGE(FirstPersonCameraController, MoveForward);
	FW_IMPLEMENT_MESSAGE(FirstPersonCameraController, MoveBackward);
	FW_IMPLEMENT_MESSAGE(FirstPersonCameraController, MoveLeft);
	FW_IMPLEMENT_MESSAGE(FirstPersonCameraController, MoveRight);
	FW_IMPLEMENT_MESSAGE(FirstPersonCameraController, MoveUp);
	FW_IMPLEMENT_MESSAGE(FirstPersonCameraController, MoveDown);
	FW_IMPLEMENT_MESSAGE(FirstPersonCameraController, LookUp);
	FW_IMPLEMENT_MESSAGE(FirstPersonCameraController, LookDown);
	FW_IMPLEMENT_MESSAGE(FirstPersonCameraController, LookLeft);
	FW_IMPLEMENT_MESSAGE(FirstPersonCameraController, LookRight);

	FirstPersonCameraController::FirstPersonCameraController() :
		CameraController()
	{
	}

	FirstPersonCameraController::~FirstPersonCameraController()
	{
	}

	void FirstPersonCameraController::HandleMessage(Message& msg)
	{
		super::HandleMessage(msg);

		if (_camera == NULL)
			return;

		real64 elapsed = Application::Current()->GetElapsedTime();
		real32 mSpeed = elapsed * 50.0f * GetMovementSpeed();
		real32 rSpeed = elapsed * Math::PiByTwo * GetRotationSpeed();

		FW_CHECK_MESSAGE(MoveForward)
		{
			_camera->Translate(_camera->GetWorldDirection() * mSpeed);
			return;
		}

		FW_CHECK_MESSAGE(MoveBackward)
		{
			_camera->Translate(-_camera->GetWorldDirection() * mSpeed);
			return;
		}
		
		FW_CHECK_MESSAGE(MoveLeft)
		{
			_camera->Translate(-_camera->GetWorldRight() * mSpeed);
			return;
		}
		
		FW_CHECK_MESSAGE(MoveRight)
		{
			_camera->Translate(_camera->GetWorldRight() * mSpeed);
			return;
		}

		FW_CHECK_MESSAGE(MoveUp)
		{
			_camera->Translate(_camera->GetWorldUp() * mSpeed);
			return;
		}

		FW_CHECK_MESSAGE(MoveDown)
		{
			_camera->Translate(-_camera->GetWorldUp() * mSpeed);
			return;
		}

		FW_CHECK_MESSAGE(LookUp)
		{
			_camera->Rotate(Quaternion::CreateFromAxisAngle(_camera->GetWorldRight(), rSpeed));
			return;
		}

		FW_CHECK_MESSAGE(LookDown)
		{
			_camera->Rotate(Quaternion::CreateFromAxisAngle(_camera->GetWorldRight(), -rSpeed));
			return;
		}

		FW_CHECK_MESSAGE(LookLeft)
		{
			_camera->Rotate(Quaternion::CreateFromAxisAngle(Vector3::UnitY, rSpeed));
			return;
		}

		FW_CHECK_MESSAGE(LookRight)
		{
			_camera->Rotate(Quaternion::CreateFromAxisAngle(Vector3::UnitY, -rSpeed));
			return;
		}
	}
}
