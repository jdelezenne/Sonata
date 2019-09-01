/*=============================================================================
NullEnvironment.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Core/System/Environment.h"
#include "Core/System/Application.h"

namespace SonataEngine
{

typedef Environment NullEnvironment;

OperatingSystem NullEnvironment::Platform()
{
	OperatingSystem os;
	os.Platform = OS_Unknown;
	return os;
}

uint32 NullEnvironment::TickCount()
{
	return 0;
}

String NullEnvironment::NewLine()
{
	return String::Empty;
}

String NullEnvironment::CommandLine()
{
	return String::Empty;
}

String NullEnvironment::MachineName()
{
	return String::Empty;
}

String NullEnvironment::UserName()
{
	return String::Empty;
}

String NullEnvironment::UserDomainName()
{
	return String::Empty;
}

Char NullEnvironment::GetPathSeparator()
{
	return Char::Null;
}

Char NullEnvironment::GetVolumeSeparatorChar()
{
	return Char::Null;
}

Array<Char> NullEnvironment::GetInvalidPathChars()
{
	return Array<Char>();
}

String NullEnvironment::GetCurrentDirectory()
{
	return String::Empty;
}

bool NullEnvironment::IsAbsolutePath(const String& path)
{
	return false;
}

void NullEnvironment::SetCurrentDirectory(const String& value)
{
}

String NullEnvironment::SystemDirectory()
{
	return String::Empty;
}

String NullEnvironment::GetHomeDirectory()
{
	return String::Empty;
}

String NullEnvironment::GetRootDirectory()
{
	return String::Empty;
}

String NullEnvironment::GetTempDirectory()
{
	return String::Empty;
}

String NullEnvironment::GetDesktopDirectory()
{
	return String::Empty;
}

}
