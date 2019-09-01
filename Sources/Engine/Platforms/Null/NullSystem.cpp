/*=============================================================================
NullSystem.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Core/System/System.h"

namespace SonataEngine
{

typedef System NullSystem;

NullSystem::System()
{
}

NullSystem::~System()
{
}

String NullSystem::GetApplicationFileName()
{
	return String::Empty;
}

String NullSystem::GetApplicationExtension()
{
	return String::Empty;
}

String NullSystem::GetLibraryExtension()
{
	return String::Empty;
}

DateTime NullSystem::GetDateTime()
{
	return DateTime();
}

real64 NullSystem::GetTime()
{
	return 0.0;
}

void NullSystem::Sleep(uint32 milliseconds)
{
}

String NullSystem::GetClipboardData(Window *window)
{
	return String::Empty;
}

void NullSystem::SetClipboardData(Window *window, const String& data)
{
}

}
