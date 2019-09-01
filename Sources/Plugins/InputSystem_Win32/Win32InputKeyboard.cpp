/*=============================================================================
Win32InputKeyboard.cpp
Project: Sonata Engine
Copyright (c) 2005
Julien Delezenne
=============================================================================*/

#include "Win32InputKeyboard.h"
#include "Win32InputSystem.h"

namespace SE_Win32
{

Win32InputKeyboard::Win32InputKeyboard(InputSystem* inputManager) :
	InputKeyboard(inputManager)
{
	Init();
}

Win32InputKeyboard::~Win32InputKeyboard()
{
	Destroy();
}

void Win32InputKeyboard::Init()
{
	::ZeroMemory(_KeyState, sizeof(ulong) * 256);
	::ZeroMemory(_PrevKeyState, sizeof(ulong) * 256);
	_Repetition = false;
}

KeyCode Win32InputKeyboard::VKToKey(uint8 keyCode)
{
	switch (keyCode)
	{
	case VK_ESCAPE: return Key_Escape;
	case VK_OEM_MINUS: return Key_Minus;
	//case VK_EQUALS: return Key_Equals;
	case VK_BACK: return Key_BackSpace;
	case VK_TAB: return Key_Tab;
	case VK_OEM_4: return Key_LeftBracket;
	case VK_OEM_6: return Key_RightBracket;
	case VK_RETURN: return Key_Return;
	case VK_LCONTROL: return Key_LeftControl;
	case VK_OEM_1: return Key_SemiColon;
	case VK_OEM_7: return Key_Apostrophe;
	//case VK_GRAVE: return Key_Grave;
	case VK_LSHIFT: return Key_LeftShift;
	case VK_OEM_5: return Key_BackSlash;
	//case VK_COMMA: return Key_Comma;
	//case VK_PERIOD: return Key_Period;
	case VK_OEM_2: return Key_Slash;
	case VK_RSHIFT: return Key_RightShift;
	case VK_MULTIPLY: return Key_Multiply;
	case VK_LMENU: return Key_LeftAlt;
	case VK_SPACE: return Key_Space;
	case VK_CAPITAL: return Key_CapsLock;
	case VK_NUMLOCK: return Key_Numlock;
	case VK_SCROLL: return Key_Scroll;
	case VK_SUBTRACT: return Key_Subtract;
	case VK_ADD: return Key_Add;
	case VK_DECIMAL: return Key_Decimal;
	case '1': return Key_D1;
	case '2': return Key_D2;
	case '3': return Key_D3;
	case '4': return Key_D4;
	case '5': return Key_D5;
	case '6': return Key_D6;
	case '7': return Key_D7;
	case '8': return Key_D8;
	case '9': return Key_D9;
	case '0': return Key_D0;
	case 'Q': return Key_Q;
	case 'W': return Key_W;
	case 'E': return Key_E;
	case 'R': return Key_R;
	case 'T': return Key_T;
	case 'Y': return Key_Y;
	case 'U': return Key_U;
	case 'I': return Key_I;
	case 'O': return Key_O;
	case 'P': return Key_P;
	case 'A': return Key_A;
	case 'S': return Key_S;
	case 'D': return Key_D;
	case 'F': return Key_F;
	case 'G': return Key_G;
	case 'H': return Key_H;
	case 'J': return Key_J;
	case 'K': return Key_K;
	case 'L': return Key_L;
	case 'Z': return Key_Z;
	case 'X': return Key_X;
	case 'C': return Key_C;
	case 'V': return Key_V;
	case 'B': return Key_B;
	case 'N': return Key_N;
	case 'M': return Key_M;
	case VK_F1: return Key_F1;
	case VK_F2: return Key_F2;
	case VK_F3: return Key_F3;
	case VK_F4: return Key_F4;
	case VK_F5: return Key_F5;
	case VK_F6: return Key_F6;
	case VK_F7: return Key_F7;
	case VK_F8: return Key_F8;
	case VK_F9: return Key_F9;
	case VK_F10: return Key_F10;
	case VK_F11: return Key_F11;
	case VK_F12: return Key_F12;
	case VK_F13: return Key_F13;
	case VK_F14: return Key_F14;
	case VK_F15: return Key_F15;
	case VK_NUMPAD0: return Key_NumPad0;
	case VK_NUMPAD1: return Key_NumPad1;
	case VK_NUMPAD2: return Key_NumPad2;
	case VK_NUMPAD3: return Key_NumPad3;
	case VK_NUMPAD4: return Key_NumPad4;
	case VK_NUMPAD5: return Key_NumPad5;
	case VK_NUMPAD6: return Key_NumPad6;
	case VK_NUMPAD7: return Key_NumPad7;
	case VK_NUMPAD8: return Key_NumPad8;
	case VK_NUMPAD9: return Key_NumPad9;
	case VK_OEM_102: return Key_OEM102;
	case VK_KANA: return Key_Kana;
	//case VK_ABNT_C1: return Key_AbntC1;
	case VK_CONVERT: return Key_Convert;
	//case VK_NOCONVERT: return Key_NoConvert;
	//case VK_YEN: return Key_Yen;
	//case VK_ABNT_C2: return Key_AbntC2;
	case VK_OEM_NEC_EQUAL: return Key_NumPadEquals;
#if(_WIN32_WINNT >= 0x0500)
	case VK_MEDIA_PREV_TRACK: return Key_PrevTrack;
#endif
	//case VK_AT: return Key_At;
	//case VK_COLON: return Key_Colon;
	//case VK_UNDERLINE: return Key_Underline;
	case VK_KANJI: return Key_Kanji;
#if(_WIN32_WINNT >= 0x0500)
	case VK_MEDIA_STOP: return Key_Stop;
#endif
	//case VK_AX: return Key_AX;
	//case VK_UNLABELED: return Key_Unlabeled;
#if(_WIN32_WINNT >= 0x0500)
	case VK_MEDIA_NEXT_TRACK: return Key_NextTrack;
#endif
	//case VK_NUMPADENTER: return Key_NumPadEnter;
	case VK_RCONTROL: return Key_RightControl;
#if(_WIN32_WINNT >= 0x0500)
	case VK_VOLUME_MUTE: return Key_Mute;
#endif
	//case VK_CALCULATOR: return Key_Calculator;
#if(_WIN32_WINNT >= 0x0500)
	case VK_MEDIA_PLAY_PAUSE: return Key_PlayPause;
	//case VK_MEDIA_STOP: return Key_MediaStop;
	case VK_VOLUME_DOWN: return Key_VolumeDown;
	case VK_VOLUME_UP: return Key_VolumeUp;
#endif
	//case VK_WEBHOME: return Key_WebHome;
	//case VK_NUMPADCOMMA: return Key_NumPadComma;
	case VK_DIVIDE: return Key_Divide;
	//case VK_SYSRQ: return Key_SysRq;
	case VK_RMENU: return Key_RightAlt;
	case VK_PAUSE: return Key_Pause;
	case VK_HOME: return Key_Home;
	case VK_UP: return Key_Up;
	case VK_PRIOR: return Key_PageUp;
	case VK_LEFT: return Key_Left;
	case VK_RIGHT: return Key_Right;
	case VK_END: return Key_End;
	case VK_DOWN: return Key_Down;
	case VK_NEXT: return Key_PageDown;
	case VK_INSERT: return Key_Insert;
	case VK_DELETE: return Key_Delete;
	case VK_LWIN: return Key_LeftWindows;
	case VK_RWIN: return Key_RightWindows;
	case VK_APPS: return Key_Apps;
	//case VK_POWER: return Key_Power;
	case VK_SLEEP: return Key_Sleep;
	//case VK_WAKE: return Key_Wake;
#if(_WIN32_WINNT >= 0x0500)
	case VK_BROWSER_SEARCH: return Key_WebSearch;
	case VK_BROWSER_FAVORITES: return Key_WebFavorites;
	case VK_BROWSER_REFRESH: return Key_WebRefresh;
	case VK_BROWSER_STOP: return Key_WebStop;
	case VK_BROWSER_FORWARD: return Key_WebForward;
	case VK_BROWSER_BACK: return Key_WebBack;
	case VK_BROWSER_HOME: return Key_MyComputer;
	case VK_LAUNCH_MAIL: return Key_Mail;
	case VK_LAUNCH_MEDIA_SELECT: return Key_MediaSelect;
#endif
	default: return (KeyCode)0;
	}
}

uint8 Win32InputKeyboard::KeyToVK(KeyCode keyCode)
{
	for (uint i = 0; i < 256; i++)
	{
		if (keyCode == Win32InputKeyboard::VKToKey(i))
			return i;
	}

	return 0;
}

bool Win32InputKeyboard::ReadBuffered()
{
	return false;
}

bool Win32InputKeyboard::ReadImmediate()
{
	BYTE keyState[256];

	// Store the last keyboard state.
	::CopyMemory(&_PrevKeyState, &_KeyState, 256 * sizeof(ulong));

	// Retrieve the keyboard state.
	::ZeroMemory(keyState, sizeof(keyState));
	if (!::GetKeyboardState((PBYTE)&keyState))
		return false;

	for (int i = 0; i < 256; i++)
	{
		if ((keyState[i] & 0x80) != 0)
		{ 
			if (_KeyState[i] == 0)
				_KeyState[i] = ((real64)TimeValue::GetTime() * 1000.0);
		}
		else
			_KeyState[i] = 0;
	}

	return true;
}

String Win32InputKeyboard::GetProductName() const
{
	return String::Empty;
}

String Win32InputKeyboard::GetGUID() const
{
	return String::Empty;
}

bool Win32InputKeyboard::Acquire()
{
	if (!_Created || !_Window)
		return false;

	return true;
}

bool Win32InputKeyboard::Unacquire()
{
	if (!_Created)
		return false;

	return true;
}

bool Win32InputKeyboard::Read()
{
	if (!_Created)
		return false;

	if (_Buffered)
		return ReadBuffered();
	else
		return ReadImmediate();
}

bool Win32InputKeyboard::Create(Window* window)
{
	if (_Created)
		return false;

	if (window == NULL)
		return false;

	_Window = window;
	_Created = true;

	return true;
}

bool Win32InputKeyboard::Destroy()
{
	if (!_Created)
		return false;

	Init();

	_Created = false;

	return true;
}

bool Win32InputKeyboard::GetKeyName(KeyCode keyCode, String& keyName) const
{
	LONG lParam = Win32InputKeyboard::KeyToVK(keyCode);
	TCHAR str[512];

	if (::GetKeyNameText(lParam, str, sizeof(str)) == 0)
		return false;

	keyName = str;
	return true;
}

KeyCode Win32InputKeyboard::GetKeyFromName(const String& keyName) const
{
	TCHAR str[512];

	for (LONG key = 0; key < 256; key++)
	{
		if (::GetKeyNameText(key, str, sizeof(str)) == 0)
			continue;

		if (String::Compare(keyName, str) == 0)
			return Win32InputKeyboard::VKToKey(key);
	}

	return (KeyCode)0;
}

bool Win32InputKeyboard::GetKeyboardRepetition() const
{
	return _Repetition;
}

void Win32InputKeyboard::SetKeyboardRepetition(bool value)
{
	_Repetition = value;
}

bool Win32InputKeyboard::IsKeyDown(KeyCode keyCode, ulong* keyTime) const
{
	ulong key = Win32InputKeyboard::KeyToVK(keyCode);
	if (keyTime != NULL)
		::CopyMemory(keyTime, &_KeyState[key], sizeof(keyTime));
	return _KeyState[key] != 0;
}

bool Win32InputKeyboard::IsKeyUp(KeyCode keyCode) const
{
	return _KeyState[Win32InputKeyboard::KeyToVK(keyCode)] == 0;
}

bool Win32InputKeyboard::IsKeyToggled(KeyCode keyCode) const
{
	ulong key = Win32InputKeyboard::KeyToVK(keyCode);
	return _KeyState[key] != _PrevKeyState[key];
}

}
