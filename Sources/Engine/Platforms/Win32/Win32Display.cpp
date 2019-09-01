/*=============================================================================
Win32SystemDisplay.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Win32Platform.h"
#include "Win32Helper.h"
#include "Graphics/System/SystemDisplay.h"

namespace SonataEngine
{

typedef SystemDisplay Win32SystemDisplay;

const Win32SystemDisplay::DisplayAdapterList Win32SystemDisplay::EnumerateDisplayAdapters()
{
	Win32SystemDisplay::DisplayAdapterList adapters;

	// Obtain information about the display devices in a system
	DWORD iDevNum = 0;
	DISPLAY_DEVICE displayDevice;
	displayDevice.cb = sizeof(DISPLAY_DEVICE);
	displayDevice.StateFlags = DISPLAY_DEVICE_ATTACHED_TO_DESKTOP;
	while (::EnumDisplayDevices(NULL, iDevNum, &displayDevice, 0) != FALSE)
	{
		adapters.Add(DisplayAdapter(iDevNum, displayDevice.DeviceName, displayDevice.DeviceString,
			0, 0, 0, 0, 0, 0));

		iDevNum++;
	}

	return adapters;
}

bool Win32SystemDisplay::GetPrimaryDisplayAdapter(DisplayAdapter& adapter)
{
	DISPLAY_DEVICE displayDevice;
	displayDevice.cb = sizeof(DISPLAY_DEVICE);
	displayDevice.StateFlags = DISPLAY_DEVICE_PRIMARY_DEVICE;
	if (::EnumDisplayDevices(NULL, 0, &displayDevice, 0) == FALSE)
		return false;

	adapter = DisplayAdapter(0, displayDevice.DeviceName,
		displayDevice.DeviceString, 0, 0, 0, 0, 0, 0);
	return true;
}

BOOL CALLBACK MyMonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	Win32SystemDisplay::DisplayMonitorList monitors;

	// Retrieves information about a display monitor
	MONITORINFOEX mi;
	mi.cbSize = sizeof(MONITORINFOEX);
	if (::GetMonitorInfo(hMonitor, &mi) != FALSE)
	{
		monitors.Add(DisplayMonitor(mi.szDevice, Win32Helper::MakeRect(mi.rcMonitor),
			Win32Helper::MakeRect(mi.rcWork), (mi.dwFlags & MONITORINFOF_PRIMARY) != 0));
	}

	return TRUE;
}

const Win32SystemDisplay::DisplayMonitorList Win32SystemDisplay::EnumerateDisplayMonitors()
{
	Win32SystemDisplay::DisplayMonitorList monitors;

	::EnumDisplayMonitors(NULL, NULL, MyMonitorEnumProc, (LPARAM)&monitors);

	return monitors;
}

bool Win32SystemDisplay::GetMonitorFromAdapter(const DisplayAdapter& adapter, DisplayMonitor& monitor)
{
	DISPLAY_DEVICE displayDevice;
	displayDevice.cb = sizeof(DISPLAY_DEVICE);
	displayDevice.StateFlags = DISPLAY_DEVICE_PRIMARY_DEVICE;
	if (::EnumDisplayDevices(adapter.GetName().Data(), 0, &displayDevice, 0) == FALSE)
		return false;

	Win32SystemDisplay::DisplayMonitorList monitors = EnumerateDisplayMonitors();
	Win32SystemDisplay::DisplayMonitorList::Iterator it = monitors.GetIterator();
	while (it.Next())
	{
		if (String::Compare(displayDevice.DeviceName, it.Current().GetName(), true) == 0)
		{
			monitor = it.Current();
			return true;
		}
	}

	return false;
}

const Win32SystemDisplay::DisplayModeList Win32SystemDisplay::EnumerateDisplayModes(const DisplayAdapter& adapter)
{
	Win32SystemDisplay::DisplayModeList modes;

	// Obtain information about one of the graphics modes for a display device
	DWORD iModeNum = 0;
	DEVMODE devMode;
	devMode.dmSize = sizeof(DEVMODE);
	while (::EnumDisplaySettings(adapter.GetName().Data(), iModeNum, &devMode) != FALSE)
	{
		modes.Add(DisplayMode(devMode.dmPelsWidth, devMode.dmPelsHeight,
			devMode.dmBitsPerPel, devMode.dmDisplayFrequency));

		iModeNum++;
	}

	return modes;
}

bool Win32SystemDisplay::GetCurrentDisplayMode(DisplayMode& mode)
{
	DEVMODE devMode;
	devMode.dmSize = sizeof(DEVMODE);
	if (::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode) == FALSE)
		return false;

	mode = DisplayMode(devMode.dmPelsWidth, devMode.dmPelsHeight,
		devMode.dmBitsPerPel, devMode.dmDisplayFrequency);
	return true;
}

bool Win32SystemDisplay::GetDesktopDisplayMode(const DisplayAdapter& adapter, DisplayMode& mode)
{
	DEVMODE devMode;
	devMode.dmSize = sizeof(DEVMODE);
	if (::EnumDisplaySettings(adapter.GetName().Data(), ENUM_REGISTRY_SETTINGS, &devMode) == FALSE)
		return false;

	mode = DisplayMode(devMode.dmPelsWidth, devMode.dmPelsHeight,
		devMode.dmBitsPerPel, devMode.dmDisplayFrequency);
	return true;
}

bool Win32SystemDisplay::SetAdapterDisplayMode(const DisplayAdapter& adapter, const DisplayMode& mode, bool fullScreen)
{
	DEVMODE devMode;
	devMode.dmSize = sizeof(DEVMODE);
	devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;
	if (::ChangeDisplaySettingsEx(adapter.GetName().Data(),
		&devMode, NULL, fullScreen ? CDS_FULLSCREEN : 0, NULL) != DISP_CHANGE_SUCCESSFUL)
	{
		return false;
	}

	return true;
}

bool Win32SystemDisplay::SetCurrentDisplayMode(const DisplayMode& mode, bool fullScreen)
{
	DEVMODE devMode;
	devMode.dmSize = sizeof(DEVMODE);
	devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;
	if (::ChangeDisplaySettings(&devMode, fullScreen ? CDS_FULLSCREEN : 0) != DISP_CHANGE_SUCCESSFUL)
	{
		return false;
	}

	return true;
}

}
