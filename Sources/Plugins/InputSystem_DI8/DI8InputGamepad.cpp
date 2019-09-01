/*=============================================================================
DI8InputGamepad.cpp
Project: Sonata Engine
Copyright (c) 2005
Julien Delezenne
=============================================================================*/

#include "DI8InputGamepad.h"
#include "DI8InputSystem.h"

namespace SE_pDI8
{

DI8InputGamepad::DI8InputGamepad(InputSystem* inputManager, LPCDIDEVICEINSTANCE pDIDI):
	InputGamepad(inputManager)
{
	_pDIDI = new DIDEVICEINSTANCE;
	Memory::Copy((void*)_pDIDI, (void*)pDIDI, sizeof(DIDEVICEINSTANCE));
	_pDID = NULL;
	_sliderCount = 0;
	Memory::Zero(&_axisPosition, sizeof(bool) * 3);
	Memory::Zero(&_axisRotation, sizeof(bool) * 3);

	Memory::Zero(&_DIJoyState, sizeof(DIJOYSTATE2));
	Memory::Zero(&_DILastJoyState, sizeof(DIJOYSTATE2));
}

DI8InputGamepad::~DI8InputGamepad()
{
	SE_DELETE(_pDIDI);
}

BOOL CALLBACK DI8InputGamepad::DIEnumDeviceObjectsCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	DI8InputGamepad* gamepad = (DI8InputGamepad*)pvRef;

	if (gamepad)
	{
		gamepad->AddInputObject(lpddoi);
	}

	return DIENUM_CONTINUE;
}

bool DI8InputGamepad::EnumObjects()
{
	if (_pDID == NULL)
	{
		return false;
	}

	_sliderCount = 0;

	// Enumerates device objects.
	if (FAILED(_pDID->EnumObjects(DIEnumDeviceObjectsCallback, this, DIDFT_ALL)))
	{
		return false;
	}

	return true;
}

void DI8InputGamepad::AddInputObject(LPCDIDEVICEOBJECTINSTANCE lpddoi)
{
	GamepadObject gamepadObject;

	gamepadObject.Index = 0;
	gamepadObject.Name = lpddoi->tszName;

	if (lpddoi->guidType == GUID_Button || 
		lpddoi->guidType == GUID_Key || 
		lpddoi->guidType == GUID_POV)
	{
		gamepadObject.RangeMin = DIPROPRANGE_NOMIN;
		gamepadObject.RangeMax = DIPROPRANGE_NOMAX;
	}
	else
	{
		DIPROPRANGE dipr;
		dipr.diph.dwSize = sizeof(DIPROPRANGE); 
		dipr.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
		dipr.diph.dwHow = DIPH_BYID; 
		dipr.diph.dwObj = lpddoi->dwType; 

		if (SUCCEEDED(_pDID->GetProperty(DIPROP_RANGE, &dipr.diph)))
		{
			gamepadObject.RangeMin = dipr.lMin;
			gamepadObject.RangeMax = dipr.lMax;
		}
		else
		{
			gamepadObject.RangeMin = DIPROPRANGE_NOMIN;
			gamepadObject.RangeMax = DIPROPRANGE_NOMAX;
		}
	}

	if (lpddoi->guidType == GUID_XAxis)
	{
		gamepadObject.Type = GamepadObjectType_XAxis;
		_axisPosition[0] = true;
	}
	else if (lpddoi->guidType == GUID_YAxis)
	{
		gamepadObject.Type = GamepadObjectType_YAxis;
		_axisPosition[1] = true;
	}
	else if (lpddoi->guidType == GUID_ZAxis)
	{
		gamepadObject.Type = GamepadObjectType_ZAxis;
		_axisPosition[2] = true;
	}

	else if (lpddoi->guidType == GUID_RxAxis)
	{
		gamepadObject.Type = GamepadObjectType_RxAxis;
		_axisRotation[0] = true;
	}
	else if (lpddoi->guidType == GUID_RyAxis)
	{
		gamepadObject.Type = GamepadObjectType_RyAxis;
		_axisRotation[1] = true;
	}
	else if (lpddoi->guidType == GUID_RzAxis)
	{
		gamepadObject.Type = GamepadObjectType_RzAxis;
		_axisRotation[2] = true;
	}

	else if (lpddoi->guidType == GUID_Slider)
	{
		gamepadObject.Type = GamepadObjectType_Slider;
		_sliderCount++;
	}

	else if (lpddoi->guidType == GUID_Button)
	{
		gamepadObject.Type = GamepadObjectType_Button;
	}
	else if (lpddoi->guidType == GUID_Key)
	{
		gamepadObject.Type = GamepadObjectType_Key;
	}
	else if (lpddoi->guidType == GUID_POV)
	{
		gamepadObject.Type = GamepadObjectType_POV;
	}
	else
	{
		gamepadObject.Type = GamepadObjectType_Unknown;
	}

	_GamepadObjects.Add(gamepadObject);
}

bool DI8InputGamepad::ReadBuffered()
{
	DIDEVICEOBJECTDATA didod[DI8_INPUT_BUFFER_SIZE]; // Receives buffered data 
	DWORD dwElements;
	HRESULT hr;

	if (_pDID == NULL)
	{
		return false;
	}

	// Retrieves buffered data from the device.
	dwElements = DI8_INPUT_BUFFER_SIZE;
	hr = _pDID->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), didod, &dwElements, 0);
	if (hr != DI_OK) 
	{
		hr = _pDID->Acquire();
		while (hr == DIERR_INPUTLOST)
		{
			hr = _pDID->Acquire();
		}

		if (hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED)
		{
			return false; // Unacquired
		}

		return true; 
	}

	for (DWORD i = 0; i < dwElements; i++)
	{
		for (int j = 0; j <= 31; j++)
		{
			if (didod[i].dwOfs == DIJOFS_BUTTON(j))
			{
				_DIJoyState.rgbButtons[j] = (didod[i].dwData & 0x80) != 0 ? didod[i].dwTimeStamp : 0;
				break;
			}
		}

		switch (didod[i].dwOfs)
		{
			case DIJOFS_X:
				_DIJoyState.lX = (didod[i].dwData & 0x80) != 0 ? didod[i].dwTimeStamp : 0;
				break;

			case DIJOFS_Y:
				_DIJoyState.lY = (didod[i].dwData & 0x80) != 0 ? didod[i].dwTimeStamp : 0;
				break;

			case DIJOFS_Z:
				_DIJoyState.lZ = (didod[i].dwData & 0x80) != 0 ? didod[i].dwTimeStamp : 0;
				break;

			case DIJOFS_RX:
				_DIJoyState.lRx = (didod[i].dwData & 0x80) != 0 ? didod[i].dwTimeStamp : 0;
				break;

			case DIJOFS_RY:
				_DIJoyState.lRy = (didod[i].dwData & 0x80) != 0 ? didod[i].dwTimeStamp : 0;
				break;

			case DIJOFS_RZ:
				_DIJoyState.lRz = (didod[i].dwData & 0x80) != 0 ? didod[i].dwTimeStamp : 0;
				break;
		}
	}

	return S_OK;
}

bool DI8InputGamepad::ReadImmediate()
{
	HRESULT hr;

	if (_pDID == NULL)
	{
		return false;
	}

	// Poll the device to read the current state
	hr = _pDID->Poll(); 

	// If failed, acquire the joystick again.
	if (FAILED(hr))
	{ 
		hr = _pDID->Acquire();
		while (hr == DIERR_INPUTLOST)
		{
			hr = _pDID->Acquire();
		}

		if (hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED)
		{
			return false; // Unacquired
		}
	}

	// Store the last joystick state.
	Memory::Copy(&_DILastJoyState, &_DIJoyState, sizeof(DIJOYSTATE2));

	// Retrieve the joystick state.
	Memory::Zero(&_DIJoyState, sizeof(_DIJoyState));
	hr = _pDID->GetDeviceState(sizeof(DIJOYSTATE2), (LPVOID)&_DIJoyState);

	return true;
}

String DI8InputGamepad::GetProductName() const
{
	if (_pDIDI == NULL)
	{
		return String::Empty;
	}

	return _pDIDI->tszProductName;
}

String DI8InputGamepad::GetGUID() const
{
	if (_pDID == NULL)
	{
		return String::Empty;
	}

	return String::Empty;
}

bool DI8InputGamepad::Acquire()
{
	if (_pDID == NULL || _window == NULL)
	{
		return false;
	}

	if (_state != InputDeviceState_Ready)
	{
		return false;
	}

	HRESULT hr;
	if (FAILED(hr = _pDID->SetCooperativeLevel((HWND) _window->GetHandle(), DISCL_BACKGROUND | DISCL_EXCLUSIVE)))
	{
		return false;
	}

	if (FAILED(hr = _pDID->Acquire()))
	{
		return false;
	}

	return true;
}

bool DI8InputGamepad::Unacquire()
{
	if (_pDID == NULL)
	{
		return false;
	}

	if (_state != InputDeviceState_Ready)
	{
		return false;
	}

	HRESULT hr;
	if (FAILED(hr = _pDID->Unacquire()))
	{
		return false;
	}

	return true;
}

bool DI8InputGamepad::Read()
{
	return ReadImmediate();
}

bool DI8InputGamepad::Create(Window* window)
{
	if (window == NULL)
		return false;

	if (_inputSystem == NULL || _pDID == NULL)
		return false;

	if (_state != InputDeviceState_Ready)
	{
		return false;
	}

	HRESULT hr;

	if (FAILED(hr = ((DI8InputSystem*)_inputSystem)->GetDIObject()->CreateDevice(_pDIDI->guidInstance, &_pDID, NULL)))
	{
		return false;
	}

	if (FAILED(hr = _pDID->SetDataFormat(&c_dfDIJoystick2)))
	{
		return false;
	}

	_DIDevCaps.dwSize = sizeof(DIDEVCAPS);
	if (FAILED(hr = _pDID->GetCapabilities(&_DIDevCaps)))
	{
		return false;
	}

	_window = window;
	_state = InputDeviceState_Ready;

	EnumObjects();

	if (!Acquire())
	{
		return false;
	}

	return true;
}

bool DI8InputGamepad::Destroy()
{
	if (_state != InputDeviceState_Ready)
	{
		return false;
	}

	_pDID->Unacquire();
	SE_RELEASE(_pDID);

	_sliderCount = 0;
	Memory::Zero(&_axisPosition, 3*sizeof(bool));
	Memory::Zero(&_axisRotation, 3*sizeof(bool));

	Memory::Zero(&_DIJoyState, sizeof(DIJOYSTATE2));
	Memory::Zero(&_DILastJoyState, sizeof(DIJOYSTATE2));

	_state = InputDeviceState_Unknown;

	return true;
}

void DI8InputGamepad::SetBuffered(bool value)
{
}

void DI8InputGamepad::GetGamepadObjectList(GamepadObjectList& objectList) const
{
	objectList = _GamepadObjects;
}

void DI8InputGamepad::GetGamepadObjectList(GamepadObjectType type, GamepadObjectList& objectList) const
{
	GamepadObjectList::Iterator it = _GamepadObjects.GetIterator();
	while (it.Next())
	{
		if (it.Current().Type == type)
		{
			objectList.Add(it.Current());
		}
	}
}

int32 DI8InputGamepad::GetButtonCount() const
{
	return _DIDevCaps.dwButtons;
}

int32 DI8InputGamepad::GetAxisCount() const
{
	return _DIDevCaps.dwAxes;
}

int32 DI8InputGamepad::GetPOVCount() const
{
	return _DIDevCaps.dwPOVs;
}

bool DI8InputGamepad::IsButtonDown(int32 button, uint32* buttonTime) const
{
	if (button < 0 || button > GetButtonCount()-1)
		return false;

	if (buttonTime != NULL)
	{
		Memory::Copy(buttonTime, (void*)&_DIJoyState.rgbButtons[button], sizeof(buttonTime));
	}

	return _DIJoyState.rgbButtons[button] != 0;
}

bool DI8InputGamepad::IsButtonUp(int32 button) const
{
	return !IsButtonDown(button);
}

bool DI8InputGamepad::IsButtonPushed(int32 button) const
{
	if (button < 0 || button > GetButtonCount()-1)
	{
		return false;
	}

	return (_DIJoyState.rgbButtons[button] != 0) && (_DILastJoyState.rgbButtons[button] == 0);
}

bool DI8InputGamepad::IsButtonToggled(int32 button) const
{
	if (button < 0 || button > GetButtonCount()-1)
	{
		return false;
	}

	return _DIJoyState.rgbButtons[button] != _DILastJoyState.rgbButtons[button];
}

bool DI8InputGamepad::IsAxisPosition(GamepadAxis axis) const
{
	return _axisPosition[axis];
}

bool DI8InputGamepad::IsAxisRotation(GamepadAxis axis) const
{
	return _axisRotation[axis];
}

int32 DI8InputGamepad::GetAxisPosition(GamepadAxis axis) const
{
	switch (axis)
	{
	case GamepadAxis_XAxis:
		return _DIJoyState.lX;
	case GamepadAxis_YAxis:
		return _DIJoyState.lY;
	case GamepadAxis_ZAxis:
		return _DIJoyState.lZ;
	default:
		return 0;
	}
}

int32 DI8InputGamepad::GetAxisRotation(GamepadAxis axis) const
{
	switch (axis)
	{
	case GamepadAxis_XAxis:
		return _DIJoyState.lRx;
	case GamepadAxis_YAxis:
		return _DIJoyState.lRy;
	case GamepadAxis_ZAxis:
		return _DIJoyState.lRz;
	default:
		return 0;
	}
}

int32 DI8InputGamepad::GetSlider(int32 slider) const
{
	if (slider < 0 || slider > GetAxisCount()-1)
	{
		return 0;
	}

	return _DIJoyState.rglSlider[slider];
}

int32 DI8InputGamepad::GetPointOfView(int32 pov) const
{
	if (pov < 0 || pov > GetPOVCount()-1)
	{
		return 0;
	}

	return _DIJoyState.rgdwPOV[pov];
}

}
