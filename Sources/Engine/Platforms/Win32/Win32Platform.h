/*=============================================================================
Win32Platform.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_WIN32PLATFORM_H_
#define _SE_WIN32PLATFORM_H_

#include <windows.h>
#include <shlwapi.h>

#include <mmsystem.h>
#include <shlobj.h>

#ifdef UNICODE
#	define Win32CreateFont           CreateFontW
#	define Win32GetMessage           GetMessageW
#	define Win32GetCommandLine       GetCommandLineW
#	define Win32GetCurrentDirectory  GetCurrentDirectoryW
#	define Win32SetCurrentDirectory  SetCurrentDirectoryW
#else
#	define Win32CreateFont           CreateFontA
#	define Win32GetMessage           GetMessageA
#	define Win32GetCommandLine       GetCommandLineA
#	define Win32GetCurrentDirectory  GetCurrentDirectoryA
#	define Win32SetCurrentDirectory  SetCurrentDirectoryA
#endif

#undef CreateFont
#undef GetMessage
#undef GetCommandLine
#undef GetCurrentDirectory
#undef SetCurrentDirectory

#endif 
