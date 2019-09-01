/*=============================================================================
Logger.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_LOGGER_H_
#define _SE_LOGGER_H_

#include "Core/Common.h"
#include "Core/Manager.h"
#include "Core/Context.h"
#include "Core/Logging/LogFilter.h"
#include "Core/Logging/LogHandler.h"

namespace SonataEngine
{

/**
	@brief Logger.

	The logger is used to log messages during execution. The logs can be
	redirected to different handlers and can be formatted with date, 
	time, source...
*/
class SE_CORE_EXPORT Logger : public Manager, public Context<Logger>
{
public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	Logger();

	/** Destructor. */
	virtual ~Logger();
	//@}

	/** @name Properties. */
	//@{
	typedef Array<LogHandlerPtr> LogHandlerList;

	/** Gets the list of log handlers. */
	LogHandlerList& GetHandlers();

	/** Gets the filter of the logger. */
	LogFilter* GetFilter() const { return _Filter; }

	/** Sets the filter of the logger. */
	void SetFilter(LogFilter* value) { _Filter = value; }
	//@}

	virtual	bool Create();
	virtual	void Destroy();
	virtual void Update(real64 elapsed);

	/** @name Logging. */
	//@{
	/**
		Writes a message to the log handlers.
		@param message The message to write.
	*/
	void Log(const String& message);

	/**
		Writes a message to the log handlers.
		@param source The source of the message.
		@param message The message to write.
	*/
	void Log(const String& source, const String& message);

	/**
		Writes a message to the log handlers.
		@param level The level of the message.
		@param message The message to write.
	*/
	void Log(LogLevel level, const String& message);

	/**
		Writes a message to the log handlers.
		@param level The level of the message.
		@param source The source of the message.
		@param message The message to write.
	*/
	void Log(LogLevel level, const String& source, const String& message);

	/**
		Checks for a condition, and displays a message if the condition is false.
		@param condition true to prevent a message being displayed; otherwise, false.
		@param message The message to write.
		@remarks
			The SE_LogAssert macro should be used instead to prevent the call
			to be included in non debug builds.
	*/
	void Assert(bool condition, const String& message);
	//@}

protected:
	LogFilter* _Filter; //TODO: SmartPtr
	LogHandlerList _handlers;
	String _Header;
	String _Footer;
};

#ifdef _DEBUG
	#define SE_LogAssert(condition, message) \
		SE_Assert(condition); \
		Logger::Current()->Assert(condition, message);
#else
	#define SE_LogAssert(condition, message)
#endif

}

#endif 

/*=============================================================================
09-02-2006 Julien Delezenne
	Removed Logf and LogLine methods.
	Added a LogFilter property.
	Added Assert method and SE_LogAssert macro.
=============================================================================*/
