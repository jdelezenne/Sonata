/*=============================================================================
DI8InputKeyboard.cpp
Project: Sonata Engine
Copyright (c) 2005
Julien Delezenne
=============================================================================*/

#include "DI8InputKeyboard.h"
#include "DI8InputSystem.h"

namespace SE_pDI8
{

DI8InputKeyboard::DI8InputKeyboard(InputSystem* inputManager, LPCDIDEVICEINSTANCE pDIDI) :
	InputKeyboard(inputManager)
{
	_pDIDI = new DIDEVICEINSTANCE;
	Memory::Copy((void*)_pDIDI, (void*)pDIDI, sizeof(DIDEVICEINSTANCE));
	_pDID = NULL;
	Memory::Zero(&_DIDevCaps, sizeof(DIDEVCAPS));
	Memory::Zero(_keyState, sizeof(uint32) * 256);
	Memory::Zero(_lastKeyState, sizeof(uint32) * 256);
	_repetition = false;
}

DI8InputKeyboard::~DI8InputKeyboard()
{
	SE_DELETE(_pDIDI);
}

KeyCode DI8InputKeyboard::DIKToKey(uint8 keyCode)
{
	switch (keyCode)
	{
	case DIK_ESCAPE: return Key_Escape;
	case DIK_MINUS: return Key_Minus;
	case DIK_EQUALS: return Key_Equals;
	case DIK_BACK: return Key_BackSpace;
	case DIK_TAB: return Key_Tab;
	case DIK_LBRACKET: return Key_LeftBracket;
	case DIK_RBRACKET: return Key_RightBracket;
	case DIK_RETURN: return Key_Return;
	case DIK_LCONTROL: return Key_LeftControl;
	case DIK_SEMICOLON: return Key_SemiColon;
	case DIK_APOSTROPHE: return Key_Apostrophe;
	case DIK_GRAVE: return Key_Grave;
	case DIK_LSHIFT: return Key_LeftShift;
	case DIK_BACKSLASH: return Key_BackSlash;
	case DIK_COMMA: return Key_Comma;
	case DIK_PERIOD: return Key_Period;
	case DIK_SLASH: return Key_Slash;
	case DIK_RSHIFT: return Key_RightShift;
	case DIK_MULTIPLY: return Key_Multiply;
	case DIK_LMENU: return Key_LeftAlt;
	case DIK_SPACE: return Key_Space;
	case DIK_CAPITAL: return Key_CapsLock;
	case DIK_NUMLOCK: return Key_Numlock;
	case DIK_SCROLL: return Key_Scroll;
	case DIK_SUBTRACT: return Key_Subtract;
	case DIK_ADD: return Key_Add;
	case DIK_DECIMAL: return Key_Decimal;
	case DIK_1: return Key_D1;
	case DIK_2: return Key_D2;
	case DIK_3: return Key_D3;
	case DIK_4: return Key_D4;
	case DIK_5: return Key_D5;
	case DIK_6: return Key_D6;
	case DIK_7: return Key_D7;
	case DIK_8: return Key_D8;
	case DIK_9: return Key_D9;
	case DIK_0: return Key_D0;
	case DIK_Q: return Key_Q;
	case DIK_W: return Key_W;
	case DIK_E: return Key_E;
	case DIK_R: return Key_R;
	case DIK_T: return Key_T;
	case DIK_Y: return Key_Y;
	case DIK_U: return Key_U;
	case DIK_I: return Key_I;
	case DIK_O: return Key_O;
	case DIK_P: return Key_P;
	case DIK_A: return Key_A;
	case DIK_S: return Key_S;
	case DIK_D: return Key_D;
	case DIK_F: return Key_F;
	case DIK_G: return Key_G;
	case DIK_H: return Key_H;
	case DIK_J: return Key_J;
	case DIK_K: return Key_K;
	case DIK_L: return Key_L;
	case DIK_Z: return Key_Z;
	case DIK_X: return Key_X;
	case DIK_C: return Key_C;
	case DIK_V: return Key_V;
	case DIK_B: return Key_B;
	case DIK_N: return Key_N;
	case DIK_M: return Key_M;
	case DIK_F1: return Key_F1;
	case DIK_F2: return Key_F2;
	case DIK_F3: return Key_F3;
	case DIK_F4: return Key_F4;
	case DIK_F5: return Key_F5;
	case DIK_F6: return Key_F6;
	case DIK_F7: return Key_F7;
	case DIK_F8: return Key_F8;
	case DIK_F9: return Key_F9;
	case DIK_F10: return Key_F10;
	case DIK_F11: return Key_F11;
	case DIK_F12: return Key_F12;
	case DIK_F13: return Key_F13;
	case DIK_F14: return Key_F14;
	case DIK_F15: return Key_F15;
	case DIK_NUMPAD0: return Key_NumPad0;
	case DIK_NUMPAD1: return Key_NumPad1;
	case DIK_NUMPAD2: return Key_NumPad2;
	case DIK_NUMPAD3: return Key_NumPad3;
	case DIK_NUMPAD4: return Key_NumPad4;
	case DIK_NUMPAD5: return Key_NumPad5;
	case DIK_NUMPAD6: return Key_NumPad6;
	case DIK_NUMPAD7: return Key_NumPad7;
	case DIK_NUMPAD8: return Key_NumPad8;
	case DIK_NUMPAD9: return Key_NumPad9;
	case DIK_OEM_102: return Key_OEM102;
	case DIK_KANA: return Key_Kana;
	case DIK_ABNT_C1: return Key_AbntC1;
	case DIK_CONVERT: return Key_Convert;
	case DIK_NOCONVERT: return Key_NoConvert;
	case DIK_YEN: return Key_Yen;
	case DIK_ABNT_C2: return Key_AbntC2;
	case DIK_NUMPADEQUALS: return Key_NumPadEquals;
	case DIK_PREVTRACK: return Key_PrevTrack;
	case DIK_AT: return Key_At;
	case DIK_COLON: return Key_Colon;
	case DIK_UNDERLINE: return Key_Underline;
	case DIK_KANJI: return Key_Kanji;
	case DIK_STOP: return Key_Stop;
	case DIK_AX: return Key_AX;
	case DIK_UNLABELED: return Key_Unlabeled;
	case DIK_NEXTTRACK: return Key_NextTrack;
	case DIK_NUMPADENTER: return Key_NumPadEnter;
	case DIK_RCONTROL: return Key_RightControl;
	case DIK_MUTE: return Key_Mute;
	case DIK_CALCULATOR: return Key_Calculator;
	case DIK_PLAYPAUSE: return Key_PlayPause;
	case DIK_MEDIASTOP: return Key_MediaStop;
	case DIK_VOLUMEDOWN: return Key_VolumeDown;
	case DIK_VOLUMEUP: return Key_VolumeUp;
	case DIK_WEBHOME: return Key_WebHome;
	case DIK_NUMPADCOMMA: return Key_NumPadComma;
	case DIK_DIVIDE: return Key_Divide;
	case DIK_SYSRQ: return Key_SysRq;
	case DIK_RMENU: return Key_RightAlt;
	case DIK_PAUSE: return Key_Pause;
	case DIK_HOME: return Key_Home;
	case DIK_UP: return Key_Up;
	case DIK_PGUP: return Key_PageUp;
	case DIK_LEFT: return Key_Left;
	case DIK_RIGHT: return Key_Right;
	case DIK_END: return Key_End;
	case DIK_DOWN: return Key_Down;
	case DIK_NEXT: return Key_PageDown;
	case DIK_INSERT: return Key_Insert;
	case DIK_DELETE: return Key_Delete;
	case DIK_LWIN: return Key_LeftWindows;
	case DIK_RWIN: return Key_RightWindows;
	case DIK_APPS: return Key_Apps;
	case DIK_POWER: return Key_Power;
	case DIK_SLEEP: return Key_Sleep;
	case DIK_WAKE: return Key_Wake;
	case DIK_WEBSEARCH: return Key_WebSearch;
	case DIK_WEBFAVORITES: return Key_WebFavorites;
	case DIK_WEBREFRESH: return Key_WebRefresh;
	case DIK_WEBSTOP: return Key_WebStop;
	case DIK_WEBFORWARD: return Key_WebForward;
	case DIK_WEBBACK: return Key_WebBack;
	case DIK_MYCOMPUTER: return Key_MyComputer;
	case DIK_MAIL: return Key_Mail;
	case DIK_MEDIASELECT: return Key_MediaSelect;
	default: return (KeyCode) 0;
	}
}

uint8 DI8InputKeyboard::KeyToDIK(KeyCode keyCode)
{
	for (uint i = 0; i < 256; i++)
	{
		if (keyCode == DI8InputKeyboard::DIKToKey(i))
			return i;
	}

	return 0;
}

bool DI8InputKeyboard::ReadBuffered()
{
	DIDEVICEOBJECTDATA didod[DI8_INPUT_BUFFER_SIZE]; // Receives buffered data 
	DWORD dwElements;
	HRESULT hr;

	assert(_pDID);
	if (_pDID == NULL)
		return false;
	
	dwElements = DI8_INPUT_BUFFER_SIZE;
	// Retrieves buffered data from the device.
	hr = _pDID->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), didod, &dwElements, 0);
	if (hr != DI_OK) 
	{
		hr = _pDID->Acquire();
		while (hr == DIERR_INPUTLOST) 
			hr = _pDID->Acquire();

		if (hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED) 
			return false; // Unacquired

		return true; 
	}

	for (DWORD i = 0; i < dwElements; i++)
	{
		if ((didod[i].dwData & 0x80) != 0)
			_keyState[i] = didod[i].dwTimeStamp;
		else
			_keyState[i] = 0;
	}

	return true;
}

bool DI8InputKeyboard::ReadImmediate()
{
	HRESULT hr;
	BYTE diks[256]; // DirectInput keyboard state buffer 

	assert(_pDID);
	if (_pDID == NULL)
		return false;

	// Retrieve the keyboard state.
	Memory::Zero(diks, sizeof(diks));
	hr = _pDID->GetDeviceState(sizeof(diks), (LPVOID)&diks);

	// Store the last keyboard state.
	Memory::Copy(&_lastKeyState, &_keyState, sizeof(uint32) * 256);

	for (int i = 0; i < 256; i++)
	{
		if ((diks[i] & 0x80) != 0)
		{ 
			if (_keyState[i] == 0)
				_keyState[i] = ((real64)TimeValue::GetTime() * 1000.0);
		}
		else
			_keyState[i] = 0;
	}

	// If failed, acquire the keyboard again.
	if (FAILED(hr))
	{ 
		hr = _pDID->Acquire();
		while (hr == DIERR_INPUTLOST) 
			hr = _pDID->Acquire();

		if (hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED)
			return false; // Unacquired
	}

	return true;
}

String DI8InputKeyboard::GetProductName() const
{
	if (_pDIDI == NULL)
		return String::Empty;

	return _pDIDI->tszProductName;
}

String DI8InputKeyboard::GetGUID() const
{
	if (_pDID == NULL)
		return String::Empty;

	return String::Empty;
}

bool DI8InputKeyboard::Acquire()
{
	if (_state != InputDeviceState_Ready || _pDID == NULL || _window == NULL)
		return false;

	HRESULT hr;
	if (FAILED(hr = _pDID->SetCooperativeLevel((HWND) _window->GetHandle(), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return false;

	if (FAILED(hr = _pDID->Acquire()))
		return false;

	return true;
}

bool DI8InputKeyboard::Unacquire()
{
	if (_state != InputDeviceState_Ready || _pDID == NULL)
		return false;

	HRESULT hr;
	if (FAILED(hr = _pDID->Unacquire()))
		return false;

	return true;
}

bool DI8InputKeyboard::Read()
{
	return ReadImmediate();
}

bool DI8InputKeyboard::Create(Window* window)
{
	if (window == NULL)
	{
		return false;
	}

	if (_inputSystem == NULL || _pDIDI == NULL)
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

	if (FAILED(hr = _pDID->SetDataFormat(&c_dfDIKeyboard)))
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
	{
		return false;
	}

	return true;
}

bool DI8InputKeyboard::Destroy()
{
	if (_state != InputDeviceState_Ready)
	{
		return false;
	}

	_pDID->Unacquire();
	SE_RELEASE(_pDID);

	Memory::Zero(_keyState, sizeof(uint32) * 256);
	Memory::Zero(_lastKeyState, sizeof(uint32) * 256);

	_state = InputDeviceState_Unknown;

	return true;
}

bool DI8InputKeyboard::GetKeyName(KeyCode keyCode, String& keyName) const
{
	if (_pDID == NULL)
	{
		return false;
	}

	DIPROPSTRING dipsz;
	dipsz.diph.dwSize = sizeof(DIPROPSTRING); 
	dipsz.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
	dipsz.diph.dwObj = DI8InputKeyboard::KeyToDIK(keyCode);
	dipsz.diph.dwHow = DIPH_BYOFFSET; 

	if (SUCCEEDED(_pDID->GetProperty(DIPROP_KEYNAME, &dipsz.diph)))
	{
		size_t str_size = wcslen(dipsz.wsz);
		char* szKeyName = new char[str_size+1];
		WideCharToMultiByte(CP_ACP, 0, dipsz.wsz, -1, szKeyName, (int) str_size, NULL, NULL);
		keyName = szKeyName;
		delete[] szKeyName;
	}
	else
	{
		switch (keyCode)
		{
		case Key_F13: keyName = "F13"; break;
		case Key_F14: keyName = "F14"; break;
		case Key_F15: keyName = "F15"; break;
		case Key_OEM102: keyName = "OEM102 (UK/German)"; break;
		case Key_AbntC1: keyName = "? (Brazilian)"; break;
		case Key_AbntC2: keyName = "Numpad . (Brazilian)"; break;
		case Key_NumPadEquals: keyName = "Numpad 0x (NEC PC98)"; break;
		case Key_At: keyName = "At (NEC PC98)"; break;
		case Key_Colon: keyName = "Colon (NEC PC98)"; break;
		case Key_Underline: keyName = "Underline (NEC PC98)"; break;
		case Key_Stop: keyName = "Stop (NEC PC98)"; break;
		case Key_Kana: keyName = "Kana (Japanese)"; break;
		case Key_Convert: keyName = "Convert (Japanese)"; break;
		case Key_NoConvert: keyName = "NoConvert (Japanese)"; break;
		case Key_Yen: keyName = "Yen (Japanese)"; break;
		case Key_Kanji: keyName = "Kanji (Japanese)"; break;
		case Key_AX: keyName = "Ax (Japanese)"; break;
		case Key_Unlabeled: keyName = "Unlabeled (Japanese)"; break;
		case Key_PrevTrack: keyName = "Previous Track"; break;
		case Key_NumPadComma: keyName = ", (NEC PC98)"; break;
		default:
			return false;
		}
	}

	return true;
}

KeyCode DI8InputKeyboard::GetKeyFromName(const String& keyName) const
{
	if (_pDID == NULL)
	{
		return (KeyCode)0;
	}

	DIPROPSTRING dipsz;
	dipsz.diph.dwSize = sizeof(DIPROPSTRING); 
	dipsz.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
	dipsz.diph.dwHow = DIPH_BYOFFSET; 

	for (DWORD key = 0; key < 256; key++)
	{
		dipsz.diph.dwObj = key;
		_pDID->GetProperty(DIPROP_KEYNAME, &dipsz.diph);

		size_t str_size = wcslen(dipsz.wsz);
		char* szKeyName = new char[str_size+1];
		WideCharToMultiByte(CP_ACP, 0, dipsz.wsz, -1, szKeyName, (int) str_size, NULL, NULL);
		delete[] szKeyName;

		if (String::Compare(keyName, szKeyName) == 0)
		{
			return DI8InputKeyboard::DIKToKey(key);
		}
	}

	return (KeyCode) 0;
}

bool DI8InputKeyboard::GetKeyboardRepetition() const
{
	return _repetition;
}

void DI8InputKeyboard::SetKeyboardRepetition(bool value)
{
	_repetition = value;
}

bool DI8InputKeyboard::IsKeyDown(KeyCode keyCode, uint32* keyTime) const
{
	uint32 key = DI8InputKeyboard::KeyToDIK(keyCode);
	if (keyTime != NULL)
	{
		Memory::Copy(keyTime, (void*)&_keyState[key], sizeof(uint32*));
	}
	return _keyState[key] != 0;
}

bool DI8InputKeyboard::IsKeyUp(KeyCode keyCode) const
{
	return _keyState[DI8InputKeyboard::KeyToDIK(keyCode)] == 0;
}

bool DI8InputKeyboard::IsKeyToggled(KeyCode keyCode) const
{
	uint32 key = DI8InputKeyboard::KeyToDIK(keyCode);
	return _keyState[key] != _lastKeyState[key];
}

}
