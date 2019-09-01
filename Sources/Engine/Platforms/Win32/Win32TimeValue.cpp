/*=============================================================================
Win32TimeValue.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <windows.h>

#include "Core/System/TimeValue.h"

#define SE_USE_QPF 1
//#define SE_USE_WINMM

namespace SonataEngine
{

class TimeValueInternal
{
public:
	TimeValueInternal();

public:
	static bool _isQPFSupported;
	static bool _isFirstTime;
	static LONGLONG _llQPFTicksPerSec;
	LARGE_INTEGER _time;
#if _DEBUG
	real64 _seconds;
#endif
};


bool TimeValueInternal::_isQPFSupported = false;
bool TimeValueInternal::_isFirstTime = true;
LONGLONG TimeValueInternal::_llQPFTicksPerSec = (LONGLONG)0;

TimeValueInternal::TimeValueInternal()
{
	_time.QuadPart = 0;
#if _DEBUG
	_seconds = 0;
#endif
}


TimeValue::TimeValue() :
	_internal(new TimeValueInternal())
{
}

TimeValue::TimeValue(real32 seconds) :
	_internal(new TimeValueInternal())
{
	_internal->_time.QuadPart = (LONGLONG)((real64)seconds * (real64)TimeValueInternal::_llQPFTicksPerSec);
#if _DEBUG
	_internal->_seconds = (real64)seconds;
#endif
}

TimeValue::TimeValue(real64 seconds) :
	_internal(new TimeValueInternal())
{
	_internal->_time.QuadPart = (LONGLONG)(seconds * (real64)TimeValueInternal::_llQPFTicksPerSec);
#if _DEBUG
	_internal->_seconds = seconds;
#endif
}

TimeValue::TimeValue(const TimeValue& value) :
	_internal(new TimeValueInternal())
{
	_internal->_time.QuadPart = value._internal->_time.QuadPart;
#if _DEBUG
	_internal->_seconds = value._internal->_seconds;
#endif
}

TimeValue::~TimeValue()
{
	delete _internal;
}

TimeValue TimeValue::operator+(const TimeValue& value) const
{
	TimeValue result;
	result._internal->_time.QuadPart = _internal->_time.QuadPart + value._internal->_time.QuadPart;
#if _DEBUG
	result._internal->_seconds = _internal->_seconds + value._internal->_seconds;
#endif
	return result;
}

TimeValue TimeValue::operator-(const TimeValue& value) const
{
	TimeValue result;
	result._internal->_time.QuadPart = _internal->_time.QuadPart - value._internal->_time.QuadPart;
#if _DEBUG
	result._internal->_seconds = _internal->_seconds - value._internal->_seconds;
#endif
	return result;
}

TimeValue TimeValue::operator*(const TimeValue& value) const
{
	TimeValue result;
	result._internal->_time.QuadPart = _internal->_time.QuadPart * value._internal->_time.QuadPart;
#if _DEBUG
	result._internal->_seconds = _internal->_seconds * value._internal->_seconds;
#endif
	return result;
}

TimeValue TimeValue::operator/(const TimeValue& value) const
{
	TimeValue result;
	result._internal->_time.QuadPart = _internal->_time.QuadPart / value._internal->_time.QuadPart;
#if _DEBUG
	result._internal->_seconds = _internal->_seconds / value._internal->_seconds;
#endif
	return result;
}

TimeValue& TimeValue::operator=(const TimeValue& value)
{
	_internal->_time.QuadPart = value._internal->_time.QuadPart;
#if _DEBUG
	_internal->_seconds = value._internal->_seconds;
#endif
	return *this;
}

TimeValue& TimeValue::operator+=(const TimeValue& value)
{
	_internal->_time.QuadPart += value._internal->_time.QuadPart;
#if _DEBUG
	_internal->_seconds += value._internal->_seconds;
#endif
	return *this;
}

TimeValue& TimeValue::operator-=(const TimeValue& value)
{
	_internal->_time.QuadPart -= value._internal->_time.QuadPart;
#if _DEBUG
	_internal->_seconds -= value._internal->_seconds;
#endif
	return *this;
}

TimeValue& TimeValue::operator*=(const TimeValue& value)
{
	_internal->_time.QuadPart *= value._internal->_time.QuadPart;
#if _DEBUG
	_internal->_seconds *= value._internal->_seconds;
#endif
	return *this;
}

TimeValue& TimeValue::operator/=(const TimeValue& value)
{
	_internal->_time.QuadPart /= value._internal->_time.QuadPart;
#if _DEBUG
	_internal->_seconds /= value._internal->_seconds;
#endif
	return *this;
}

bool TimeValue::operator==(const TimeValue& value) const
{
	return _internal->_time.QuadPart == value._internal->_time.QuadPart;
}

bool TimeValue::operator!=(const TimeValue& value) const
{
	return _internal->_time.QuadPart != value._internal->_time.QuadPart;
}

bool TimeValue::operator<(const TimeValue& value) const
{
	return _internal->_time.QuadPart < value._internal->_time.QuadPart;
}

bool TimeValue::operator>(const TimeValue& value) const
{
	return _internal->_time.QuadPart > value._internal->_time.QuadPart;
}

bool TimeValue::operator<=(const TimeValue& value) const
{
	return _internal->_time.QuadPart <= value._internal->_time.QuadPart;
}

bool TimeValue::operator>=(const TimeValue& value) const
{
	return _internal->_time.QuadPart >= value._internal->_time.QuadPart;
}

TimeValue::operator real32() const
{
	return (real32)((real64)_internal->_time.QuadPart / (real64)TimeValueInternal::_llQPFTicksPerSec);
}

TimeValue::operator real64() const
{
	return ((real64)_internal->_time.QuadPart / (real64)TimeValueInternal::_llQPFTicksPerSec);
}

void TimeValue::Update()
{
	if (TimeValueInternal::_isFirstTime)
	{
#ifdef SE_USE_QPF
		// Retrieves the frequency of the high-resolution performance counter.
		LARGE_INTEGER qwTicksPerSec;
		TimeValueInternal::_isQPFSupported = (::QueryPerformanceFrequency(&qwTicksPerSec) != 0);
		if (TimeValueInternal::_isQPFSupported)
		{
			TimeValueInternal::_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
		}
		TimeValueInternal::_isFirstTime = false;
#else
		TimeValueInternal::_isQPFSupported = false;
#endif
	}

	if (TimeValueInternal::_isQPFSupported)
	{
		::QueryPerformanceCounter(&_internal->_time);
	}
	else
	{
		real64 seconds;
#ifdef SE_USE_WINMM
		seconds = (real64)::timeGetTime() / 1000.0;
#else
		seconds = (real64)::GetTickCount() / 1000.0;
#endif
		_internal->_time.QuadPart = (LONGLONG)(seconds / (real64)TimeValueInternal::_llQPFTicksPerSec);
	}

#if _DEBUG
	_internal->_seconds = ((real64)_internal->_time.QuadPart / (real64)TimeValueInternal::_llQPFTicksPerSec);
#endif
}

DateTime TimeValue::GetDateTime()
{
	SYSTEMTIME t;
	::GetLocalTime(&t);
	return DateTime(t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
}

}
