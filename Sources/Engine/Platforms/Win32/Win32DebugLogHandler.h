/*=============================================================================
Win32DebugLogHandler.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_WIN32DEBUGLOGHANDLER_H_
#define _SE_WIN32DEBUGLOGHANDLER_H_

#include "Win32Platform.h"
#include "Core/Common.h"
#include "Core/Logging/LogHandler.h"

namespace SonataEngine
{

/**
	@brief Win32 Debugger Log Handler.

	Log handler to the current Win32 debugger.
	With Visual Studio, logs appear in the Output panel.
*/
class SE_CORE_EXPORT Win32DebugLogHandler : public LogHandler
{
public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	Win32DebugLogHandler();

	/** Destructor. */
	virtual ~Win32DebugLogHandler();
	//@}

	virtual void Close();

	virtual void Write(const String& message);
};

}

#endif 

/*=============================================================================
09-02-2006 Julien Delezenne
	Modified to reflect the changes of the Logging classes.
=============================================================================*/
