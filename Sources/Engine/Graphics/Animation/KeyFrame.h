/*=============================================================================
KeyFrame.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_KEYFRAME_H_
#define _SE_KEYFRAME_H_

#include "Core/Core.h"
#include "Graphics/Common.h"

namespace SonataEngine
{

class AnimationTrack;

/** Interpolation Mode. */
enum InterpolationMode
{
	/// Constant Interpolation.
	InterpolationMode_Constant,

	/// Linear Interpolation.
	InterpolationMode_Linear,

	/// Bezier Spline Interpolation.
	InterpolationMode_Bezier,

	/// TCB Spline Interpolation.
	InterpolationMode_TCB
};

/** Rotation Interpolation Mode. */
enum RotationInterpolationMode
{
	/// Constant Interpolation.
	RotationInterpolationMode_Constant,

	/// Linear Interpolation.
	RotationInterpolationMode_Linear,

	/// Spherical Interpolation.
	RotationInterpolationMode_Spherical
};

/**
	@brief Animation key frame.

	A key frame is an abstract class that contains some data at a given time in an animation track.
*/
class SE_GRAPHICS_EXPORT KeyFrame : public Object
{
protected:
	AnimationTrack* _parentTrack;
	TimeValue _timeValue;
	InterpolationMode _interpolation;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Default constructor. */
	KeyFrame();

	/** Constructor with a time value.
		@param timeValue The initial time value of the key frame.
	*/
	KeyFrame(const TimeValue& timeValue);

	/** Destructor. */
	virtual ~KeyFrame();
	//@}

	/** @name Properties. */
	//@{
	/** Gets the track of the key frame. */
	AnimationTrack* GetParentTrack() const { return _parentTrack; }

	/** Gets the time value of the key frame. */
	const TimeValue& GetTime() const { return _timeValue; }

	/** Sets the time value of the key frame.
		This method will require the parent track to sort the key frames.
	*/
	void SetTime(const TimeValue& value);

	/** Gets the interpolation mode of the key frame. */
	InterpolationMode GetInterpolation() const { return _interpolation; }

	/** Sets the interpolation mode of the key frame. */
	void SetInterpolation(InterpolationMode value) { _interpolation = value; }
	//@}
};

}

#endif 
