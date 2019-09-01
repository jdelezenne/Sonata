/*=============================================================================
KeyFrame.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "KeyFrame.h"
#include "Graphics/Animation/AnimationTrack.h"

namespace SonataEngine
{

KeyFrame::KeyFrame() :
	Object(),
	_parentTrack(NULL),
	_interpolation(InterpolationMode_Linear)
{
}

KeyFrame::KeyFrame(const TimeValue& timeValue) :
	Object(),
	_parentTrack(NULL),
	_timeValue(timeValue),
	_interpolation(InterpolationMode_Linear)
{
}

KeyFrame::~KeyFrame()
{
}

void KeyFrame::SetTime(const TimeValue& value)
{
	if (_parentTrack != NULL)
	{
		_parentTrack->SetKeyFrameTime(this, value);
	}
}

}
