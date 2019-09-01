/*=============================================================================
FreeCameraController.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SFW_FREECAMERACONTROLLER_H_
#define _SFW_FREECAMERACONTROLLER_H_

#include "SampleFramework/Common.h"
#include "SampleFramework/Messaging/Message.h"
#include "SampleFramework/Camera/CameraController.h"

namespace SampleFramework
{
	/** Free camera controller. */
	class SE_SAMPLEFRAMEWORK_EXPORT FreeCameraController : public CameraController
	{
		SE_DECLARE_CLASS(FreeCameraController, CameraController);

	public:
		FW_DECLARE_MESSAGE(MovePX);
		FW_DECLARE_MESSAGE(MovePY);
		FW_DECLARE_MESSAGE(MovePZ);
		FW_DECLARE_MESSAGE(MoveNX);
		FW_DECLARE_MESSAGE(MoveNY);
		FW_DECLARE_MESSAGE(MoveNZ);
		FW_DECLARE_MESSAGE(LookPX);
		FW_DECLARE_MESSAGE(LookPY);
		FW_DECLARE_MESSAGE(LookPZ);
		FW_DECLARE_MESSAGE(LookNX);
		FW_DECLARE_MESSAGE(LookNY);
		FW_DECLARE_MESSAGE(LookNZ);

		/** @name Constructors / Destructor. */
		//@{
		/** Constructor. */
		FreeCameraController();

		/** Destructor. */
		virtual ~FreeCameraController();
		//@}

		virtual void HandleMessage(Message& msg);
	};
}

#endif

/*=============================================================================
09-02-2006 Julien Delezenne
	Created.
=============================================================================*/
