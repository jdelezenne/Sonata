/*=============================================================================
LookAtCameraController.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SFW_LOOKATCAMERACONTROLLER_H_
#define _SFW_LOOKATCAMERACONTROLLER_H_

#include "SampleFramework/Common.h"
#include "SampleFramework/Messaging/Message.h"
#include "SampleFramework/Camera/CameraController.h"

namespace SampleFramework
{
	/** Look at camera controller. */
	class SE_SAMPLEFRAMEWORK_EXPORT LookAtCameraController : public CameraController
	{
		SE_DECLARE_CLASS(LookAtCameraController, CameraController);

		SE_BEGIN_REFLECTION(LookAtCameraController);
			SE_Field(_target, SceneObject, Public);
			SE_Field(_distance, real32, Public);
			SE_Field(_elevation, real32, Public);
			SE_Field(_angle, real32, Public);
		SE_END_REFLECTION(LookAtCameraController);

	protected:
		SceneObject* _target;
		real32 _distance;
		real32 _elevation;
		real32 _angle;
		real32 _lastAngle;

	public:
		/** @name Constructors / Destructor. */
		//@{
		/** Constructor. */
		LookAtCameraController();

		/** Destructor. */
		virtual ~LookAtCameraController();
		//@}

		SceneObject* GetTarget() const { return _target; }
		void SetTarget(SceneObject* value) { _target = value; }

		real32 GetDistance() const { return _distance; }
		void SetDistance(real32 value) { _distance = value; }

		real32 GetElevation() const { return _elevation; }
		void SetElevation(real32 value) { _elevation = value; }

		real32 GetAngle() const { return _angle; }
		void SetAngle(real32 value) { _angle = value; }

		virtual void HandleMessage(Message& msg);
	};
}

#endif

/*=============================================================================
09-02-2006 Julien Delezenne
	Created.
=============================================================================*/
