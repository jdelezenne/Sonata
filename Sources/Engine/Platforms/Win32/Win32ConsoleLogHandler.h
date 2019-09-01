/*=============================================================================
Win32ConsoleLogHandler.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_WIN32CONSOLELOGHANDLER_H_
#define _SE_WIN32CONSOLELOGHANDLER_H_

#include "Win32Platform.h"
#include "Core/Common.h"
#include "Core/Logging/LogHandler.h"

namespace SonataEngine
{

/**
	@brief Win32 Console Log Handler.

	Log handler to the a Win32 console prompt.
	The Win32 console can display different colors.
*/
class SE_CORE_EXPORT Win32ConsoleLogHandler : public LogHandler
{
public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	Win32ConsoleLogHandler();

	/** Destructor. */
	virtual ~Win32ConsoleLogHandler();
	//@}

	/** @name Properties. */
	//@{
	bool GetColored() const { return _Colored; }
	void SetColored(bool value) { _Colored = value; }
	//@}

	virtual void Close();

	virtual void Log(const LogRecord& record);

	virtual void Write(const String& message);

	virtual void WriteLine(const String& message);

protected:
	void SetConsoleAttributes(const LogLevel& level, int32 length) const;

protected:
	HANDLE _hConsoleOutput;
	SHORT _Row;
	bool _Colored;
};

}

#endif 

/*=============================================================================
09-02-2006 Julien Delezenne
	Modified to reflect the changes of the Logging classes.
=============================================================================*/
