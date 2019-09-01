/*=============================================================================
XboxEnvironment.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <xtl.h>

#include "Core/System/Environment.h"

namespace SonataEngine
{

static Char InvalidPathChars[] =
{
	_T('\"'), _T('<'), _T('>'), _T('|'), _T('\0'), _T('\b')
};
static int InvalidPathCharsCount = sizeof(InvalidPathChars) / sizeof(InvalidPathChars[0]);

static String _XboxClipboardData;

OperatingSystem Environment::Platform()
{
	OperatingSystem os;
	os.Platform = OS_Xbox;

	return os;
}

uint32 Environment::TickCount()
{
	return ::GetTickCount();
}

String Environment::NewLine()
{
	return _T("\r\n");
}

String Environment::CommandLine()
{
	DWORD dwLaunchDataType;
	LAUNCH_DATA LaunchData;
	XGetLaunchInfo(&dwLaunchDataType, &LaunchData);
	if (dwLaunchDataType == LDT_FROM_DEBUGGER_CMDLINE)
	{
		return ((PLD_FROM_DEBUGGER_CMDLINE)(&LaunchData))->szCmdLine;
	}
	return String::Empty;
}

String Environment::MachineName()
{
	// Unsupport on Xbox
	return String::Empty;
}

String Environment::UserName()
{
	// Unsupport on Xbox
	return String::Empty;
}

String Environment::UserDomainName()
{
	// Unsupport on Xbox
	return String::Empty;
}

String Environment::GetApplicationFileName()
{
	// Unsupport on Xbox
	return String::Empty;
}

String Environment::GetApplicationExtension()
{
	return _T("xbe");
}

String Environment::GetLibraryExtension()
{
	return _T("xbe");
}

String Environment::GetClipboardData(Window* window)
{
	return _XboxClipboardData;
}

void Environment::SetClipboardData(Window *window, const String& data)
{
	_XboxClipboardData = data;
}

Char Environment::GetPathSeparator()
{
	return _T('\\');
}

Char Environment::GetVolumeSeparatorChar()
{
	return _T(':');
}

Array<Char> Environment::GetInvalidPathChars()
{
	return Array<Char>(InvalidPathChars, InvalidPathCharsCount);
}

String Environment::GetCurrentDirectory()
{
	return String::Empty;
}

bool Environment::IsAbsolutePath(const String& path)
{
	if (path.Length() < 3)
		return false;

	if (Char::IsLetter(path[0]) && path[1] == GetVolumeSeparatorChar() &&
		path[2] == GetPathSeparator())
	{
		return true;
	}

	return false;
}

void Environment::SetCurrentDirectory(const String& value)
{
	// Unsupport on Xbox
}

String Environment::SystemDirectory()
{
	// Unsupport on Xbox
	return String::Empty;
}

String Environment::GetHomeDirectory()
{
	// Unsupport on Xbox
	return String::Empty;
}

String Environment::GetRootDirectory()
{
	return String::Empty;
}

String Environment::GetTempDirectory()
{
	// Unsupport on Xbox
	return String::Empty;
}

String Environment::GetDesktopDirectory()
{
	// Unsupport on Xbox
	return String::Empty;
}

}
