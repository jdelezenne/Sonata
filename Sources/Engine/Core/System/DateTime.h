/*=============================================================================
DateTime.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_DATETIME_H_
#define _SE_DATETIME_H_

#include "Core/Common.h"
#include "Core/String.h"

namespace SonataEngine
{

/** Months. */
enum MonthOfYear
{
	/// Indicates January. 
	January,

	/// Indicates February.
	February, 
	
	/// Indicates March.
	March,
	
	/// Indicates April.
	April,
	
	/// Indicates May.
	May,
	
	/// Indicates June.
	June,
	
	/// Indicates July.
	July,
	
	/// Indicates August.
	August,
	
	/// Indicates September.
	September,
	
	/// Indicates October.
	October,
	
	/// Indicates November.
	November,
	
	/// Indicates December.
	December
};


/** Days. */
enum DayOfWeek
{
	/// Indicates Monday.
	Monday,
	
	/// Indicates Tuesday.
	Tuesday,
	
	/// Indicates Wednesday.
	Wednesday,
	
	/// Indicates Thursday.
	Thursday,
	
	/// Indicates Friday.
	Friday,
	
	/// Indicates Saturday.
	Saturday,
	
	/// Indicates Sunday.
	Sunday
};


/** Date and time.
	Represents an instant in time.
*/
class SE_CORE_EXPORT DateTime
{
protected:
	int64 _Ticks;
	int32 _Year;
	int32 _Month;
	int32 _Day;
	int32 _Hour;
	int32 _Minute;
	int32 _Second;
	int32 _Millisecond;

public:
	/// Initializes a new DateTime with the current time.
	DateTime();

	/// Copy constructor.
	DateTime(const DateTime& dt);

	/// Initializes a new DateTime to a specified number of ticks.
	DateTime(int64 ticks);

	/// Initializes a new DateTime to the specified year, month, and day.
	DateTime(int32 year, int32 month, int32 day);

	/// Initializes a new DateTime to the specified year, month, day, hour, minute, and second.
	DateTime(int32 year, int32 month, int32 day, int32 hour, int32 minute, int32 second);

	/// Initializes a new DateTime to the specified year, month, day, hour, minute, second, and millisecond.
	DateTime(int32 year, int32 month, int32 day, int32 hour, int32 minute, int32 second, int32 millisecond);

	/// Destructor.
	virtual ~DateTime();

protected:
	void Reset();

	bool IsValid();

	static bool IsValid(int32 year, int32 month, int32 day);
	static bool IsValid(int32 year, int32 month, int32 day, int32 hour, int32 minute, int32 second, int32 millisecond);

public:
	int64 GetTicks() const { return _Ticks; }
	int32 GetYear() const { return _Year; }
	int32 GetMonth() const { return _Month; }
	int32 GetDay() const { return _Day; }
	int32 GetHour() const { return _Hour; }
	int32 GetMinute() const { return _Minute; }
	int32 GetSecond() const { return _Second; }
	int32 GetMillisecond() const { return _Millisecond; }

	/// Gets the month of the year.
	MonthOfYear GetMonthOfYear() const;

	/// Gets the day of the week.
	DayOfWeek GetDayOfWeek() const;

	/// Gets the day of the year.
	int32 GetDayOfYear() const;

	//DateTime Add(TimeSpan value);
	DateTime AddTicks(int32 value);
	DateTime AddYears(int32 value);
	DateTime AddMonths(int32 value);
	DateTime AddDays(double value);
	DateTime AddHours(double value);
	DateTime AddMinutes(double value);
	DateTime AddSeconds(double value);
	DateTime AddMilliseconds(double value);

	DateTime GetDate();
	//TimeSpan GetTimeOfDay();

	String DateToString() const;

	String TimeToString() const;

	String ToString() const;

	/// Operators...
	bool operator == (const DateTime& dt) const;
	bool operator != (const DateTime& dt) const;
	bool operator < (const DateTime& dt) const;
	bool operator <= (const DateTime& dt) const;
	bool operator > (const DateTime& dt) const;
	bool operator >= (const DateTime& dt) const;
	//bool operator + (const TimeSpan& ts) const;
	//bool operator - (const TimeSpan& ts) const;

	/// Represents the smallest possible value of DateTime.
	static const DateTime& MinValue;

	/// Represents the largest possible value of DateTime.
	static const DateTime& MaxValue;

	/// Gets a DateTime that is the current local date and time on this computer.
	static DateTime GetNow();

	/// Gets the current date.
	static DateTime GetToday();

	/// Compares two instances of DateTime and returns an indication of their relative values.
	static int32 Compare(const DateTime dt1, const DateTime dt2);

	/// Returns the number of days in the specified month of the specified year.
	static int32 DaysInMonth(int32 year, int32 month);
};

}

#endif 
