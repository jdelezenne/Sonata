/*=============================================================================
NullWindow.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Core/System/Window.h"

namespace SonataEngine
{

class WindowInternal
{
};


Window::Window() :
	_internal(NULL)
{
}

Window::Window(WindowHandle handle) :
	_internal(NULL)
{
}

Window::~Window()
{
}

WindowHandle Window::GetHandle() const
{
	return (WindowHandle)NULL;
}

String Window::GetTitle() const
{
	return String::Empty;
}

void Window::SetTitle(const String& title)
{
}

RectangleInt Window::GetWindowBounds() const
{
	return RectangleInt::Empty;
}

int32 Window::GetWindowWidth() const
{
	return 0;
}

int32 Window::GetWindowHeight() const
{
	return 0;
}

void Window::SetWindowBounds(const RectangleInt& rectangle)
{
}

void Window::SetWindowLocation(int32 x, int32 y)
{
}

void Window::SetWindowSize(int32 width, int32 height)
{
}

void Window::SetWindowWidth(int32 width) const
{
}

void Window::SetWindowHeight(int32 height) const
{
}


RectangleInt Window::GetClientRectangle() const
{
	return RectangleInt::Empty;
}

int32 Window::GetClientWidth() const
{
	return 0;
}

int32 Window::GetClientHeight() const
{
	return 0;
}

bool Window::IsActive() const
{
	return false;
}

void Window::Activate()
{
}

bool Window::Create(int32 width, int32 height, WindowStyle style)
{
	return true;
}

bool Window::Create(const String& title, int32 width, int32 height, WindowStyle style)
{
	return true;
}

bool Window::Create(const String& title, int32 x, int32 y, int32 width, int32 height, WindowStyle style)
{
	return true;
}

void Window::Destroy()
{
}

bool Window::Update()
{
	return true;
}

}
