/*=============================================================================
XboxTimeValue.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <xtl.h>

#include "Core/System/TimeValue.h"

namespace SonataEngine
{

class TimeValueInternal
{
public:
	TimeValueInternal();

public:
	static bool _isFirstTime;
	static LONGLONG _llQPFTicksPerSec;
	LARGE_INTEGER _time;
};


bool TimeValueInternal::_isFirstTime = true;
LONGLONG TimeValueInternal::_llQPFTicksPerSec = (LONGLONG)0;

TimeValueInternal::TimeValueInternal()
{
}


TimeValue::TimeValue() :
	_internal(new TimeValueInternal())
{
}

TimeValue::TimeValue(real32 seconds) :
	_internal(new TimeValueInternal())
{
	_internal->_time.QuadPart = (LONGLONG)((real64)seconds * (real64)TimeValueInternal::_llQPFTicksPerSec);
}

TimeValue::TimeValue(real64 seconds) :
	_internal(new TimeValueInternal())
{
	_internal->_time.QuadPart = (LONGLONG)(seconds * (real64)TimeValueInternal::_llQPFTicksPerSec);
}

TimeValue::TimeValue(const TimeValue& value) :
	_internal(new TimeValueInternal())
{
	_internal->_time.QuadPart = value._internal->_time.QuadPart;
}

TimeValue::~TimeValue()
{
	delete _internal;
}

TimeValue TimeValue::operator+(const TimeValue& value) const
{
	TimeValue result;
	_internal->_time.QuadPart = _internal->_time.QuadPart + value._internal->_time.QuadPart;
	return result;
}

TimeValue TimeValue::operator-(const TimeValue& value) const
{
	TimeValue result;
	_internal->_time.QuadPart = _internal->_time.QuadPart - value._internal->_time.QuadPart;
	return result;
}

TimeValue TimeValue::operator*(const TimeValue& value) const
{
	TimeValue result;
	_internal->_time.QuadPart = _internal->_time.QuadPart * value._internal->_time.QuadPart;
	return result;
}

TimeValue TimeValue::operator/(const TimeValue& value) const
{
	TimeValue result;
	_internal->_time.QuadPart = _internal->_time.QuadPart / value._internal->_time.QuadPart;
	return result;
}

TimeValue& TimeValue::operator=(const TimeValue& value)
{
	_internal->_time.QuadPart = value._internal->_time.QuadPart;
	return *this;
}

TimeValue& TimeValue::operator+=(const TimeValue& value)
{
	_internal->_time.QuadPart += value._internal->_time.QuadPart;
	return *this;
}

TimeValue& TimeValue::operator-=(const TimeValue& value)
{
	_internal->_time.QuadPart -= value._internal->_time.QuadPart;
	return *this;
}

TimeValue& TimeValue::operator*=(const TimeValue& value)
{
	_internal->_time.QuadPart *= value._internal->_time.QuadPart;
	return *this;
}

TimeValue& TimeValue::operator/=(const TimeValue& value)
{
	_internal->_time.QuadPart /= value._internal->_time.QuadPart;
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
		// Retrieves the frequency of the high-resolution performance counter.
		::QueryPerformanceFrequency((LARGE_INTEGER*)&TimeValueInternal::_llQPFTicksPerSec);
		TimeValueInternal::_isFirstTime = false;
	}

	::QueryPerformanceCounter(&_internal->_time);
}

DateTime TimeValue::GetDateTime()
{
	SYSTEMTIME t;
	::GetLocalTime(&t);
	return DateTime(t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
}

}
