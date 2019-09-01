/*=============================================================================
LogHandler.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_LOGHANDLER_H_
#define _SE_LOGHANDLER_H_

#include "Core/Common.h"
#include "Core/Object.h"
#include "Core/Logging/LogRecord.h"
#include "Core/Logging/LogFilter.h"
#include "Core/Logging/LogFormatter.h"

namespace SonataEngine
{

/**
	@brief Log handler.

	Provides the base class for log handler implementations.
	A log handler is responsible for writing the log records.
	@remarks Close and Write needs to be overloaded.
*/
class SE_CORE_EXPORT LogHandler : public RefObject
{
public:
	/** Destructor. */
	virtual ~LogHandler();

public:
	/** Properties. */
	//@{
	/** Gets the name of the log handler. */
	String GetName() const { return _name; }

	/** Sets the name of the log handler. */
	void SetName(String value) { _name = value; }

	/** Gets the filter of the log handler. */
	LogFilter* GetFilter() const { return _Filter; }

	/** Sets the filter of the log handler. */
	void SetFilter(LogFilter* value) { _Filter = value; }

	/** Gets the formatter of the log handler. */
	LogFormatter* GetFormatter() const { return _Formatter; }

	/** Sets the formatter of the log handler. */
	void SetFormatter(LogFormatter* value) { _Formatter = value; }
	//@}

	/** Closes the handler so it no longer receives messages. */
	virtual void Close() = 0;

	/** Writes a message to the log handler. */
	virtual void Write(const String& message) = 0;

	/** Writes a message followed by a line terminator to the log handler. */
	virtual void WriteLine(const String& message);

	/** Writes the indent to the log handler. */
	virtual void WriteIndent();

	/** Writes a log record to the log handler. */
	virtual void Log(const LogRecord& record);

protected:
	/** Protected constructor. */
	LogHandler();

protected:
	String _name;
	LogFilter* _Filter; //TODO: SmartPtr
	LogFormatter* _Formatter; //TODO: SmartPtr
	bool _NeedIndent;
	int32 _IndentLevel;
	int32 _IndentSize;
};

typedef SmartPtr<LogHandler> LogHandlerPtr;

}

#endif 

/*=============================================================================
09-02-2006 Julien Delezenne
	Rewritten. Added LogFilter and LogFormatter. Indentation not complete.
=============================================================================*/
