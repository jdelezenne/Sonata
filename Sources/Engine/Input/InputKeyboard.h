/*=============================================================================
InputKeyboard.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_INPUTKEYBOARD_H_
#define _SE_INPUTKEYBOARD_H_

#include "Core/Core.h"
#include "Input/InputDevice.h"

namespace SonataEngine
{
namespace Input
{

/** Key codes (from DirectInput). */
enum KeyCode
{
	Key_Escape = 1,
	Key_D1 = 2,
	Key_D2 = 3,
	Key_D3 = 4,
	Key_D4 = 5,
	Key_D5 = 6,
	Key_D6 = 7,
	Key_D7 = 8,
	Key_D8 = 9,
	Key_D9 = 10,
	Key_D0 = 11,
	Key_Minus = 12,
	Key_Equals = 13,
	Key_BackSpace = 14,
	Key_Tab = 15,
	Key_Q = 16,
	Key_W = 17,
	Key_E = 18,
	Key_R = 19,
	Key_T = 20,
	Key_Y = 21,
	Key_U = 22,
	Key_I = 23,
	Key_O = 24,
	Key_P = 25,
	Key_LeftBracket = 26,
	Key_RightBracket = 27,
	Key_Return = 28,
	Key_LeftControl = 29,
	Key_A = 30,
	Key_S = 31,
	Key_D = 32,
	Key_F = 33,
	Key_G = 34,
	Key_H = 35,
	Key_J = 36,
	Key_K = 37,
	Key_L = 38,
	Key_SemiColon = 39,
	Key_Apostrophe = 40,
	Key_Grave = 41,
	Key_LeftShift = 42,
	Key_BackSlash = 43,
	Key_Z = 44,
	Key_X = 45,
	Key_C = 46,
	Key_V = 47,
	Key_B = 48,
	Key_N = 49,
	Key_M = 50,
	Key_Comma = 51,
	Key_Period = 52,
	Key_Slash = 53,
	Key_RightShift = 54,
	Key_Multiply = 55,
	Key_LeftAlt = 56,
	Key_LeftMenu = 56,
	Key_Space = 57,
	Key_CapsLock = 58,
	Key_F1 = 59,
	Key_F2 = 60,
	Key_F3 = 61,
	Key_F4 = 62,
	Key_F5 = 63,
	Key_F6 = 64,
	Key_F7 = 65,
	Key_F8 = 66,
	Key_F9 = 67,
	Key_F10 = 68,
	Key_Numlock = 69,
	Key_NumPad7 = 71,
	Key_NumPad8 = 72,
	Key_NumPad9 = 73,
	Key_Subtract = 74,
	Key_NumPad4 = 75,
	Key_NumPad5 = 76,
	Key_NumPad6 = 77,
	Key_Add = 78,
	Key_NumPad1 = 79,
	Key_NumPad2 = 80,
	Key_NumPad3 = 81,
	Key_NumPad0 = 82,
	Key_Decimal = 83,
	Key_OEM102 = 86,
	Key_F11 = 87,
	Key_F12 = 88,
	Key_Scroll = 70,
	Key_F13 = 100,
	Key_F14 = 101,
	Key_F15 = 102,
	Key_Kana = 112,
	Key_AbntC1 = 115,
	Key_Convert = 121,
	Key_NoConvert = 123,
	Key_Yen = 125,
	Key_AbntC2 = 126,
	Key_NumPadEquals = 141,
	Key_PrevTrack = 144,
	Key_At = 145,
	Key_Colon = 146,
	Key_Underline = 147,
	Key_Kanji = 148,
	Key_Stop = 149,
	Key_AX = 150,
	Key_Unlabeled = 151,
	Key_NextTrack = 153,
	Key_NumPadEnter = 156,
	Key_RightControl = 157,
	Key_Mute = 160,
	Key_Calculator = 161,
	Key_PlayPause = 162,
	Key_MediaStop = 164,
	Key_VolumeDown = 174,
	Key_VolumeUp = 176,
	Key_WebHome = 178,
	Key_NumPadComma = 179,
	Key_Divide = 181,
	Key_SysRq = 183,
	Key_RightAlt = 184,
	Key_Pause = 197,
	Key_Home = 199,
	Key_Up = 200,
	Key_PageUp = 201,
	Key_Left = 203,
	Key_Right = 205,
	Key_End = 207,
	Key_Down = 208,
	Key_PageDown = 209,
	Key_Insert = 210,
	Key_Delete = 211,
	Key_LeftWindows = 219,
	Key_RightWindows = 220,
	Key_Apps = 221,
	Key_Power = 222,
	Key_Sleep = 223,
	Key_Wake = 227,
	Key_WebSearch = 229,
	Key_WebFavorites = 230,
	Key_WebRefresh = 231,
	Key_WebStop = 232,
	Key_WebForward = 233,
	Key_WebBack = 234,
	Key_MyComputer = 235,
	Key_Mail = 236,
	Key_MediaSelect = 237
};

const int32 KeyCount = 256;

/**
	@brief Keyboard device.
	@ingroup InputDevices
*/
class SE_INPUT_EXPORT InputKeyboard : public InputDevice
{
public:
	/** @name Constructor / Destructor. */
	//@{
	/**
		Constructor.
		@param name The name of the input device.
		@param inputSystem Pointer to the owner input context.
		@see InputSystem
	*/
	InputKeyboard(InputSystem* inputSystem);

	/** Destructor. */
	virtual ~InputKeyboard();
	//@}

	virtual InputDeviceType GetDeviceType() const { return InputDeviceType_Keyboard; }

	/** Returns an ASCII character from a key code. */
	char KeyCodeToASCII(KeyCode keyCode, char defaultASCII = '.');

	/** @name Device information. */
	//@{
	/**
		Retrieves the localized name of a given key.
		@param keyCode Key code.
		@param keyName Localized name of the key.
		@return true if successful; otherwise, false.
	*/
	virtual bool GetKeyName(KeyCode keyCode, String& keyName) const = 0;

	/**
		Retrieves the code of a key given its localized name.
		@param keyName Localized key name.
		@return Key code corresponding to the localized key name.
	*/
	virtual KeyCode GetKeyFromName(const String& keyName) const = 0;
	//@}

	/** @name Keyboard */
	//@{
	/**
		Returns whether the keyboard repetition is enabled.
		@return true if the keyboard repetition is enabled; otherwise, false.
	*/
	virtual bool GetKeyboardRepetition() const = 0;

	/**
		Enables or disables the keyboard repetition.
		@param value TRUE to enable the keyboard repetition; otherwise, false.
	*/
	virtual void SetKeyboardRepetition(bool value) = 0;

	/**
		Returns whether a key is down (and from how long).
		@param keyCode Key code.
		@param keyTime Pointer to be filled with the time the key has been pressed.
		@return true if the key is down; otherwise, false.
	*/
	virtual bool IsKeyDown(KeyCode keyCode, uint32* keyTime = NULL) const = 0;

	/**
		Returns whether a key is up.
		@param keyCode Key code.
		@return true if the key is up; otherwise, false.
	*/
	virtual bool IsKeyUp(KeyCode keyCode) const = 0;

	/**
		Returns whether a key is toggled.
		@param keyCode Key code.
		@return true if the key is toggled; otherwise, false.
	*/
	virtual bool IsKeyToggled(KeyCode keyCode) const = 0;
	//@}
};

}
}

#endif 
