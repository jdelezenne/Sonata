/*=============================================================================
Win32Window.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Win32Platform.h"
#include "Win32Helper.h"
#include "Core/System/Window.h"
#include "Core/Logging/Logger.h"
#include "Core/IO/Path.h"

namespace SonataEngine
{

class WindowInternal
{
public:
	WindowInternal();

public:
	HWND _hWnd;
};


WindowInternal::WindowInternal() :
	_hWnd(NULL)
{
}


#define SE_WIN32CLASSNAME "SE_WindowClass"
#define SE_WM_CREATE      WM_APP+100

LRESULT MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Window* window = NULL;

	if (uMsg != WM_CREATE)
	{
		// Gets the pointer to the instance of this window.
#ifdef _WIN64
		window = (Window*)(LONG_PTR)::GetWindowLongPtr(hWnd, GWL_USERDATA);
#else
		window = (Window*)(LONG_PTR)(LONG)::GetWindowLongPtr(hWnd, GWL_USERDATA);
#endif 
	}

	if (window == NULL)
	{
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	switch (uMsg)
	{
	case SE_WM_CREATE:
		window->Created.Invoke((Object*)window, EventArgs::Empty);
		break;

	case WM_DESTROY:
		window->Destroyed.Invoke((Object*)window, EventArgs::Empty);
		break;

	case WM_CLOSE:
		window->Closed.Invoke((Object*)window, EventArgs::Empty);
		break;

	case WM_ACTIVATE:
		if (wParam == WA_INACTIVE)
		{
		}
		else
		{
		}
		break;

	case WM_SIZE:
		window->Sized.Invoke((Object*)window, EventArgs::Empty);
		break;
	}

	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}


Window::Window() :
	_internal(new WindowInternal())
{
}

Window::Window(WindowHandle handle) :
	_internal(new WindowInternal())
{
	_internal->_hWnd = (HWND)handle;

#ifdef _WIN64
	LONG_PTR nResult = ::SetWindowLongPtr((HWND)_internal->_hWnd, GWL_USERDATA, (LONG_PTR)this);
#else
	LONG_PTR nResult = ::SetWindowLongPtr((HWND)_internal->_hWnd, GWL_USERDATA, (LONG)(LONG_PTR)this);
#endif 
}

Window::~Window()
{
	delete _internal;
}

WindowHandle Window::GetHandle() const
{
	return (WindowHandle)_internal->_hWnd;
}

String Window::GetTitle() const
{
	if (_internal->_hWnd == NULL)
	{
		return String::Empty;
	}

	String title;
	TCHAR* pszTitle;
	int nLength = ::GetWindowTextLength(_internal->_hWnd);

	if (nLength > 0)
	{
		pszTitle = new TCHAR[nLength];
		::GetWindowText(_internal->_hWnd, pszTitle, nLength);
		title = pszTitle;
		delete pszTitle;
	}

	return title;
}

void Window::SetTitle(const String& title)
{
	if (_internal->_hWnd != NULL)
	{
		::SetWindowText(_internal->_hWnd, title.Data());
	}
}

RectangleInt Window::GetWindowBounds() const
{
	if (_internal->_hWnd == NULL)
	{
		return RectangleInt::Empty;
	}

	RECT rc;
	::GetWindowRect(_internal->_hWnd, &rc);
	return Win32Helper::MakeRect(rc);
}

int32 Window::GetWindowWidth() const
{
	if (_internal->_hWnd == NULL)
	{
		return 0;
	}

	RECT rc;
	::GetWindowRect(_internal->_hWnd, &rc);
	return (rc.right - rc.left);
}

int32 Window::GetWindowHeight() const
{
	if (_internal->_hWnd == NULL)
	{
		return 0;
	}

	RECT rc;
	::GetWindowRect(_internal->_hWnd, &rc);
	return (rc.bottom - rc.top);
}

void Window::SetWindowBounds(const RectangleInt& rectangle)
{
	if (_internal->_hWnd == NULL)
	{
		return;
	}

	::SetWindowPos(
		_internal->_hWnd,
		NULL,
		rectangle.X,
		rectangle.Y,
		rectangle.Width,
		rectangle.Height,
		SWP_NOACTIVATE);
}

void Window::SetWindowLocation(int32 x, int32 y)
{
	if (_internal->_hWnd == NULL)
	{
		return;
	}

	::SetWindowPos(
		_internal->_hWnd,
		NULL,
		x,
		y,
		0,
		0,
		SWP_NOACTIVATE | SWP_NOSIZE);
}

void Window::SetWindowSize(int32 width, int32 height)
{
	if (_internal->_hWnd == NULL)
	{
		return;
	}

	::SetWindowPos(
		_internal->_hWnd,
		NULL,
		0,
		0,
		width,
		height,
		SWP_NOACTIVATE | SWP_NOMOVE);
}

void Window::SetWindowWidth(int32 width) const
{
	if (_internal->_hWnd == NULL)
	{
		return;
	}

	RECT rc;
	::GetWindowRect(_internal->_hWnd, &rc);

	::SetWindowPos(
		_internal->_hWnd,
		NULL,
		0,
		0,
		width,
		rc.bottom - rc.top,
		SWP_NOACTIVATE | SWP_NOMOVE);
}

void Window::SetWindowHeight(int32 height) const
{
	if (_internal->_hWnd == NULL)
	{
		return;
	}

	RECT rc;
	::GetWindowRect(_internal->_hWnd, &rc);

	::SetWindowPos(
		_internal->_hWnd,
		NULL,
		0,
		0,
		rc.right - rc.left,
		height,
		SWP_NOACTIVATE | SWP_NOMOVE);
}


RectangleInt Window::GetClientRectangle() const
{
	if (_internal->_hWnd == NULL)
	{
		return RectangleInt::Empty;
	}

	RECT rc;
	GetClientRect(_internal->_hWnd, &rc);
	return Win32Helper::MakeRect(rc);
}

int32 Window::GetClientWidth() const
{
	if (_internal->_hWnd == NULL)
		return 0;

	RECT rc;
	::GetClientRect(_internal->_hWnd, &rc);
	return rc.right - rc.left;
}

int32 Window::GetClientHeight() const
{
	if (_internal->_hWnd == NULL)
	{
		return 0;
	}

	RECT rc;
	::GetClientRect(_internal->_hWnd, &rc);
	return rc.bottom - rc.top;
}

bool Window::IsActive() const
{
	if (_internal->_hWnd == NULL)
	{
		return false;
	}

	return (::GetActiveWindow() == _internal->_hWnd);
}

void Window::Activate()
{
	if (_internal->_hWnd == NULL)
	{
		return;
	}

	::SetWindowPos(
		_internal->_hWnd,
		HWND_TOP,
		0,
		0,
		0,
		0,
		SWP_NOMOVE | SWP_NOSIZE);
}

bool Window::Create(int32 width, int32 height, WindowStyle style)
{
	String title = Environment::GetApplicationFileName();
	title = Path::GetFileNameWithoutExtension(title);

	return Create(title, CW_USEDEFAULT, CW_USEDEFAULT, width, height, style);
}

bool Window::Create(const String& title, int32 width, int32 height, WindowStyle style)
{
	return Create(title, CW_USEDEFAULT, CW_USEDEFAULT, width, height, style);
}

bool Window::Create(const String& title, int32 x, int32 y, int32 width, int32 height, WindowStyle style)
{
	if (_internal->_hWnd != NULL)
	{
		return false;
	}

	WNDPROC WndProc = (WNDPROC)MsgProc;
	HINSTANCE hInstance = ::GetModuleHandle(NULL);
	WORD iIcon = 0;
	HICON hIcon = ::ExtractAssociatedIcon(hInstance, (LPSTR)Environment::GetApplicationFileName().Data(), &iIcon);

	// Create and register the window class.
	WNDCLASS wndClass;
	wndClass.style = 0;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	// bytes reserved for the pointer to the application instance.
	wndClass.cbWndExtra = sizeof(SEptr);
	wndClass.hInstance = hInstance;
	wndClass.hIcon = hIcon;
	wndClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = SE_WIN32CLASSNAME;
	::RegisterClass(&wndClass);

	// Initial window styles.
	DWORD dwStyle;
	dwStyle = (WS_POPUP | WS_VISIBLE);
	if ((style & WindowStyle_Borders) != 0)
	{
		if ((style & WindowStyle_Resize) != 0)
		{
			dwStyle |= WS_THICKFRAME;
		}
		else
		{
			dwStyle |= WS_BORDER;
		}
	}
	if ((style & WindowStyle_Caption) != 0)
	{
		dwStyle |= (WS_CAPTION | WS_SYSMENU);
	}
	if ((style & WindowStyle_Minimize) != 0)
	{
		dwStyle |= WS_MINIMIZEBOX;
	}
	if ((style & WindowStyle_Maximize) != 0)
	{
		dwStyle |= WS_MAXIMIZEBOX;
	}

	RECT rc = { 0, 0, width, height };
	::AdjustWindowRect(&rc, dwStyle, FALSE);

	if (x == -1)
	{
		x = CW_USEDEFAULT;
	}
	if (y == -1)
	{
		y = CW_USEDEFAULT;
	}
	if (width == -1)
	{
		width = CW_USEDEFAULT;
	}
	if (height == -1)
	{
		height = CW_USEDEFAULT;
	}

	// Create the window.
	HWND hWnd = ::CreateWindow(
		SE_WIN32CLASSNAME,
		title.Data(), 
		dwStyle,
		x,
		y, 
		rc.right-rc.left,
		rc.bottom-rc.top,
		0,
		NULL,
		hInstance,
		this);

	if (hWnd == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("Window.Create"),
			_T("Failed to create the window."));
		return false;
	}
	_internal->_hWnd = hWnd;

	// Set 'this' as extra data.
#ifdef _WIN64
	LONG_PTR nResult = ::SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)this);
#else
	LONG_PTR nResult = ::SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG)(LONG_PTR)this);
#endif 

	DWORD dwError = ::GetLastError();

 	// Notify the creation of the window
	::SendMessage(hWnd, SE_WM_CREATE, 0, 0);

	::ShowWindow(hWnd, SW_SHOW);
	::UpdateWindow(hWnd);

	return true;
}

void Window::Destroy()
{
	::UnregisterClass(SE_WIN32CLASSNAME, GetModuleHandle(NULL));

	if (_internal->_hWnd != NULL)
	{
		::DestroyWindow(_internal->_hWnd);
		_internal->_hWnd = NULL;
	}
}

bool Window::Update()
{
	MSG msg;

	// Enter the message loop
	while (::PeekMessage(&msg, _internal->_hWnd, 0, 0, PM_NOREMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return false;
		}
		else if (::Win32GetMessage(&msg, _internal->_hWnd, 0, 0))
		{
			// Translate and send the message.
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	return true;
}

}
