/*=============================================================================
LinuxSystem.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Core/System/System.h"

#include <sys/time.h>
#include <unistd.h>

namespace SonataEngine
{

typedef System LinuxSystem;

LinuxSystem::System()
{
}

LinuxSystem::~System()
{
}

String LinuxSystem::GetApplicationFileName()
{
	char fileName[PATH_MAX];
	if (readlink("/proc/self/exe", fileName, PATH_MAX) < 0)
		return String::Empty;
	else
		return fileName;
}

String LinuxSystem::GetApplicationExtension()
{
	return "elf";
}

String LinuxSystem::GetLibraryExtension()
{
	return "so";
}

DateTime LinuxSystem::GetDateTime()
{
#if defined(UNIX)
	struct timeval tv;
	gettimeofday(&tv, 0);
	time_t ltime = tv.tv_sec;
	tm* t = localtime(&ltime);
	return DateTime(1, 1, 1, t->t_hour, t->t_min, t->t_sec, t->tv_usec);
#else
	time_t ltime;
	::time(&ltime);
	tm* t = localtime(&ltime);
	return DateTime(1, 1, 1, t->t_hour, t->t_min, t->t_sec, t->tv_usec);
#endif
}

real64 LinuxSystem::GetTime()
{
	timeval tp;

	gettimeofday(&tp, NULL);

	return (tp.tv_usec / 1000000.0);
}

void LinuxSystem::Sleep(uint32 milliseconds)
{
	timeval tv;
	tv.tv_sec = milliseconds / 1000;
	tv.tv_usec = (milliseconds%1000) * 1000;
	select(0, 0, 0, 0, &tv);
}

}
