/*=============================================================================
LinuxEnvironment.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Core/System/Environment.h"

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>

namespace SonataEngine
{

typedef Environment LinuxEnvironment;

OperatingSystem LinuxEnvironment::Platform()
{
	OperatingSystem os;
	os.Platform = OS_Linux;
	
		return os;
}

uint32 LinuxEnvironment::TickCount()
{
	//TODO
	return 0;
}

String LinuxEnvironment::NewLine()
{
	return _T("\n");
}

String LinuxEnvironment::CommandLine()
{
	//TODO
	return String::Empty;
}

String LinuxEnvironment::MachineName()
{
	//TODO
	return String::Empty;
}

String LinuxEnvironment::UserName()
{
	struct passwd* pw;
	char* name;

	if (!(name = getlogin()))
	{
		if ((pw = getpwuid(getuid())))
			return pw->pw_name;
		else
			return String::Empty;
	}
	else
	{
		return name;
	}
}

String LinuxEnvironment::UserDomainName()
{
	//TODO
	return String::Empty;
}

Char LinuxEnvironment::GetPathSeparator()
{
	return _T('/');
}

Char LinuxEnvironment::GetVolumeSeparatorChar()
{
	return _T('/');
}

Array<Char> LinuxEnvironment::GetInvalidPathChars()
{
	//TODO
	return Array<Char>();
}

bool LinuxEnvironment::IsAbsolutePath(const String& path)
{
	if (path.IsEmpty())
		return false;

	return (path[0] == GetVolumeSeparatorChar());
}

String LinuxEnvironment::GetCurrentDirectory()
{
	return getenv("PWD");
}

void LinuxEnvironment::SetCurrentDirectory(const String& value)
{
	putenv("PWD=" + value.Data());
}

String LinuxEnvironment::SystemDirectory()
{
	return String::Empty;
}

String LinuxEnvironment::GetHomeDirectory()
{
	return getenv("HOME");
}

String LinuxEnvironment::GetRootDirectory()
{
	return _T("/");
}

String LinuxEnvironment::GetTempDirectory()
{
	return getenv("TMP");
}

String LinuxEnvironment::GetDesktopDirectory()
{
	return String::Empty;
}

}
