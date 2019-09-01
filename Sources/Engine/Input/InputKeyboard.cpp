/*=============================================================================
InputKeyboard.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "InputKeyboard.h"

namespace SonataEngine
{
namespace Input
{

InputKeyboard::InputKeyboard(InputSystem* inputSystem) :
	InputDevice(inputSystem)
{
}

InputKeyboard::~InputKeyboard()
{
}

char InputKeyboard::KeyCodeToASCII(KeyCode keyCode, char defaultASCII)
{
	bool controlDown = IsKeyDown(Key_LeftControl) || IsKeyDown(Key_RightControl);
	bool shiftDown = IsKeyDown(Key_LeftShift) || IsKeyDown(Key_RightShift);
	bool capsMapOn = IsKeyDown(Key_CapsLock);
	bool numMapOn = true;//IsKeyDown(Key_Numlock); //TODO
	bool upperCase = (capsMapOn && !shiftDown) || (!capsMapOn && shiftDown);

	switch (keyCode)
	{
	case Key_D1: return shiftDown ? '!' : '1';
	case Key_D2: return shiftDown ? '@' : '2';
	case Key_D3: return shiftDown ? '#' : '3';
	case Key_D4: return shiftDown ? '$' : '4';
	case Key_D5: return shiftDown ? '%' : '5';
	case Key_D6: return shiftDown ? '^' : '6';
	case Key_D7: return shiftDown ? '&' : '7';
	case Key_D8: return shiftDown ? '*' : '8';
	case Key_D9: return shiftDown ? '(' : '9';
	case Key_D0: return shiftDown ? ')' : '0';
	case Key_Minus: return shiftDown ? '_' : '-';
	case Key_Equals: return shiftDown ? '+' : '=';
	case Key_Q: return upperCase ? 'Q' : 'q';
	case Key_W: return upperCase ? 'W' : 'w';
	case Key_E: return upperCase ? 'E' : 'e';
	case Key_R: return upperCase ? 'R' : 'r';
	case Key_T: return upperCase ? 'T' : 't';
	case Key_Y: return upperCase ? 'Y' : 'y';
	case Key_U: return upperCase ? 'U' : 'u';
	case Key_I: return upperCase ? 'I' : 'i';
	case Key_O: return upperCase ? 'O' : 'o';
	case Key_P: return upperCase ? 'P' : 'p';
	case Key_LeftBracket: return shiftDown ? '{' : '[';
	case Key_RightBracket: return shiftDown ? '}' : ']';
	case Key_A: return controlDown ? 1 : (upperCase ? 'A' : 'a');
	case Key_S: return upperCase ? 'S' : 's';
	case Key_D: return upperCase ? 'D' : 'd';
	case Key_F: return upperCase ? 'F' : 'f';
	case Key_G: return upperCase ? 'G' : 'g';
	case Key_H: return upperCase ? 'H' : 'h';
	case Key_J: return upperCase ? 'J' : 'j';
	case Key_K: return upperCase ? 'K' : 'k';
	case Key_L: return upperCase ? 'L' : 'l';
	case Key_SemiColon: return shiftDown ? ':' : ';';
	case Key_Apostrophe: return shiftDown ? '\"' : '\'';
	case Key_Grave: return shiftDown ? '\\' : '|';
	case Key_BackSlash: return shiftDown ? '\\' : '|';
	case Key_Z: return controlDown ? 26 : (upperCase ? 'Z' : 'z');
	case Key_X: return controlDown ? 24 : (upperCase ? 'X' : 'x');
	case Key_C: return controlDown ? 3 : (upperCase ? 'C' : 'c');
	case Key_V: return controlDown ? 22 : (upperCase ? 'V' : 'v');
	case Key_B: return upperCase ? 'B' : 'b';
	case Key_N: return upperCase ? 'N' : 'n';
	case Key_M: return upperCase ? 'M' : 'm';
	case Key_Comma: return shiftDown ? '<' : ',';
	case Key_Period: return shiftDown ? '>' : '.';
	case Key_Slash: return shiftDown ? '?' : '/';
	case Key_Multiply: return '*';
	case Key_Space: return ' ';
	case Key_Subtract: return '-';
	case Key_Add: return '+';
	case Key_Decimal: return '.';
	case Key_Yen: return '�';
	case Key_Colon: return ':';
	case Key_Underline: return '_';
	case Key_Divide: return '/';

	case Key_BackSpace: return '\b';
	case Key_Tab: return '\t';
	case Key_Return: return '\n';
	}

	if (numMapOn)
	{
		switch (keyCode)
		{
		case Key_NumPad7: return '7';
		case Key_NumPad8: return '8';
		case Key_NumPad9: return '9';
		case Key_NumPad4: return '4';
		case Key_NumPad5: return '5';
		case Key_NumPad6: return '6';
		case Key_NumPad1: return '1';
		case Key_NumPad2: return '2';
		case Key_NumPad3: return '3';
		case Key_NumPad0: return '0';
		case Key_NumPadEquals: return '=';
		case Key_NumPadComma: return ',';
		}
	}

	return defaultASCII;
}

}
}
