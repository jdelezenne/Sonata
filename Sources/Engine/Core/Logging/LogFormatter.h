/*=============================================================================
LogFormatter.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_LOGFORMATTER_H_
#define _SE_LOGFORMATTER_H_

#include "Core/Common.h"
#include "Core/String.h"
#include "Core/Logging/LogRecord.h"

namespace SonataEngine
{

/**
	@brief Log formatter.

	Provides the base class for log formatter implementations.
*/
class SE_CORE_EXPORT LogFormatter
{
public:
	virtual String Format(const LogRecord& record) const = 0;
};


/** Specifies log data options to be written to the log handler. */
enum LogOptions
{
	/** Do not write any elements. */
	LogOptions_None = 0,

	/** Write the level. */
	LogOptions_Level = (1<<0),

	/** Write the source. */
	LogOptions_Source = (1<<1),

	/** Write the date. */
	LogOptions_Date = (1<<2),

	/** Write the time. */
	LogOptions_Time = (1<<3)

};

/**
	@brief Default log formatter.

	Provides the default log formatter.
*/
class SE_CORE_EXPORT DefaultLogFormatter : public LogFormatter
{
public:
	DefaultLogFormatter();
	virtual ~DefaultLogFormatter();

	virtual String Format(const LogRecord& record) const;

	LogOptions GetOptions() const { return _Options; }
	void SetOptions(LogOptions value) { _Options = value; }

protected:
	LogOptions _Options;
};

}

#endif 

/*=============================================================================
09-02-2006 Julien Delezenne
	Created.
25-02-2006 Julien Delezenne
	Added LogOptions_Level.
=============================================================================*/
