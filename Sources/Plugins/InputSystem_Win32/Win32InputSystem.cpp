/*=============================================================================
Win32InputSystem.cpp
Project: Sonata Engine
Copyright (c) 2005
Julien Delezenne
=============================================================================*/

#include "Win32InputSystem.h"
#include "Win32InputKeyboard.h"
#include "Win32InputMouse.h"
#include "Win32InputGamepad.h"

namespace SE_Win32
{

Win32InputSystem::Win32InputSystem() :
	InputSystem()
{
	Init();
}

Win32InputSystem::~Win32InputSystem()
{
	Destroy();
}

void Win32InputSystem::Init()
{
	_CursorLocked = false;
}

bool Win32InputSystem::Create()
{
	if (!EnumDevices())
		return false;

	return true;
}

void Win32InputSystem::Destroy()
{
	// Release all devices
	RemoveAllInputDevices();
}

bool Win32InputSystem::EnumDevices()
{

	// Detect the devices
	if (::GetSystemMetrics(SM_MOUSEPRESENT) != FALSE)
	{
		Win32InputMouse* inputMouse = new Win32InputMouse(this);
		AddInputDevice(inputMouse);
	}

	Win32InputKeyboard* inputKeyboard = new Win32InputKeyboard(this);
	AddInputDevice(inputKeyboard);

	JOYINFOEX joyInfoEx;
	::ZeroMemory(&joyInfoEx, sizeof(joyInfoEx));
	joyInfoEx.dwSize = sizeof(joyInfoEx);
	UINT iJoyCount = ::joyGetNumDevs();
	for (UINT i = 0; i < iJoyCount; i++)
	{
		MMRESULT res = ::joyGetPosEx(JOYSTICKID1 + i, &joyInfoEx);
		if (res == JOYERR_NOERROR)
		{
			Win32InputGamepad* inputGamepad = new Win32InputGamepad(this);
			//inputGamepad->SetJoystickID(i);
			AddInputDevice(inputGamepad);
		}
	}

	return true;
}

void Win32InputSystem::SetCursorVisible(bool value)
{
	::ShowCursor(value);
}

void Win32InputSystem::SetCursorLocked(bool value)
{
	_CursorLocked = value;
}

void Win32InputSystem::DestroyInputDevice(InputDevice* inputDevice)
{
	delete inputDevice;
}

}
