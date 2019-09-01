/*=============================================================================
AnimationTrack.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "AnimationTrack.h"

namespace SonataEngine
{

AnimationTrack::AnimationTrack() :
	NamedObject(),
	_isEnabled(true)
{
}

AnimationTrack::~AnimationTrack()
{
	KeyFrameList::Iterator it = _keyFrames.GetIterator();
	while (it.Next())
	{
		delete it.Current();
	}
}

void AnimationTrack::SetStartTime(const TimeValue& value)
{
	_startTime = value;
	_length = _endTime - _startTime;
}

void AnimationTrack::SetEndTime(const TimeValue& value)
{
	_endTime = value;
	_length = _endTime - _startTime;
}

int AnimationTrack::GetKeyFrameCount() const
{
	return _keyFrames.Count();
}

KeyFrame* AnimationTrack::AddKeyFrame(const TimeValue& timeValue)
{
	KeyFrame* keyFrame = NULL;//GetKeyFrameAtTime(timeValue);
	if (keyFrame != NULL)
	{
		return keyFrame;
	}

	keyFrame = _CreateKeyFrame(timeValue);

	if (_keyFrames.Count() == 0)
	{
		// There are no keyframes, just add the first one
		_keyFrames.AddLast(keyFrame);
		return keyFrame;
	}
	else
	{
#if 0
		const TimeValue& timeValue = keyFrame->GetTime();
		KeyFrameList::Node node = _keyFrames.First();
		while (node != _keyFrames.Last())
		{
			KeyFrame* current = node.GetValue();
			if (current->GetTime() > timeValue)
			{
				// The time value is lower than the keyframe, insert at this index
				_keyFrames.AddAfter(node, keyFrame);
				return keyFrame;
			}
			node = node.Next();
		}
#endif
		// The time value is higher than all the first keyframe, insert at the end
		_keyFrames.AddLast(keyFrame);
		return keyFrame;
	}
}

void AnimationTrack::RemoveKeyFrame(KeyFrame* value)
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return;
	}

	_keyFrames.Remove(value);
}

void AnimationTrack::RemoveAllKeyFrames()
{
	_keyFrames.Clear();
}

int AnimationTrack::GetKeyFrameIndex(KeyFrame* value) const
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return -1;
	}
	else
	{
		return 0;//_keyFrames.IndexOf(value);
	}
}

KeyFrame* AnimationTrack::GetKeyFrameByIndex(int index) const
{
	return NULL;//_keyFrames[index];
}

void AnimationTrack::SetStartEndTimes(const TimeValue& startTime, const TimeValue& endTime)
{
	_startTime = startTime;
	_endTime = endTime;
}

void AnimationTrack::SetLength(const TimeValue& length)
{
	_startTime = TimeValue(0.0);
	_endTime = length;
}

KeyFrame* AnimationTrack::_CreateKeyFrame(const TimeValue& timeValue)
{
	return new KeyFrame(timeValue);
}

KeyFrame* AnimationTrack::GetKeyFrameAtTime(const TimeValue& timeValue) const
{
	KeyFrameList::Iterator it = _keyFrames.GetIterator();
	while (it.Next())
	{
		if (it.Current()->GetTime() == timeValue)
		{
			return it.Current();
		}
	}

	return NULL;
}

TimeValue AnimationTrack::GetKeyFramesAtTime(const TimeValue& timeValue, KeyFrame** keyA, KeyFrame** keyB) const
{
	if (keyA == NULL)
	{
		SEthrow(ArgumentNullException("keyA"));
		return TimeValue(0.0);
	}
	if (keyB == NULL)
	{
		SEthrow(ArgumentNullException("keyB"));
		return TimeValue(0.0);
	}

	KeyFrame* previous = NULL;
	KeyFrameList::Iterator it = _keyFrames.GetIterator();
	while (it.Next())
	{
		KeyFrame* current = it.Current();
		if (current->GetTime() > timeValue)
		{
			*keyA = (previous != NULL) ? previous : current;
			*keyB = current;
			return ((*keyB)->GetTime() - (*keyA)->GetTime());
		}

		previous = current;
	}

	*keyA = previous;
	*keyB = previous;

	return TimeValue(0.0);
}

void AnimationTrack::SetKeyFrameTime(KeyFrame* keyFrame, const TimeValue& timeValue)
{
	if (keyFrame == NULL)
	{
		throw ArgumentNullException("keyFrame");
		return;
	}
	if (keyFrame->GetParentTrack() != this)
	{
		return;
	}
	if (keyFrame->GetTime() == timeValue)
	{
		return;
	}

	//TODO
	/*int index = 0;
	KeyFrameList::Iterator it = _keyFrames.GetIterator();
	while (it.Next())
	{
		// A key frame at that time already exists
		if (it.Current()->GetTime() == timeValue)
		{
			return;
		}
		// This key frame has a greater time valuer, move the key frame before it
		if (it.Current()->GetTime() > timeValue)
		{
			_keyFrames.Remove(keyFrame);
			_keyFrames.Insert(index, keyFrame);
			return;
		}
		index++;
	}
	_keyFrames.Remove(keyFrame);
	_keyFrames.AddLast(keyFrame);*/
}

void AnimationTrack::Update(const TimeValue& timeValue)
{
}

}
