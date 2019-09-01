/*=============================================================================
FreeCameraController.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "FreeCameraController.h"
#include "SampleFramework/System/Application.h"

namespace SampleFramework
{
	SE_IMPLEMENT_CLASS(FreeCameraController);

	FW_IMPLEMENT_MESSAGE(FreeCameraController, MovePX);
	FW_IMPLEMENT_MESSAGE(FreeCameraController, MovePY);
	FW_IMPLEMENT_MESSAGE(FreeCameraController, MovePZ);
	FW_IMPLEMENT_MESSAGE(FreeCameraController, MoveNX);
	FW_IMPLEMENT_MESSAGE(FreeCameraController, MoveNY);
	FW_IMPLEMENT_MESSAGE(FreeCameraController, MoveNZ);
	FW_IMPLEMENT_MESSAGE(FreeCameraController, LookPX);
	FW_IMPLEMENT_MESSAGE(FreeCameraController, LookPY);
	FW_IMPLEMENT_MESSAGE(FreeCameraController, LookPZ);
	FW_IMPLEMENT_MESSAGE(FreeCameraController, LookNX);
	FW_IMPLEMENT_MESSAGE(FreeCameraController, LookNY);
	FW_IMPLEMENT_MESSAGE(FreeCameraController, LookNZ);

	FreeCameraController::FreeCameraController() :
		CameraController()
	{
	}

	FreeCameraController::~FreeCameraController()
	{
	}

	void FreeCameraController::HandleMessage(Message& msg)
	{
		super::HandleMessage(msg);

		if (_camera == NULL)
			return;

		real64 elapsed = Application::Current()->GetElapsedTime();
		real32 mSpeed = elapsed * 5.0f * GetMovementSpeed();
		real32 rSpeed = elapsed * Math::Pi * 0.5f * GetRotationSpeed();

		FW_CHECK_MESSAGE(MovePX)
		{
			_camera->Translate(-_camera->GetWorldRight() * mSpeed);
			return;
		}

		FW_CHECK_MESSAGE(MoveNX)
		{
			_camera->Translate(_camera->GetWorldRight() * mSpeed);
			return;
		}

		FW_CHECK_MESSAGE(MovePY)
		{
			_camera->Translate(_camera->GetWorldUp() * mSpeed);
			return;
		}

		FW_CHECK_MESSAGE(MoveNY)
		{
			_camera->Translate(-_camera->GetWorldUp() * mSpeed);
			return;
		}

		FW_CHECK_MESSAGE(MovePZ)
		{
			_camera->Translate(_camera->GetWorldDirection() * mSpeed);
			return;
		}

		FW_CHECK_MESSAGE(MoveNZ)
		{
			_camera->Translate(-_camera->GetWorldDirection() * mSpeed);
			return;
		}

		FW_CHECK_MESSAGE(LookPX)
		{
			_camera->Rotate(Quaternion::CreateFromAxisAngle(_camera->GetWorldUp(), rSpeed));
			return;
		}

		FW_CHECK_MESSAGE(LookNX)
		{
			_camera->Rotate(Quaternion::CreateFromAxisAngle(_camera->GetWorldUp(), -rSpeed));
			return;
		}

		FW_CHECK_MESSAGE(LookPY)
		{
			_camera->Rotate(Quaternion::CreateFromAxisAngle(_camera->GetWorldRight(), rSpeed));
			return;
		}

		FW_CHECK_MESSAGE(LookNY)
		{
			_camera->Rotate(Quaternion::CreateFromAxisAngle(_camera->GetWorldRight(), -rSpeed));
			return;
		}

		FW_CHECK_MESSAGE(LookPZ)
		{
			_camera->Rotate(Quaternion::CreateFromAxisAngle(_camera->GetWorldDirection(), rSpeed));
			return;
		}

		FW_CHECK_MESSAGE(LookNZ)
		{
			_camera->Rotate(Quaternion::CreateFromAxisAngle(_camera->GetWorldDirection(), -rSpeed));
			return;
		}
	}
}
