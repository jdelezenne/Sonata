/*=============================================================================
DI8InputSystem.cpp
Project: Sonata Engine
Copyright (c) 2005
Julien Delezenne
=============================================================================*/

#include "DI8InputSystem.h"
#include "DI8InputKeyboard.h"
#include "DI8InputMouse.h"
#include "DI8InputGamepad.h"

namespace SE_pDI8
{

DI8InputSystem::DI8InputSystem() :
	InputSystem()
{
	Initialize();
}

DI8InputSystem::~DI8InputSystem()
{
	Destroy();
}

void DI8InputSystem::Initialize()
{
	_pDI = NULL;
	_cleanupCOM = false;
	_isCursorLocked = false;
}

bool DI8InputSystem::Create()
{
	HRESULT hr;

	// Initialize the COM library
	hr = CoInitialize(NULL);
	_cleanupCOM = SUCCEEDED(hr);

	// Creates a DirectInput object 
	hr = DirectInput8Create(
		GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
		IID_IDirectInput8, (VOID**)&_pDI, NULL);
	if (FAILED(hr))
	{
		return false;
	}

	if (!EnumDevices())
	{
		return false;
	}

	return true;
}

void DI8InputSystem::Destroy()
{
	// Release all devices
	RemoveAllInputDevices();

	// Release DirectInput object
	SE_RELEASE(_pDI);

	if (_cleanupCOM)
	{
		// Close the COM library
		CoUninitialize();
		_cleanupCOM = false;
	}
}

BOOL CALLBACK DI8InputSystem::DIEnumDevicesCallback(LPCDIDEVICEINSTANCE pDIDI, LPVOID pvRef)
{
	DI8InputSystem* manager = (DI8InputSystem*) pvRef;

	if (manager != NULL)
	{
		manager->AddDIDevice(pDIDI);
	}

	return DIENUM_CONTINUE;
}

bool DI8InputSystem::EnumDevices()
{
	if (_pDI == NULL)
	{
		return false;
	}

	// Enumerates attached devices
	if (FAILED(_pDI->EnumDevices(DI8DEVCLASS_ALL, DIEnumDevicesCallback, this, DIEDFL_ATTACHEDONLY)))
	{
		return false;
	}

	return true;
}

void DI8InputSystem::SetCursorVisible(bool value)
{
	::ShowCursor(value);
}

void DI8InputSystem::SetCursorLocked(bool value)
{
	_isCursorLocked = value;
}

bool DI8InputSystem::AddDIDevice(LPCDIDEVICEINSTANCE pDIDI)
{
	InputDevice* inputDevice = NULL;

	switch (GET_DIDEVICE_TYPE(pDIDI->dwDevType))
	{
	case DI8DEVTYPE_MOUSE:
		inputDevice = new DI8InputMouse(this, pDIDI);
		break;

	case DI8DEVTYPE_KEYBOARD:
		inputDevice = new DI8InputKeyboard(this, pDIDI);
		break;

	case DI8DEVTYPE_JOYSTICK:
		inputDevice = new DI8InputGamepad(this, pDIDI);
		break;

	default:
		// Unknown device!
		return false;
	}

	if (inputDevice != NULL)
	{
		_AddInputDevice(inputDevice);
	}

	return true;
}

void DI8InputSystem::_DestroyInputDevice(InputDevice* inputDevice)
{
	SE_DELETE(inputDevice);
}

}
