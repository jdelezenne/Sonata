/*=============================================================================
Win32InputMouse.cpp
Project: Sonata Engine
Copyright (c) 2005
Julien Delezenne
=============================================================================*/

#include "Win32InputMouse.h"
#include "Win32InputSystem.h"

namespace SE_Win32
{

Win32InputMouse::Win32InputMouse(InputSystem* inputManager):
	InputMouse(inputManager)
{
	Init();
}

Win32InputMouse::~Win32InputMouse()
{
	Destroy();
}

void Win32InputMouse::Init()
{
	::ZeroMemory(&_Buttons, sizeof(ulong)*3);
	ZeroMemory(&_PrevButtons, sizeof(ulong)*3);
	ZeroMemory(&_Position, sizeof(PointInt));
	ZeroMemory(&_PrevPosition, sizeof(PointInt));
	_Wheel = 0;
}

bool Win32InputMouse::ReadBuffered()
{
	return false;
}

bool Win32InputMouse::ReadImmediate()
{
	_PrevButtons[0] = _Buttons[0];
	_PrevButtons[1] = _Buttons[1];
	_PrevButtons[2] = _Buttons[2];

	_PrevPosition.X = _Position.X;
	_PrevPosition.Y = _Position.Y;

	_Buttons[0] = 0;
	_Buttons[1] = 0;
	_Buttons[2] = 0;

	ulong timeValue = ((real64)TimeValue::GetTime() * 1000.0);

	if ((::GetAsyncKeyState(VK_LBUTTON) && 0x80) != 0)
	{
		if (_Buttons[0] == 0)
			_Buttons[0] = timeValue;
	}

	if ((::GetAsyncKeyState(VK_RBUTTON) && 0x80) != 0)
	{
		if (_Buttons[1] == 0)
			_Buttons[1] = timeValue;
	}

	if ((::GetAsyncKeyState(VK_MBUTTON) && 0x80) != 0)
	{
		if (_Buttons[2] == 0)
			_Buttons[2] = timeValue;
	}

	POINT pt;
	::GetCursorPos(&pt);
	::ScreenToClient((HWND)_Window->GetHandle(), &pt);

	_Position.X = pt.x;
	_Position.Y = pt.y;

	return true;
}

String Win32InputMouse::GetProductName() const
{
	return String::Empty;
}

String Win32InputMouse::GetGUID() const
{
	return String::Empty;
}

bool Win32InputMouse::Acquire()
{
	return true;
}

bool Win32InputMouse::Unacquire()
{
	return true;
}

bool Win32InputMouse::Read()
{
	if (!_Created)
		return false;

	if (_Buffered)
		return ReadBuffered();
	else
		return ReadImmediate();
}

bool Win32InputMouse::Create(Window* window)
{
	if (_Created)
		return false;

	if (window == NULL)
		return false;

	if (_InputSystem == NULL)
		return false;

	_Window = window;
	_Created = true;

    POINT pt;    
	::GetCursorPos(&pt);
    
	::ScreenToClient((HWND)_Window->GetHandle(), &pt);
	_Position.X = pt.x;
	_Position.Y = pt.y;

	return true;
}

bool Win32InputMouse::Destroy()
{
	if (!_Created)
		return false;

	Init();

	_Created = false;

	return true;
}

void Win32InputMouse::SetBuffered(bool value)
{
	_Buffered = value;
}

bool Win32InputMouse::IsMouseButtonDown(MouseButton button, ulong* buttonTime) const
{
	switch (button)
	{
	case MouseButton_Left:
		return _Buttons[0] != 0;
	case MouseButton_Right:
		return _Buttons[1] != 0;
	case MouseButton_Middle:
		return _Buttons[2] != 0;
	default:
		return 0;
	}
}

bool Win32InputMouse::IsMouseButtonUp(MouseButton button) const
{
	return !IsMouseButtonDown(button);
}


bool Win32InputMouse::IsMouseClicked(MouseButton button) const
{
	switch (button)
	{
	case MouseButton_Left:
		return _Buttons[0] != 0 && _PrevButtons[0] == 0;
	case MouseButton_Right:
		return _Buttons[1] != 0 && _PrevButtons[1] == 0;
	case MouseButton_Middle:
		return _Buttons[2] != 0 && _PrevButtons[2] == 0;
	default:
		return 0;
	}
}

bool Win32InputMouse::IsMouseToggled(MouseButton button) const
{
	switch (button)
	{
	case MouseButton_Left:
		return _Buttons[0] != _PrevButtons[0];
	case MouseButton_Right:
		return _Buttons[1] != _PrevButtons[1];
	case MouseButton_Middle:
		return _Buttons[2] != _PrevButtons[2];
	default:
		return 0;
	}
}

void Win32InputMouse::GetMousePosition(int32& x, int32& y) const
{
	x = _Position.X;
	y = _Position.Y;
}

void Win32InputMouse::GetMouseRelativePosition(int32& x, int32& y) const
{
	x = _Position.X - _PrevPosition.X;
	y = _Position.Y - _PrevPosition.Y;
}

int32 Win32InputMouse::GetMouseWheel() const
{
	return _Wheel;
}

}
