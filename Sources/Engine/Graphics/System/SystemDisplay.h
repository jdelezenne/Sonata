/*=============================================================================
SystemDisplay.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SYSTEMDISPLAY_H_
#define _SE_SYSTEMDISPLAY_H_

#include "Graphics/Common.h"
#include "Graphics/System/DisplayAdapter.h"
#include "Graphics/System/DisplayMode.h"
#include "Graphics/System/DisplayMonitor.h"

namespace SonataEngine
{

/**
	@brief System display.

	Provides system display operations.
*/
class SE_GRAPHICS_EXPORT SystemDisplay
{
public:
	typedef BaseArray<DisplayAdapter> DisplayAdapterList;
	typedef BaseArray<DisplayMonitor> DisplayMonitorList;
	typedef BaseArray<DisplayMode> DisplayModeList;

public:
	/** Enumerates the display adapters of the system. */
	static const DisplayAdapterList EnumerateDisplayAdapters();

	/** Retrieves the primary display adapter of the system. */
	static bool GetPrimaryDisplayAdapter(DisplayAdapter& adapter);

	/** Enumerates the display adapters of the system. */
	static const DisplayMonitorList EnumerateDisplayMonitors();

	/** Retrieves the current display mode of the specified display adapter. */
	static bool GetMonitorFromAdapter(const DisplayAdapter& adapter, DisplayMonitor& monitor);

	/** Enumerates the display modes of the specified display adapter. */
	static const DisplayModeList EnumerateDisplayModes(const DisplayAdapter& adapter);

	/** Retrieves the current display mode of the system. */
	static bool GetCurrentDisplayMode(DisplayMode& mode);

	/** Retrieves the desktop display mode of the specified display adapter. */
	static bool GetDesktopDisplayMode(const DisplayAdapter& adapter, DisplayMode& mode);

	/** Changes the display mode of the specified display adapter. */
	static bool SetAdapterDisplayMode(const DisplayAdapter& adapter, const DisplayMode& mode, bool fullScreen = false);

	/** Changes the display mode of the system. */
	static bool SetCurrentDisplayMode(const DisplayMode& mode, bool fullScreen = false);
};

}

#endif 

/*=============================================================================
11-02-2006 Julien Delezenne
	Created.
=============================================================================*/
