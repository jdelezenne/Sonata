/*=============================================================================
AnimationController.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ANIMATIONCONTROLLER_H_
#define _SE_ANIMATIONCONTROLLER_H_

#include "Core/Core.h"
#include "Graphics/Common.h"
#include "Graphics/Animation/AnimationSequence.h"

namespace SonataEngine
{

/**
	@brief Animation controller.

	An animation controller controls an animation sequence.
*/
class SE_GRAPHICS_EXPORT AnimationController : public RefObject
{
protected:
	AnimationSequence* _animationSequence;
	TimeValue _currentTime;
	TimeValue _globalStartTime;
	TimeValue _globalEndTime;
	TimeValue _globalLength;
	TimeValue _playStartTime;
	TimeValue _playEndTime;
	TimeValue _playLength;
	PlayMode _playMode;
	real32 _playSpeed;
	bool _isLooping;
	bool _isPlaying;

public:
	/** Constructor / Destructor. */
	//@{
	AnimationController();
	virtual ~AnimationController();
	//@}

	/** Properties. */
	//@{
	AnimationSequence* GetAnimationSequence() const { return _animationSequence; }
	void SetAnimationSequence(AnimationSequence* value);

	const TimeValue& GetCurrentTime() const { return _currentTime; }
	void SetCurrentTime(const TimeValue& value) { _currentTime = value; }

	const TimeValue& GetGlobalStartTime() const { return _globalStartTime; }
	void SetGlobalStartTime(const TimeValue& value);

	const TimeValue& GetGlobalEndTime() const { return _globalEndTime; }
	void SetGlobalEndTime(const TimeValue& value);

	const TimeValue& GetGlobalLength() const { return _globalLength; }

	const TimeValue& GetPlayStartTime() const { return _playStartTime; }
	void SetPlayStartTime(const TimeValue& value);

	const TimeValue& GetPlayEndTime() const { return _playEndTime; }
	void SetPlayEndTime(const TimeValue& value);

	const TimeValue& GetPlayLength() const { return _playLength; }

	/** Gets or sets the play mode of the animation. */
	PlayMode GetPlayMode() const { return _playMode; }
	void SetPlayMode(PlayMode value) { _playMode = value; }

	/** Gets or sets a the speed of the animation. */
	real32 GetPlaySpeed() const { return _playSpeed; }
	void SetPlaySpeed(real32 value) { _playSpeed = value; }

	/** Gets or sets a value indicating whether the animation is looping. */
	bool IsLooping() const { return _isLooping; }
	void SetLooping(bool value) { _isLooping = value; }

	/** Gets a value indicating whether the animation is playing. */
	bool IsPlaying() const { return _isPlaying; }
	//@}

	/** Operations. */
	//@{
	/** Sets the start time and end time of the animation. */
	void SetStartEndTimes(const TimeValue& startTime, const TimeValue& endTime);

	/** Sets the length of the animation by setting the start time to zero and the end time to the specified length. */
	void SetLength(const TimeValue& length);

	/** Resets the animation time to the start play time of the animation. */
	void ResetTime();

	/** Updates the active animation with the specified time value. */
	void Update(const TimeValue& timeValue);
	//@}

protected:
};

}

#endif 
