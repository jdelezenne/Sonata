/*=============================================================================
Timer.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Timer.h"
#include "Core/System/TimeValue.h"

namespace SonataEngine
{

Timer::Timer()
{
	Reset();
}

Timer::~Timer()
{
}

real64 Timer::Elapsed() const
{
	if (!_IsRunning)
	{
		return _Elapsed;
	}
	else
	{
		real64 current = TimeValue::GetTime();
		return current - _StartTime;
	}
}

void Timer::Start()
{
	if (!_IsRunning)
	{
		_StartTime = TimeValue::GetTime();
	}
	_IsRunning = true;
}

void Timer::Stop()
{
	if (_IsRunning)
	{
		real64 current = TimeValue::GetTime();
		_Elapsed = current - _StartTime;
		_IsRunning = false;
	}
}

void Timer::Reset()
{
	_IsRunning = false;
	_Elapsed = 0.0;
}

}
