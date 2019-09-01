/*=============================================================================
AnimationSequence.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "AnimationSequence.h"

namespace SonataEngine
{

AnimationSequence::AnimationSequence() :
	NamedObject(),
	_isEnabled(true),
	_playMode(PlayMode_Forwards),
	_playSpeed(1.0f),
	_isLooping(false),
	_isPlaying(false)
{
}

void AnimationSequence::SetStartTime(const TimeValue& value)
{
	_startTime = value;
	_length = _endTime - _startTime;
}

void AnimationSequence::SetEndTime(const TimeValue& value)
{
	_endTime = value;
	_length = _endTime - _startTime;
}

AnimationSequence::~AnimationSequence()
{
	AnimationTrackList::Iterator it = _tracks.GetIterator();
	while (it.Next())
	{
		delete it.Current();
	}
}

int AnimationSequence::GetAnimationTrackCount() const
{
	return _tracks.Count();
}

bool AnimationSequence::AddAnimationTrack(AnimationTrack* value)
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return false;
	}
	else
	{
		_tracks.Add(value);
		return true;
	}
}

bool AnimationSequence::RemoveAnimationTrack(AnimationTrack* value)
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return false;
	}
	else
	{
		_tracks.Remove(value);
		return true;
	}
}

void AnimationSequence::RemoveAllAnimationTracks()
{
	_tracks.Clear();
}

int AnimationSequence::GetAnimationTrackIndex(AnimationTrack* value) const
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return -1;
	}
	else
	{
		return _tracks.IndexOf(value);
	}
}

AnimationTrack* AnimationSequence::GetAnimationTrackByIndex(int index) const
{
	return _tracks[index];
}

AnimationTrack* AnimationSequence::GetAnimationTrackByName(const String& name) const
{
	AnimationTrackList::Iterator it = _tracks.GetIterator();
	while (it.Next())
	{
		if (it.Current()->GetName() == name)
		{
			return it.Current();
		}
	}

	return NULL;
}

void AnimationSequence::Start()
{
	if (!_isEnabled || _isPlaying)
	{
		return;
	}

	_currentTime = _startTime;
	_lastTime = TimeValue(0.0);
	_isPlaying = true;
}

void AnimationSequence::Stop()
{
	if (!_isEnabled || !_isPlaying)
	{
		return;
	}

	_isPlaying = false;
}

void AnimationSequence::Update(const TimeValue& timeValue)
{
	if (!_isPlaying)
	{
		return;
	}

	TimeValue newTime = timeValue;
	_currentTime += (timeValue - _lastTime);
	_lastTime = timeValue;

	if (_currentTime > _endTime)
	{
		if (_isLooping)
		{
			_currentTime = _startTime;
		}
		else
		{
			_currentTime = _endTime;
			_isPlaying = false;
		}
	}

	// Updates the tracks.
	AnimationTrackList::Iterator it = _tracks.GetIterator();
	while (it.Next())
	{
		AnimationTrack* track = it.Current();
		if (track->IsEnabled())
		{
			track->Update(_currentTime);
		}
	}
}

}
