/*=============================================================================
EventTimer.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "EventTimer.h"
#include "SampleFramework/System/Application.h"

namespace SampleFramework
{

EventTimer::EventTimer(real64 interval) :
	Object()
{
	_isEnabled = false;
	_autoReset = true;
	_interval = interval;
	_lastTime = 0.0;
}

EventTimer::~EventTimer()
{
}

void EventTimer::Update()
{
	if (!_isEnabled)
		return;

	real64 time = TimeValue::GetTime();

	if ((time - _lastTime) > _interval)
	{
		ElapsedEventArgs e(time);
		Elapsed.Invoke(this, e);
		_lastTime = time;

		if (_autoReset)
		{
			_isEnabled = false;
		}
	}
}

void EventTimer::SetEnabled(bool value)
{
	_isEnabled = value;
	/*if (value)
	{
		Application::Instance()->AddTimer(this);
	}
	else
	{
		Application::Instance()->RemoveTimer(this);
	}*/
}

void EventTimer::Start()
{
	if (!_isEnabled)
	{
		_lastTime = TimeValue::GetTime();
	}
	_isEnabled = true;
}

void EventTimer::Stop()
{
	_isEnabled = false;
}

void EventTimer::Close()
{
	_isEnabled = false;
	//Application::Instance()->RemoveTimer(this);
}

}
