/*=============================================================================
DateTime.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "DateTime.h"
#include "Core/Exception/ArgumentException.h"
#include "Core/System/TimeValue.h"

namespace SonataEngine
{

const DateTime& DateTime::MinValue = DateTime(1, 1, 1, 0, 0, 0);
const DateTime& DateTime::MaxValue = DateTime(9999, 12, 31, 23, 59, 59);

DateTime::DateTime()
{
	Reset();
}

DateTime::DateTime(const DateTime& dt)
{
	_Ticks = dt.GetTicks();
	_Year = dt.GetYear();
	_Month = dt.GetMonth();
	_Day = dt.GetDay();
	_Hour = dt.GetHour();
	_Minute = dt.GetMinute();
	_Second = dt.GetSecond();
	_Millisecond = dt.GetMillisecond();
}

DateTime::DateTime(int64 ticks)
{
	_Ticks = ticks;

	Reset();
}

DateTime::DateTime(int32 year, int32 month, int32 day)
{
	Reset();

	if (!IsValid(year, month, day))
	{
		SEthrow(ArgumentException());
		return;
	}

	_Year = year;
	_Month = month;
	_Day = day;
}

DateTime::DateTime(int32 year, int32 month, int32 day, int32 hour, int32 minute, int32 second)
{
	Reset();

	if (!IsValid(year, month, day, hour, minute, second, 0))
	{
		SEthrow(ArgumentException());
		return;
	}

	_Year = year;
	_Month = month;
	_Day = day;
	_Hour = hour;
	_Minute = minute;
	_Second = second;
}

DateTime::DateTime(int32 year, int32 month, int32 day, int32 hour, int32 minute, int32 second, int32 millisecond)
{
	Reset();

	if (!IsValid(year, month, day, hour, minute, second, millisecond))
	{
		SEthrow(ArgumentException());
		return;
	}

	_Year = year;
	_Month = month;
	_Day = day;
	_Hour = hour;
	_Minute = minute;
	_Second = second;
	_Millisecond = millisecond;
}

DateTime::~DateTime()
{
}

void DateTime::Reset()
{
	_Ticks = 0;
	_Year = 1;
	_Month = 1;
	_Day = 1;
	_Hour = 0;
	_Minute = 0;
	_Second = 0;
	_Millisecond = 0;
}

bool DateTime::IsValid()
{
	return (*this >= DateTime::MinValue && *this <= DateTime::MinValue);
}

bool DateTime::IsValid(int32 year, int32 month, int32 day)
{
	if (year >= 1 && year <= 9999 && month >= 1 && month <= 12)
	{
		//TODO
		if (day >= 1 && day <= 31)
			return true;
	}

	return false;
}

bool DateTime::IsValid(int32 year, int32 month, int32 day, int32 hour, int32 minute, int32 second, int32 millisecond)
{
	return (IsValid(year, month, day) &&
		hour >= 0 && hour < 24 && minute >= 0 && minute < 60 && second >= 0 && second < 60);
}

MonthOfYear DateTime::GetMonthOfYear() const
{
	return (MonthOfYear) _Month;
}

int32 DateTime::GetDayOfYear() const
{
	return 0;
}

DayOfWeek DateTime::GetDayOfWeek() const
{
	return Monday;
}

DateTime DateTime::AddTicks(int32 value)
{
	DateTime dt(*this);
	dt._Ticks += value;
	return dt;
}

DateTime DateTime::AddYears(int32 value)
{
	DateTime dt(*this);
	dt._Year += value;
	return dt;
}

DateTime DateTime::AddMonths(int32 value)
{
	DateTime dt(*this);
	dt._Month += value;
	return dt;
}

DateTime DateTime::AddDays(double value)
{
	DateTime dt(*this);
	dt._Day += value;
	return dt;
}

DateTime DateTime::AddHours(double value)
{
	DateTime dt(*this);
	dt._Hour += value;
	return dt;
}

DateTime DateTime::AddMinutes(double value)
{
	DateTime dt(*this);
	dt._Minute += value;
	return dt;
}

DateTime DateTime::AddSeconds(double value)
{
	DateTime dt(*this);
	dt._Second += value;
	return dt;
}

DateTime DateTime::AddMilliseconds(double value)
{
	DateTime dt(*this);
	dt._Millisecond += value;
	return dt;
}


DateTime DateTime::GetDate()
{
	DateTime dt(_Year, _Month, _Day);
	return dt;
}

String DateTime::DateToString() const
{
	return String::Format("%02d/%02d/%d", _Day, _Month, _Year);
}

String DateTime::TimeToString() const
{
	return String::Format("%02d:%02d:%02d", _Hour, _Minute, _Second);
}

String DateTime::ToString() const
{
	return String::Format("%02d/%02d/%d %02d:%02d:%02d", _Day, _Month, _Year, _Hour, _Minute, _Second);
}

DateTime DateTime::GetNow()
{
	return TimeValue::GetDateTime();
}


DateTime DateTime::GetToday()
{
	return TimeValue::GetDateTime().GetDate();
}

int32 DateTime::Compare(const DateTime dt1,	const DateTime dt2)
{
	if (dt1 < dt2)
		return -1;
	else if (dt1 > dt2)
		return 1;
	else
		return 0;
}

int32 DateTime::DaysInMonth(int32 year, int32 month)
{
	return 0;
}

SE_INLINE bool DateTime::operator==(const DateTime& dt) const
{
	return (_Year == dt.GetYear() && 
		_Month == dt.GetMonth() && 
		_Day == dt.GetDay() && 
		_Hour == dt.GetHour() && 
		_Minute == dt.GetMinute() && 
		_Second == dt.GetSecond() && 
		_Millisecond == dt.GetMillisecond());
}

SE_INLINE bool DateTime::operator!=(const DateTime& dt) const
{
	return (_Year != dt.GetYear() || 
		_Month != dt.GetMonth() || 
		_Day != dt.GetDay() || 
		_Hour != dt.GetHour() || 
		_Minute != dt.GetMinute() || 
		_Second != dt.GetSecond() || 
		_Millisecond != dt.GetMillisecond());
}

SE_INLINE bool DateTime::operator<(const DateTime& dt) const
{
	// Year.
	if (_Year < dt.GetYear())
		return true;
	else if (_Year > dt.GetYear())
		return false;
	else
	{
		// Month.
		if (_Month < dt.GetMonth())
			return true;
		else if (_Month > dt.GetMonth())
			return false;
		else
		{
			// Day.
			if (_Day < dt.GetDay())
				return true;
			else if (_Day > dt.GetDay())
				return false;
			else
			{
				// Hour.
				if (_Hour < dt.GetHour())
					return true;
				else if (_Hour > dt.GetHour())
					return false;
				else
				{
					// Minute.
					if (_Minute < dt.GetMinute())
						return true;
					else if (_Minute > dt.GetMinute())
						return false;
					else
					{
						// Seconde.
						if (_Second < dt.GetSecond())
							return true;
						else if (_Second > dt.GetSecond())
							return false;
						else
						{
							// Milliseconde.
							if (_Millisecond < dt.GetMillisecond())
								return true;
							else if (_Millisecond > dt.GetMillisecond())
								return false;
							else
								return false;
						}
					}
				}
			}
		}
	}
}

SE_INLINE bool DateTime::operator<=(const DateTime& dt) const
{
	return (*this < dt || *this == dt);
}

SE_INLINE bool DateTime::operator>(const DateTime& dt) const
{
	return (!(*this <= dt));
}

SE_INLINE bool DateTime::operator>=(const DateTime& dt) const
{
	return (*this > dt || *this == dt);
}

/*SE_INLINE bool DateTime::operator + (const TimeSpan& ts) const
{
	return false;
}

SE_INLINE bool DateTime::operator - (const TimeSpan& ts) const
{
	return false;
}*/

}
