/*=============================================================================
DisplayMonitor.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_DISPLAYMONITOR_H_
#define _SE_DISPLAYMONITOR_H_

#include "Graphics/Common.h"

namespace SonataEngine
{

/** Display Monitor.
	Description of a monitor.
	@remark From Win32
*/
class SE_GRAPHICS_EXPORT DisplayMonitor
{
protected:
	String _name;
	RectangleInt _monitorRectangle;
	RectangleInt _workArea;
	bool _isPrimary;

public:
	/** Constructor. */
	DisplayMonitor();

	/** Constructor. */
	DisplayMonitor(const String& name, const RectangleInt& monitorRectangle, const RectangleInt& workArea, bool isPrimary);

	/** Retrieves the name of the display monitor. */
	String GetName() const { return _name; }

	/** Retrieves the display monitor rectangle. */
	RectangleInt GetMonitorRectangle() const { return _monitorRectangle; }

	/** Retrieves the work area rectangle of the display monitor. */
	RectangleInt GetWorkArea() const { return _workArea; }

	/** Returns whether this monitor is the primary display monitor. */
	bool IsPrimary() const { return _isPrimary; }
};


SE_INLINE DisplayMonitor::DisplayMonitor() :
	_monitorRectangle(RectangleInt::Empty),
	_workArea(RectangleInt::Empty),
	_isPrimary(false)
{
}

SE_INLINE DisplayMonitor::DisplayMonitor(const String& name, const RectangleInt& monitorRectangle, const RectangleInt& workArea, bool isPrimary) :
	_name(name),
	_monitorRectangle(monitorRectangle),
	_workArea(workArea),
	_isPrimary(isPrimary)
{
}

}

#endif 
