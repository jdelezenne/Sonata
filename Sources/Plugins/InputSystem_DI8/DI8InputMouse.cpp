/*=============================================================================
DI8InputMouse.cpp
Project: Sonata Engine
Copyright (c) 2005
Julien Delezenne
=============================================================================*/

#include "DI8InputMouse.h"
#include "DI8InputSystem.h"

namespace SE_pDI8
{

DI8InputMouse::DI8InputMouse(InputSystem* inputManager, LPCDIDEVICEINSTANCE pDIDI):
	InputMouse(inputManager)
{
	_pDIDI = new DIDEVICEINSTANCE;
	Memory::Copy((void*)_pDIDI, (void*)pDIDI, sizeof(DIDEVICEINSTANCE));
	_pDID = NULL;
	Memory::Zero(&_DIDevCaps, sizeof(DIDEVCAPS));
	Memory::Zero(&_DIMouseState, sizeof(DIMOUSESTATE2));
	Memory::Zero(&_DILastMouseState, sizeof(DIMOUSESTATE2));
	Memory::Zero(&_position, sizeof(PointInt));
	_wheel = 0;
}

DI8InputMouse::~DI8InputMouse()
{
	SE_DELETE(_pDIDI);
}

bool DI8InputMouse::ReadBuffered()
{
	DIDEVICEOBJECTDATA didod[DI8_INPUT_BUFFER_SIZE]; // Receives buffered data 
	DWORD dwElements;
	HRESULT hr;

	if (_pDID == NULL)
	{
		return false;
	}

	// Store the last mouse state.
	Memory::Copy(&_DILastMouseState, &_DIMouseState, sizeof(DIMOUSESTATE2));

	dwElements = DI8_INPUT_BUFFER_SIZE;
	// Retrieve buffered data from the device.
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
		switch (didod[i].dwOfs)
		{
		case DIMOFS_BUTTON0:
			_DIMouseState.rgbButtons[0] = (didod[i].dwData & 0x80) != 0 ? didod[i].dwTimeStamp : 0;
			break;

		case DIMOFS_BUTTON1:
			_DIMouseState.rgbButtons[1] = (didod[i].dwData & 0x80) != 0 ? didod[i].dwTimeStamp : 0;
			break;

		case DIMOFS_BUTTON2:
			_DIMouseState.rgbButtons[2] = (didod[i].dwData & 0x80) != 0 ? didod[i].dwTimeStamp : 0;
			break;

		case DIMOFS_BUTTON3:
			_DIMouseState.rgbButtons[3] = (didod[i].dwData & 0x80) != 0 ? didod[i].dwTimeStamp : 0;
			break;

		case DIMOFS_BUTTON4:
			_DIMouseState.rgbButtons[4] = (didod[i].dwData & 0x80) != 0 ? didod[i].dwTimeStamp : 0;
			break;

		case DIMOFS_BUTTON5:
			_DIMouseState.rgbButtons[5] = (didod[i].dwData & 0x80) != 0 ? didod[i].dwTimeStamp : 0;
			break;

		case DIMOFS_BUTTON6:
			_DIMouseState.rgbButtons[6] = (didod[i].dwData & 0x80) != 0 ? didod[i].dwTimeStamp : 0;
			break;

		case DIMOFS_BUTTON7:
			_DIMouseState.rgbButtons[7] = (didod[i].dwData & 0x80) != 0 ? didod[i].dwTimeStamp : 0;
			break;

		case DIMOFS_X:
			_DIMouseState.lX = didod[i].dwData;
			_position.X += _DIMouseState.lX;
			break;

		case DIMOFS_Y:
			_DIMouseState.lY = didod[i].dwData;
			_position.Y += _DIMouseState.lY;
			break;

		case DIMOFS_Z:
			_DIMouseState.lZ = didod[i].dwData;
			_wheel = _DIMouseState.lZ;
			break;
		}
	}

	return true;
}

bool DI8InputMouse::ReadImmediate()
{
	HRESULT hr;

	if (_pDID == NULL)
	{
		return false;
	}

	// Store the last mouse state.
	Memory::Copy(&_DILastMouseState, &_DIMouseState, sizeof(DIMOUSESTATE2));

	// Retrieve the mouse state.
	Memory::Zero(&_DIMouseState, sizeof(_DIMouseState));
	hr = _pDID->GetDeviceState(sizeof(DIMOUSESTATE2), &_DIMouseState);

	// If failed, acquire the mouse again.
	if (FAILED(hr))
	{ 
		hr = _pDID->Acquire();
		while(hr == DIERR_INPUTLOST) 
			hr = _pDID->Acquire();

		if (hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED)
			return false; // Unacquired
	}

    POINT pt;    
	::GetCursorPos(&pt);

	::ScreenToClient((HWND)_window->GetHandle(), &pt);

	_position.X = pt.x;
	_position.Y = pt.y;
	_wheel = _DIMouseState.lZ / 120; //TODO: use system constant

	return true;
}

String DI8InputMouse::GetProductName() const
{
	if (_pDIDI == NULL)
	{
		return String::Empty;
	}

	return _pDIDI->tszProductName;
}

String DI8InputMouse::GetGUID() const
{
	if (_pDID == NULL)
	{
		return String::Empty;
	}

	return String::Empty;
}

bool DI8InputMouse::Acquire()
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
	if (FAILED(hr = _pDID->SetCooperativeLevel((HWND) _window->GetHandle(), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
	{
		return false;
	}

	if (FAILED(hr = _pDID->Acquire()))
	{
		return false;
	}

	return true;
}

bool DI8InputMouse::Unacquire()
{
	if (_state != InputDeviceState_Ready || _pDID == NULL)
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

bool DI8InputMouse::Read()
{
	return ReadImmediate();
}

bool DI8InputMouse::Create(Window* window)
{
	if (window == NULL)
	{
		return false;
	}

	if (_inputSystem == NULL || _pDID != NULL)
	{
		return false;
	}

	if (_state != InputDeviceState_Unknown)
	{
		return false;
	}

	HRESULT hr;

	if (FAILED(hr = ((DI8InputSystem*)_inputSystem)->GetDIObject()->CreateDevice(_pDIDI->guidInstance, &_pDID, NULL)))
	{
		return false;
	}

	if (FAILED(hr = _pDID->SetDataFormat(&c_dfDIMouse2)))
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

	if (!Acquire())
		return false;

    POINT pt;    
	::GetCursorPos(&pt);
    
	::ScreenToClient((HWND)window->GetHandle(), &pt);
	_position.X = pt.x;
	_position.Y = pt.y;

	return true;
}

bool DI8InputMouse::Destroy()
{
	if (_state != InputDeviceState_Ready)
	{
		return false;
	}

	_pDID->Unacquire();
	SE_RELEASE(_pDID);

	Memory::Zero(&_DIMouseState, sizeof(DIMOUSESTATE2));
	Memory::Zero(&_DILastMouseState, sizeof(DIMOUSESTATE2));
	Memory::Zero(&_position, sizeof(PointInt));

	_state = InputDeviceState_Unknown;

	return true;
}

void DI8InputMouse::SetBuffered(bool value)
{
	if (_pDID != NULL)
	{
		DIPROPDWORD dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = DI8_INPUT_BUFFER_SIZE;
		if (FAILED(_pDID->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))
		{
			return;
		}

		Acquire();
	}
}

bool DI8InputMouse::IsMouseButtonDown(MouseButton button, uint32* buttonTime) const
{
	switch (button)
	{
	case MouseButton_0:
		return _DIMouseState.rgbButtons[0] != 0;
	case MouseButton_1:
		return _DIMouseState.rgbButtons[1] != 0;
	case MouseButton_2:
		return _DIMouseState.rgbButtons[2] != 0;
	case MouseButton_3:
		return _DIMouseState.rgbButtons[3] != 0;
	case MouseButton_4:
		return _DIMouseState.rgbButtons[4] != 0;
	case MouseButton_5:
		return _DIMouseState.rgbButtons[5] != 0;
	case MouseButton_6:
		return _DIMouseState.rgbButtons[6] != 0;
	case MouseButton_7:
		return _DIMouseState.rgbButtons[7] != 0;
	default:
		return 0;
	}
}

bool DI8InputMouse::IsMouseButtonUp(MouseButton button) const
{
	return !IsMouseButtonDown(button);
}


bool DI8InputMouse::IsMouseClicked(MouseButton button) const
{
	switch (button)
	{
	case MouseButton_0:
		return _DIMouseState.rgbButtons[0] != 0 && _DILastMouseState.rgbButtons[0] == 0;
	case MouseButton_1:
		return _DIMouseState.rgbButtons[1] != 0 && _DILastMouseState.rgbButtons[1] == 0;
	case MouseButton_2:
		return _DIMouseState.rgbButtons[2] != 0 && _DILastMouseState.rgbButtons[2] == 0;
	case MouseButton_3:
		return _DIMouseState.rgbButtons[3] != 0 && _DILastMouseState.rgbButtons[3] == 0;
	case MouseButton_4:
		return _DIMouseState.rgbButtons[4] != 0 && _DILastMouseState.rgbButtons[4] == 0;
	case MouseButton_5:
		return _DIMouseState.rgbButtons[5] != 0 && _DILastMouseState.rgbButtons[5] == 0;
	case MouseButton_6:
		return _DIMouseState.rgbButtons[6] != 0 && _DILastMouseState.rgbButtons[6] == 0;
	case MouseButton_7:
		return _DIMouseState.rgbButtons[7] != 0 && _DILastMouseState.rgbButtons[7] == 0;
	default:
		return 0;
	}
}

bool DI8InputMouse::IsMouseToggled(MouseButton button) const
{
	switch (button)
	{
	case MouseButton_0:
		return _DIMouseState.rgbButtons[0] != _DILastMouseState.rgbButtons[0];
	case MouseButton_1:
		return _DIMouseState.rgbButtons[1] != _DILastMouseState.rgbButtons[1];
	case MouseButton_2:
		return _DIMouseState.rgbButtons[2] != _DILastMouseState.rgbButtons[2];
	case MouseButton_3:
		return _DIMouseState.rgbButtons[3] != _DILastMouseState.rgbButtons[3];
	case MouseButton_4:
		return _DIMouseState.rgbButtons[4] != _DILastMouseState.rgbButtons[4];
	case MouseButton_5:
		return _DIMouseState.rgbButtons[5] != _DILastMouseState.rgbButtons[5];
	case MouseButton_6:
		return _DIMouseState.rgbButtons[6] != _DILastMouseState.rgbButtons[6];
	case MouseButton_7:
		return _DIMouseState.rgbButtons[7] != _DILastMouseState.rgbButtons[7];
	default:
		return 0;
	}
}

void DI8InputMouse::GetMousePosition(int32& x, int32& y) const
{
	x = _position.X;
	y = _position.Y;
}

void DI8InputMouse::GetMouseRelativePosition(int32& x, int32& y) const
{
	x = _DIMouseState.lX;
	y = _DIMouseState.lY;
}

int32 DI8InputMouse::GetMouseWheel() const
{
	return _DIMouseState.lZ;
}

}
