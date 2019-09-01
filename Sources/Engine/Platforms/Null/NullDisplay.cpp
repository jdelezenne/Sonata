/*=============================================================================
NullSystemDisplay.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Graphics/System/SystemDisplay.h"

namespace SonataEngine
{

typedef SystemDisplay NullSystemDisplay;

const NullSystemDisplay::DisplayAdapterList NullSystemDisplay::EnumerateDisplayAdapters()
{
	return NullSystemDisplay::DisplayAdapterList();
}

bool NullSystemDisplay::GetPrimaryDisplayAdapter(DisplayAdapter& adapter)
{
	return false;
}

const NullSystemDisplay::DisplayMonitorList NullSystemDisplay::EnumerateDisplayMonitors()
{
	return NullSystemDisplay::DisplayMonitorList();
}

bool NullSystemDisplay::GetMonitorFromAdapter(const DisplayAdapter& adapter, DisplayMonitor& monitor)
{
	return false;
}

const NullSystemDisplay::DisplayModeList NullSystemDisplay::EnumerateDisplayModes(const DisplayAdapter& adapter)
{
	return NullSystemDisplay::DisplayModeList();
}

bool NullSystemDisplay::GetCurrentDisplayMode(DisplayMode& mode)
{
	return false;
}

bool NullSystemDisplay::SetAdapterDisplayMode(const DisplayAdapter& adapter, const DisplayMode& mode, bool fullScreen)
{
	return true;
}

bool NullSystemDisplay::SetCurrentDisplayMode(const DisplayMode& mode, bool fullScreen)
{
	return true;
}

}
