/*=============================================================================
Win32InputGamepad.cpp
Project: Sonata Engine
Copyright (c) 2005
Julien Delezenne
=============================================================================*/

#include "Win32InputGamepad.h"
#include "Win32InputSystem.h"

namespace SE_Win32
{

Win32InputGamepad::Win32InputGamepad(InputSystem* inputManager):
	InputGamepad(inputManager)
{
}

Win32InputGamepad::~Win32InputGamepad()
{
}

bool Win32InputGamepad::ReadBuffered()
{
	return false;
}

bool Win32InputGamepad::ReadImmediate()
{
	return false;
}

void Win32InputGamepad::SetBuffered(bool value)
{
}

String Win32InputGamepad::GetProductName() const
{
	return String::Empty;
}

String Win32InputGamepad::GetGUID() const
{
	return String::Empty;
}

bool Win32InputGamepad::Acquire()
{
	return true;
}

bool Win32InputGamepad::Unacquire()
{
	return true;
}

bool Win32InputGamepad::Read()
{
	if (_Buffered)
		return ReadBuffered();
	else
		return ReadImmediate();
}

bool Win32InputGamepad::Create(Window* window)
{
	if (_Created)
		return false;

	if (window == NULL)
		return false;

	_Window = window;
	_Created = true;

	return true;
}

bool Win32InputGamepad::Destroy()
{
	if (!_Created)
		return false;

	_Created = false;

	return true;
}

void Win32InputGamepad::GetGamepadObjectList(GamepadObjectList& objectList) const
{
}

void Win32InputGamepad::GetGamepadObjectList(GamepadObjectType type, GamepadObjectList& objectList) const
{
}

int32 Win32InputGamepad::GetButtonCount() const
{
	return 0;
}

int32 Win32InputGamepad::GetAxisCount() const
{
	return 0;
}

int32 Win32InputGamepad::GetPOVCount() const
{
	return 0;
}

bool Win32InputGamepad::IsButtonDown(int32 button, ulong* buttonTime) const
{
	if (button < 0 || button > GetButtonCount()-1)
		return false;

	return false;
}

bool Win32InputGamepad::IsButtonUp(int32 button) const
{
	return !IsButtonDown(button);
}

bool Win32InputGamepad::IsButtonPushed(int32 button) const
{
	if (button < 0 || button > GetButtonCount()-1)
		return false;

	return false;
}

bool Win32InputGamepad::IsButtonToggled(int32 button) const
{
	if (button < 0 || button > GetButtonCount()-1)
		return false;

	return false;
}

bool Win32InputGamepad::IsAxisPosition(GamepadAxis axis) const
{
	return false;
}

bool Win32InputGamepad::IsAxisRotation(GamepadAxis axis) const
{
	return false;
}

int32 Win32InputGamepad::GetAxisPosition(GamepadAxis axis) const
{
	return 0;
}

int32 Win32InputGamepad::GetAxisRotation(GamepadAxis axis) const
{
	return 0;
}

int32 Win32InputGamepad::GetSlider(int32 slider) const
{
	if (slider < 0 || slider > GetAxisCount()-1)
		return 0;

	return 0;
}

int32 Win32InputGamepad::GetPointOfView(int32 pov) const
{
	if (pov < 0 || pov > GetPOVCount()-1)
		return 0;

	return 0;
}

}
