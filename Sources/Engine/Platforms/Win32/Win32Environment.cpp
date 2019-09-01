/*=============================================================================
Win32Environment.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Win32Platform.h"
#include "Core/System/Environment.h"
#include "Core/System/Window.h"

#ifndef UNLEN
#define UNLEN 256 // Maximum user name length
#endif

namespace SonataEngine
{

static Char InvalidPathChars[] =
{
	_T('\"'), _T('<'), _T('>'), _T('|'), _T('\0'), _T('\b')
};
static int InvalidPathCharsCount = sizeof(InvalidPathChars) / sizeof(InvalidPathChars[0]);

String _Win32GetSystemPath(int nFolder)
{
	TCHAR szPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, nFolder, NULL, SHGFP_TYPE_CURRENT, szPath)))
		return szPath;
	else
		return String::Empty;
}

OperatingSystem Environment::Platform()
{
	OperatingSystem os;
	OSVERSIONINFOEX osvi;
	BOOL bOSVersionInfoEx;

	// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
	// If that fails, try using the OSVERSIONINFO structure.
	::ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if (!(bOSVersionInfoEx = ::GetVersionEx((OSVERSIONINFO*)&osvi)))
	{
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (!::GetVersionEx((OSVERSIONINFO*)&osvi))
			return os;
	}

	os.BuildNumber = osvi.dwBuildNumber;
	os.ServicePack = osvi.szCSDVersion;
	os.ServicePackMajor = osvi.dwMajorVersion;
	os.ServicePackMinor = osvi.dwMinorVersion;

	switch (osvi.dwPlatformId)
	{
		// Test for the Windows NT product family.
		case VER_PLATFORM_WIN32_NT:
			os.Platform = OS_Windows_NT;

			// Test for the specific product family.
			if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
				os.Version = Win32OS_Windows_NET_Server_2003_Web;

			if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
				os.Version = Win32OS_Windows_XP_Home;

			if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
				os.Version = Win32OS_Windows_2000_Professional;

			if (osvi.dwMajorVersion <= 4)
				os.Version = Win32OS_Windows_NT_351;

			// Test for specific product on Windows NT 4.0 SP6 and later.
			if (bOSVersionInfoEx)
			{
				// Test for the workstation type.
				if (osvi.wProductType == VER_NT_WORKSTATION)
				{
					if (osvi.dwMajorVersion == 4)
						os.Version = Win32OS_Windows_NT_Workstation_40;

					else if (osvi.wSuiteMask & VER_SUITE_PERSONAL)
					{
						if (os.Version == Win32OS_Windows_XP_Home)
							os.Version = Win32OS_Windows_XP_Home;
					}
					else
					{
						if (os.Version == Win32OS_Windows_XP_Home)
							os.Version = Win32OS_Windows_XP_Professional;

						else if (os.Version == Win32OS_Windows_2000_Professional)
							os.Version = Win32OS_Windows_2000_Professional;
					}
				}

				// Test for the server type.
				else if (osvi.wProductType == VER_NT_SERVER)
				{
					if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
					{
						if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
							os.Version = Win32OS_Windows_NET_Server_2003_Datacenter;
						else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
							os.Version = Win32OS_Windows_NET_Server_2003_Enterprise;
						else if (osvi.wSuiteMask == VER_SUITE_BLADE)
							os.Version = Win32OS_Windows_NET_Server_2003_Web;
						else
							os.Version = Win32OS_Windows_NET_Server_2003_Standard;
					}

					else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
					{
						if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
							os.Version = Win32OS_Windows_2000_Datacenter;
						else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
							os.Version = Win32OS_Windows_2000_Advanced;
						else
							os.Version = Win32OS_Windows_2000_Server;
					}

					else // Windows NT 4.0 
					{
						if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
							os.Version = Win32OS_Windows_NT_Server_40_Enterprise;
						else
							os.Version = Win32OS_Windows_NT_Server_40;
					}
				}
			}

			else // Test for specific product on Windows NT 4.0 SP5 and earlier
			{
				HKEY hKey;
				char szProductType[80];
				DWORD dwBufLen = 80;

				if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
					"SYSTEM\\CurrentControlSet\\Control\\ProductOptions", 
					0, KEY_QUERY_VALUE, &hKey)!= ERROR_SUCCESS)
				{
					return os;
				}

				if (::RegQueryValueEx(hKey, "ProductType", NULL, NULL,
					(LPBYTE)szProductType, &dwBufLen)!= ERROR_SUCCESS || (dwBufLen > 80))
				{
					return os;
				}

				::RegCloseKey(hKey);

				if (lstrcmpi("WINNT", szProductType)== 0)
					os.Version = Win32OS_Windows_NT_Workstation_40;
				if (lstrcmpi("LANMANNT", szProductType)== 0)
					os.Version = Win32OS_Windows_NT_Server_40;
				if (lstrcmpi("SERVERNT", szProductType)== 0)
					os.Version = Win32OS_Windows_NT_Server_40_Enterprise;
			}

			// Display service pack (if any) and build number.
			if (osvi.dwMajorVersion == 4 && 
				lstrcmpi(osvi.szCSDVersion, "Service Pack 6")== 0)
			{
				HKEY hKey;

				// Test for SP6 versus SP6a.
				if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE,
					"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009",
					0, KEY_QUERY_VALUE, &hKey)== ERROR_SUCCESS)
				{
					os.ServicePack = "Service Pack 6a";
				}
				else // Windows NT 4.0 prior to SP6a
				{
					os.ServicePack = osvi.szCSDVersion;
				}

				::RegCloseKey(hKey);
			}

			else // Windows NT 3.51 and earlier or Windows 2000 and later
			{
				os.ServicePack = osvi.szCSDVersion;
			}
			break;

		// Test for the Windows 95 product family.
		case VER_PLATFORM_WIN32_WINDOWS:
			os.Platform = OS_Windows_9x;

			if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
			{
				if (osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B')
					os.Version = Win32OS_Windows_95_OSR2;
				else
					os.Version = Win32OS_Windows_95;
			} 

			if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
			{
				if (osvi.szCSDVersion[1] == 'A')
					os.Version = Win32OS_Windows_98_SE;
				else
					os.Version = Win32OS_Windows_98;
			} 

			if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
			{
				os.Version = Win32OS_Windows_Me;
			} 
			break;

		case VER_PLATFORM_WIN32s:
			os.Platform = OS_Windows_32;
			break;
	}

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
	return Win32GetCommandLine();
}

String Environment::MachineName()
{
	TCHAR szComputer[MAX_COMPUTERNAME_LENGTH+1];
	DWORD dwSize;

	dwSize = MAX_COMPUTERNAME_LENGTH + 1;
	if (::GetComputerName(szComputer, &dwSize))
		return szComputer;
	else
		return String::Empty;
}

String Environment::UserName()
{
	TCHAR szUser[UNLEN+1];
	DWORD dwSize;

	dwSize = UNLEN+1;
	if (::GetUserName(szUser, &dwSize))
	{
		String userName = szUser;
		return userName.Left(userName.IndexOf(_T('/')));
	}
	else
	{
		return String::Empty;
	}
}

String Environment::UserDomainName()
{
	TCHAR szUser[UNLEN+1];
	DWORD dwSize;

	dwSize = UNLEN+1;
	if (::GetUserName(szUser, &dwSize))
	{
		String userName = szUser;
		return userName.Right(userName.IndexOf(_T('/')) + 1);
	}
	else
	{
		return String::Empty;
	}
}

String Environment::GetApplicationFileName()
{
	TCHAR szFileName[MAX_PATH];
	::GetModuleFileName(NULL, szFileName, MAX_PATH);
	return szFileName;
}

String Environment::GetApplicationExtension()
{
	return _T("exe");
}

String Environment::GetLibraryExtension()
{
	return _T("dll");
}

String Environment::GetClipboardData(Window* window)
{
	HWND hWnd = (window != NULL ? (HWND)window->GetHandle() : NULL);

	// Determines whether the clipboard contains data in the specified format
	if (!::IsClipboardFormatAvailable(CF_TEXT))
	{
		return String::Empty;
	}

	// Opens the clipboard for examination
	if (!::OpenClipboard(hWnd))
	{
		return String::Empty;
	}

	// Retrieves data from the clipboard
	String data;
	HGLOBAL hglb = ::GetClipboardData(CF_TEXT);
	if (hglb != NULL)
	{
		LPTSTR lptstr = (LPTSTR)::GlobalLock(hglb);
		if (lptstr != NULL)
		{
			data = lptstr;
			::GlobalUnlock(hglb);
		}
	}

	// Closes the clipboard
	::CloseClipboard();

	return data;
}

void Environment::SetClipboardData(Window *window, const String& data)
{
	HWND hWnd = (window != NULL ? (HWND)window->GetHandle() : NULL);
	int32 size = data.Length() + 1;

	// Opens the clipboard for examination
	if (!::OpenClipboard(hWnd))
	{
		return;
	}

	// Empties the clipboard
	::EmptyClipboard();

	// Allocate a global memory object for the text.
	HGLOBAL hglbCopy = ::GlobalAlloc(GMEM_MOVEABLE, size);
	if (hglbCopy != NULL)
	{
		LPTSTR lptstrCopy = (LPTSTR)::GlobalLock(hglbCopy);
		::RtlCopyMemory(lptstrCopy, data.Data(), size);
		::GlobalUnlock(hglbCopy);

		// Places data on the clipboard
		::SetClipboardData(CF_TEXT, hglbCopy);
	}

	// Closes the clipboard
	::CloseClipboard();
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
	TCHAR szPath[MAX_PATH];
	if (SUCCEEDED(::Win32GetCurrentDirectory(MAX_PATH, szPath)))
		return szPath;
	else
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
	::Win32SetCurrentDirectory(value.Data());
}

String Environment::SystemDirectory()
{
	return _Win32GetSystemPath(CSIDL_WINDOWS);
}

String Environment::GetHomeDirectory()
{
	return _Win32GetSystemPath(CSIDL_MYDOCUMENTS);
}

String Environment::GetRootDirectory()
{
	return _T("C:\\");
}

String Environment::GetTempDirectory()
{
	TCHAR szPath[MAX_PATH];
	if (SUCCEEDED(::GetTempPath(MAX_PATH, szPath)))
		return szPath;
	else
		return String::Empty;
}

String Environment::GetDesktopDirectory()
{
	return _Win32GetSystemPath(CSIDL_DESKTOPDIRECTORY);
}

}
