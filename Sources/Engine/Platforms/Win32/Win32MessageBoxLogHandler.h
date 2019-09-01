/*=============================================================================
Win32MessageBoxLogHandler.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_WIN32MESSAGEBOXLOGHANDLER_H_
#define _SE_WIN32MESSAGEBOXLOGHANDLER_H_

#include "Win32Platform.h"
#include "Core/Common.h"
#include "Core/Logging/LogHandler.h"

namespace SonataEngine
{

/**
	@brief Win32 Message Box Log Handler.

	Log handler to a Win32 system message box.
*/
class SE_CORE_EXPORT Win32MessageBoxLogHandler : public LogHandler
{
public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	Win32MessageBoxLogHandler();

	/** Destructor. */
	virtual ~Win32MessageBoxLogHandler();
	//@}

	/** @name Properties. */
	//@{
	const String& GetTitle() const { return _Title; }
	void SetTitle(const String& value) { _Title = value; }
	//@}

	virtual void Close();

	virtual void Log(const LogRecord& record);

	virtual void Write(const String& message);

protected:
	UINT GetIconFlag(const LogLevel& level);

protected:
	UINT _IconFlag;
	String _Title;
};

}

#endif 

/*=============================================================================
09-02-2006 Julien Delezenne
	Modified to reflect the changes of the Logging classes.
=============================================================================*/
