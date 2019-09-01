/*=============================================================================
FirstPersonCameraController.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SFW_FIRSTPERSONCAMERACONTROLLER_H_
#define _SFW_FIRSTPERSONCAMERACONTROLLER_H_

#include "SampleFramework/Common.h"
#include "SampleFramework/Messaging/Message.h"
#include "SampleFramework/Camera/CameraController.h"

namespace SampleFramework
{
	/** First-person camera controller. */
	class SE_SAMPLEFRAMEWORK_EXPORT FirstPersonCameraController : public CameraController
	{
		SE_DECLARE_CLASS(FirstPersonCameraController, CameraController);

	public:
		FW_DECLARE_MESSAGE(MoveForward);
		FW_DECLARE_MESSAGE(MoveBackward);
		FW_DECLARE_MESSAGE(MoveLeft);
		FW_DECLARE_MESSAGE(MoveRight);
		FW_DECLARE_MESSAGE(MoveUp);
		FW_DECLARE_MESSAGE(MoveDown);
		FW_DECLARE_MESSAGE(LookUp);
		FW_DECLARE_MESSAGE(LookDown);
		FW_DECLARE_MESSAGE(LookLeft);
		FW_DECLARE_MESSAGE(LookRight);

		/** @name Constructors / Destructor. */
		//@{
		/** Constructor. */
		FirstPersonCameraController();

		/** Destructor. */
		virtual ~FirstPersonCameraController();
		//@}

		virtual void HandleMessage(Message& msg);
	};
}

#endif
