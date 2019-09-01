/*=============================================================================
AnimationController.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "AnimationController.h"

namespace SonataEngine
{

AnimationController::AnimationController() :
	_animationSequence(NULL),
	_playMode(PlayMode_Forwards),
	_playSpeed(1.0f),
	_isLooping(false),
	_isPlaying(false)
{
}

AnimationController::~AnimationController()
{
}

void AnimationController::SetAnimationSequence(AnimationSequence* value)
{
	_animationSequence = value;
	_currentTime = TimeValue(0.0);
	_isPlaying = false;
}

void AnimationController::SetGlobalStartTime(const TimeValue& value)
{
	_globalStartTime = value;
	_globalLength = _globalEndTime - _globalStartTime;
}

void AnimationController::SetGlobalEndTime(const TimeValue& value)
{
	_globalEndTime = value;
	_globalLength = _globalEndTime - _globalStartTime;
}

void AnimationController::SetPlayStartTime(const TimeValue& value)
{
	_playStartTime = value;
	_playLength = _playEndTime - _playStartTime;
}

void AnimationController::SetPlayEndTime(const TimeValue& value)
{
	_playEndTime = value;
	_playLength = _playEndTime - _playStartTime;
}

void AnimationController::SetStartEndTimes(const TimeValue& startTime, const TimeValue& endTime)
{
	_globalStartTime = _globalEndTime = startTime;
	_playEndTime = _playEndTime = endTime;
}

void AnimationController::SetLength(const TimeValue& length)
{
	_globalStartTime = _playStartTime = TimeValue(0.0);
	_globalEndTime = _playEndTime = length;
}

void AnimationController::ResetTime()
{
	_currentTime = _playStartTime;
}

void AnimationController::Update(const TimeValue& timeValue)
{
	if (!_isPlaying)
	{
		return;
	}

	TimeValue newTime = timeValue;

	// Updates the active sequence.
	if (_animationSequence != NULL && _animationSequence->IsEnabled())
	{
		_animationSequence->Update(newTime);
	}
}

}
