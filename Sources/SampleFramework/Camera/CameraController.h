/*=============================================================================
CameraController.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SFW_CAMERACONTROLLER_H_
#define _SFW_CAMERACONTROLLER_H_

#include "SampleFramework/Common.h"
#include "SampleFramework/Entity/Entity.h"

namespace SampleFramework
{
	/**
		@brief Camera controller.

		Base class for camera controllers.

		@todo ArcBall
	*/
	class SE_SAMPLEFRAMEWORK_EXPORT CameraController : public Entity
	{
		SE_DECLARE_CLASS(CameraController, Entity);

		SE_BEGIN_REFLECTION(CameraController);
			SE_Field(_camera, Camera, Public);
			SE_Field(_movementSpeed, real32, Public);
			SE_Field(_rotationSpeed, real32, Public);
		SE_END_REFLECTION(CameraController);

	protected:
		Camera* _camera;
		real32 _movementSpeed;
		real32 _rotationSpeed;

	public:
		/** @name Constructors / Destructor. */
		//@{
		/** Constructor. */
		CameraController();

		/** Destructor. */
		virtual ~CameraController();
		//@}

		Camera* GetCamera() const { return _camera; }
		void SetCamera(Camera* value) { _camera = value; }

		real32 GetMovementSpeed() const { return _movementSpeed; }
		void SetMovementSpeed(real32 value) { _movementSpeed = value; }

		real32 GetRotationSpeed() const { return _rotationSpeed; }
		void SetRotationSpeed(real32 value) { _rotationSpeed = value; }

		virtual void HandleMessage(Message& msg);
	};
}

#endif

/*=============================================================================
09-02-2006 Julien Delezenne
	Created.
=============================================================================*/
