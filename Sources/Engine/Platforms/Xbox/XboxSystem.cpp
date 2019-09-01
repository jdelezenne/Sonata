/*=============================================================================
XboxSystem.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <xtl.h>

#include "Core/System/System.h"

namespace SonataEngine
{

typedef System XboxSystem;

XboxSystem::System()
{
}

XboxSystem::~System()
{
}

String XboxSystem::GetApplicationFileName()
{
	return String::Empty;
}

String XboxSystem::GetApplicationExtension()
{
	return "xbe";
}

String XboxSystem::GetLibraryExtension()
{
	return "xbe";
}

DateTime XboxSystem::GetDateTime()
{
	SYSTEMTIME t;
	GetLocalTime(&t);

	return DateTime(t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
}

real64 XboxSystem::GetTime()
{
	return timeGetTime();
}

void XboxSystem::Sleep(uint32 milliseconds)
{
	::Sleep(milliseconds);
}

}
