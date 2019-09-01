/*=============================================================================
TimeValue.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_TIMEVALUE_H_
#define _SE_TIMEVALUE_H_

#include "Core/Common.h"
#include "Core/System/DateTime.h"

namespace SonataEngine
{

class TimeValueInternal;

/**
	@class TimeValue.
	@brief TimeValue functions.
*/
class SE_CORE_EXPORT TimeValue
{
public:
	TimeValue();
	explicit TimeValue(real32 seconds);
	explicit TimeValue(real64 seconds);
	TimeValue(const TimeValue& value);
	~TimeValue();

	TimeValue operator+(const TimeValue& value) const;
	TimeValue operator-(const TimeValue& value) const;
	TimeValue operator*(const TimeValue& value) const;
	TimeValue operator/(const TimeValue& value) const;

	TimeValue& operator=(const TimeValue& value);
	TimeValue& operator+=(const TimeValue& value);
	TimeValue& operator-=(const TimeValue& value);
	TimeValue& operator*=(const TimeValue& value);
	TimeValue& operator/=(const TimeValue& value);

	bool operator==(const TimeValue& value) const;
	bool operator!=(const TimeValue& value) const;
	bool operator<(const TimeValue& value) const;
	bool operator>(const TimeValue& value) const;
	bool operator<=(const TimeValue& value) const;
	bool operator>=(const TimeValue& value) const;

	operator real32() const;
	operator real64() const;

	/** Gets the current system date and time. */
	static DateTime GetDateTime();

	/** Gets the current system time. */
	static TimeValue GetTime();

private:
	void Update();

private:
	TimeValueInternal* _internal;
};


SE_INLINE TimeValue TimeValue::GetTime()
{
	TimeValue result;
	result.Update();
	return result;
}

}

#endif 
