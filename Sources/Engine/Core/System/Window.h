/*=============================================================================
Window.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_WINDOW_H_
#define _SE_WINDOW_H_

#include "Core/Common.h"
#include "Core/Event.h"
#include "Core/Rectangle.h"

namespace SonataEngine
{

#ifdef WIN32
	typedef void* WindowHandle;
#elif defined(LINUX)
	typedef unsigned long WindowHandle;
#elif defined(__APPLE__)
	typedef void* WindowHandle;
#else
	typedef void* WindowHandle;
#endif

/** Window styles. */
enum WindowStyle
{
	/// No style for a full screen window with no borders and no caption bar.
	WindowStyle_None = 0,

	/// Window with borders.
	WindowStyle_Borders = (1<<0),

	/// Window with resizable borders.
	WindowStyle_Resize = (1<<1),

	/// Window with a caption bar.
	WindowStyle_Caption = (1<<3),

	/// Window that can be minimized.
	WindowStyle_Minimize = (1<<4),

	/// Window that can be maximized.
	WindowStyle_Maximize = (1<<5),

	/// Default window with resizable borders and a caption bar.
	WindowStyle_Default = (WindowStyle_Borders | WindowStyle_Resize | WindowStyle_Caption |	WindowStyle_Minimize | WindowStyle_Maximize)
};

class WindowInternal;

/**
	@class Window.
	@brief System window.
*/
class SE_CORE_EXPORT Window
{
public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	Window();

	/** Constructor.
		@param handle Handle of an existing window.
	*/
	Window(WindowHandle handle);

	/** Destructor. */
	virtual ~Window();
	//@}

	/** @name Properties. */
	//@{
	/** Gets the handle of the window. */
	WindowHandle GetHandle() const;

	/** Gets the title of the window. */
	String GetTitle() const;

	/** Sets the title of the window. */
	void SetTitle(const String& title);
	//@}

	/** @name Window bound dimmensions. */
	//@{
	/** Gets the size and location of the window. */
	RectangleInt GetWindowBounds() const;

	/** Sets the size and location of the window. */
	void SetWindowBounds(const RectangleInt& bounds);

	/** Gets the width of the window. */
	int32 GetWindowWidth() const;

	/** Sets the width of the window. */
	void SetWindowWidth(int32 width) const;

	/** Gets the height of the window. */
	int32 GetWindowHeight() const;

	/** Sets the height of the window. */
	void SetWindowHeight(int32 height) const;

	/** Sets the location of the window. */
	void SetWindowLocation(int32 x, int32 y);

	/** Sets the size of the window. */
	void SetWindowSize(int32 width, int32 height);
	//@}

	/** @name Client area dimmensions. */
	//@{
	/** Gets the rectangle that represents the client area of the window. */
	RectangleInt GetClientRectangle() const;

	/** Gets the width of the client area of the window. */
	int32 GetClientWidth() const;

	/** Gets the height of the client area of the window. */
	int32 GetClientHeight() const;
	//@}

	/** @name Status */
	//@{
	/** Gets the active state. */
	bool IsActive() const;

	/** Activate the window. */
	void Activate();
	//@}

	/** Creates a new window using the specified attributes. */
	bool Create(int32 width, int32 height, WindowStyle style = WindowStyle_Default);
	bool Create(const String& title, int32 width, int32 height, WindowStyle style = WindowStyle_Default);
	bool Create(const String& title, int32 x, int32 y, int32 width, int32 height, WindowStyle style = WindowStyle_Default);

	/** Closes and destroys the window. */
	void Destroy();

	/** Process the system message loop. */
	bool Update();

	/** @name Events. */
	//@{
	/** Called when the window is created. */
	DefaultEvent Created;

	/** Called when the window is destroyed. */
	DefaultEvent Destroyed;

	/** Called when the window is closed. */
	DefaultEvent Closed;

	/** Called after the window's size has changed. */
	DefaultEvent Sized;
	//@}

private:
	WindowInternal* _internal;
};

}

#endif 
