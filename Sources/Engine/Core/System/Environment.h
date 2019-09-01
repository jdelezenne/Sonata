/*=============================================================================
Environment.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ENVIRONMENT_H_
#define _SE_ENVIRONMENT_H_

#include "Core/Common.h"
#include "Core/String.h"

namespace SonataEngine
{

class Window;

/** Operating Systems and Console Systems. */
enum PlatformID
{
	OS_Unknown, // Unknown
	OS_DOS_32, // Microsoft DOS 32
	OS_Windows_32, // Microsoft Windows 32
	OS_Windows_64, // Microsoft Windows 64
	OS_Windows_9x, // Microsoft Windows 9x
	OS_Windows_NT, // Microsoft Windows NT
	OS_Windows_CE, // Microsoft Windows CE
	OS_Linux, // Linux
	OS_Linux_64, // Linux 64
	OS_MacOS, // Apple MacOS
	OS_MacOSX, // Apple MacOSX
	OS_PS2, // Sony Playstation 2
	OS_Xbox, // Microsoft Xbox
	OS_GC, // Nintendo GameCube
	OS_GBA, // Nintendo GBA
	OS_DS, // Nintendo DS
	OS_PSP, // Sony PSP
	OS_Xbox360, // Microsoft Xbox 360
	OS_Wii, // Nintendo Wii
	OS_PS3 // Sony Playstation 3
};

#ifdef _WIN32
/** Operating System versions. */
enum Win32OSVersion
{
	Win32OS_Windows_95, // Windows 95
	Win32OS_Windows_95_OSR2, // Windows 95 OSR2
	Win32OS_Windows_98, // Windows 98
	Win32OS_Windows_98_SE, // Windows 98 SE
	Win32OS_Windows_Me, // Windows Me
	Win32OS_Windows_NT_351, // Windows NT 3.51
	Win32OS_Windows_NT_Workstation_40, // Windows NT Workstation 4.0
	Win32OS_Windows_NT_Server_40, // Windows NT Server 4.0
	Win32OS_Windows_NT_Server_40_Enterprise, // Windows NT Server 4.0 Enterprise Edition
	Win32OS_Windows_2000_Professional, // Windows 2000 Professional
	Win32OS_Windows_2000_Server, // Windows 2000 Server
	Win32OS_Windows_2000_Datacenter, // Windows 2000 Datacenter Server
	Win32OS_Windows_2000_Advanced, // Windows 2000 Advanced Server
	Win32OS_Windows_XP_Home, // Windows XP Home
	Win32OS_Windows_XP_Professional, // Windows XP Professional
	Win32OS_Windows_XP_Professional_64, // Windows XP Professional 64
	Win32OS_Windows_NET_Server_2003_Web, // Windows .NET Server 2003 Web Edition
	Win32OS_Windows_NET_Server_2003_Standard, // Windows .NET Server 2003 Standard Edition
	Win32OS_Windows_NET_Server_2003_Enterprise, // Windows .NET Server 2003 Enterprise Edition
	Win32OS_Windows_NET_Server_2003_Datacenter // Windows .NET Server 2003 Datacenter Edition
};
#endif

struct OperatingSystem
{
	PlatformID Platform;
#ifdef _WIN32
	Win32OSVersion Version;
	int32 BuildNumber;
	String ServicePack;
	int32 ServicePackMajor;
	int32 ServicePackMinor;
#endif
};

/** @brief Environment
	Provides information about  the current environment.
	These methods are platform-specific.
*/
class SE_CORE_EXPORT Environment
{
public:
	/** Swaps the contents of an array of bytes. */
	static void SwapEndian(SEbyte* data, int size, int count = 1);

	/** @name General */
	//@{
	/** Gets the current platform identifier. */
	static OperatingSystem Platform();

	/** Gets the number of milliseconds elapsed since the system started. */
	static uint32 TickCount(); 

	/** Gets the newline string defined for this environment. */
	static String NewLine();

	/** Gets the command line for this process. */
	static String CommandLine();

	/** Gets the name of this local computer. */
	static String MachineName();

	/** Gets the user name of the person who started the current thread. */
	static String UserName();

	/** Gets the network domain name associated with the current user. */
	static String UserDomainName();

	/** Gets the file name of the application executable. */
	static String GetApplicationFileName();

	/** Gets the extension of the system executables. */
	static String GetApplicationExtension();

	/** Gets the extension of the system libraries. */
	static String GetLibraryExtension();

	/** Retrieves data from the clipboard. */
	static String GetClipboardData(Window* window);

	/** Places data on the clipboard. */
	static void SetClipboardData(Window* window, const String& data);
	//@}

	/** @name Paths */
	//@{
	/** Gets the directory separator character. */
	static Char GetPathSeparator();

	/** Gets the volume separator character. */
	static Char GetVolumeSeparatorChar();

	/** Gets the array of characters that cannot be specified in path string. */
	static Array<Char> GetInvalidPathChars();

	/** Returns whether the specified path is absolute. */
	static bool IsAbsolutePath(const String& path);

	/** Gets the path of the current directory. */
	static String GetCurrentDirectory();

	/** Sets the path of the current directory. */
	static void SetCurrentDirectory(const String& value);

	/** Gets the of the system directory. */
	static String SystemDirectory();

	/** Gets the path of the current system's home directory. */
	static String GetHomeDirectory();

	/** Gets the path of the current system's root directory. */
	static String GetRootDirectory();

	/** Gets the path of the current system's temporary folder. */
	static String GetTempDirectory();

	/** Gets the path of the current system's desktop folder. */
	static String GetDesktopDirectory();
	//@}
};

}

#endif 
