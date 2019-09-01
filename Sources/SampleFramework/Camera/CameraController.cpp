/*=============================================================================
CameraController.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "CameraController.h"

namespace SampleFramework
{
	SE_IMPLEMENT_CLASS(CameraController);
	SE_IMPLEMENT_REFLECTION(CameraController);

	CameraController::CameraController() :
		Entity(),
		_camera(NULL),
		_movementSpeed(1.0f),
		_rotationSpeed(1.0f)
	{
	}

	CameraController::~CameraController()
	{
	}

	void CameraController::HandleMessage(Message& msg)
	{
	}
}
