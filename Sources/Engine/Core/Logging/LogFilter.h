/*=============================================================================
LogFilter.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_LOGFILTER_H_
#define _SE_LOGFILTER_H_

#include "Core/Common.h"
#include "Core/String.h"
#include "Core/Logging/LogRecord.h"

namespace SonataEngine
{

/**
	@brief Log filter.

	Provides the base class for log filter implementations.
*/
class SE_CORE_EXPORT LogFilter
{
public:
	/** Returns whether the log handler can log the record. */
	virtual bool CanLog(const LogRecord& record) const = 0;
};


enum LogFilterMode
{
	/** Allow all the log levels. */
	LogFilterMode_All,

	/** Allow only one log level given by the SingleLevel property of the filter. */
	LogFilterMode_Single,

	/** Allow all the log levels excepted the log level given by the ExcludedLevel property of the filter. */
	LogFilterMode_Excluded,

	/** Allow all the log levels greater the level given by the MinimumLevel property of the filter. */
	LogFilterMode_Minimum
};

/**
	@brief Default log filter.

	Provides the default log filter.
*/
class SE_CORE_EXPORT DefaultLogFilter : public LogFilter
{
public:
	DefaultLogFilter();
	virtual ~DefaultLogFilter();

	virtual bool CanLog(const LogRecord& record) const;

	LogFilterMode GetFilterMode() const { return _FilterMode; }
	void SetFilterMode(LogFilterMode value) { _FilterMode = value; }

	const LogLevel& GetSingleLevel() const { return _SingleLevel; }
	void SetSingleLevel(const LogLevel& value) { _SingleLevel = value; }

	LogLevel GetExcludedLevel() const { return _ExcludedLevel; }
	void SetExcludedLevel(LogLevel value) { _ExcludedLevel = value; }

	const LogLevel& GetMinimumLevel() const { return _MinimumLevel; }
	void SetMinimumLevel(const LogLevel& value) { _MinimumLevel = value; }

protected:
	LogFilterMode _FilterMode;
	LogLevel _SingleLevel;
	LogLevel _ExcludedLevel;
	LogLevel _MinimumLevel;
};

}

#endif 

/*=============================================================================
09-02-2006 Julien Delezenne
	Created.
=============================================================================*/
