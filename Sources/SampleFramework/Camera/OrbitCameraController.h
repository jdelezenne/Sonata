/*=============================================================================
OrbitCameraController.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SFW_ORBITCAMERACONTROLLER_H_
#define _SFW_ORBITCAMERACONTROLLER_H_

#include "SampleFramework/Common.h"
#include "SampleFramework/Messaging/Message.h"
#include "SampleFramework/Camera/CameraController.h"

namespace SampleFramework
{
	/** Look at camera controller. */
	class SE_SAMPLEFRAMEWORK_EXPORT OrbitCameraController : public CameraController
	{
		SE_DECLARE_CLASS(OrbitCameraController, CameraController);

		SE_BEGIN_REFLECTION(OrbitCameraController);
			SE_Field(_target, SceneObject, Public);
			SE_Field(_distance, real32, Public);
			SE_Field(_azimuthAngle, real32, Public);
			SE_Field(_elevationAngle, real32, Public);
		SE_END_REFLECTION(OrbitCameraController);

	protected:
		SceneObject* _target;
		real32 _distance;
		real32 _azimuthAngle;
		real32 _elevationAngle;
		real32 _lastDistance;
		real32 _lastAzimuthAngle;
		real32 _lastElevationAngle;

	public:
		/** @name Constructors / Destructor. */
		//@{
		/** Constructor. */
		OrbitCameraController();

		/** Destructor. */
		virtual ~OrbitCameraController();
		//@}

		SceneObject* GetTarget() const { return _target; }
		void SetTarget(SceneObject* value) { _target = value; }

		real32 GetDistance() const { return _distance; }
		void SetDistance(real32 value);

		real32 GetAzimuthAngle() const { return _azimuthAngle; }
		void SetAzimuthAngle(real32 value) { _azimuthAngle = value; }

		real32 GetElevationAngle() const { return _elevationAngle; }
		void SetElevationAngle(real32 value) { _elevationAngle = value; }

		virtual void HandleMessage(Message& msg);
	};
}

#endif

/*=============================================================================
09-02-2006 Julien Delezenne
	Created.
=============================================================================*/
