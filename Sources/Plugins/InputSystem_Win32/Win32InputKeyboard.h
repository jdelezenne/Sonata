/*=============================================================================
Win32InputKeyboard.h
Project: Sonata Engine
Copyright (c) 2005
Julien Delezenne
=============================================================================*/

#ifndef _SE_WIN32INPUTKEYBOARD_H_
#define _SE_WIN32INPUTKEYBOARD_H_

#include "Win32InputSystem.h"

namespace SE_Win32
{

/** Win32 keyboard device. */
class Win32InputKeyboard : public InputKeyboard
{
public:
	Win32InputKeyboard(InputSystem* inputManager);
	virtual ~Win32InputKeyboard();

	virtual bool Create(Window* window);
	virtual bool Destroy();
	virtual String GetProductName() const;
	virtual String GetGUID() const;
	virtual bool Acquire();
	virtual bool Unacquire();
	virtual bool Read();

	virtual bool GetKeyName(KeyCode keyCode, String& keyName) const;
	virtual KeyCode GetKeyFromName(const String& keyName) const;
	virtual bool GetKeyboardRepetition() const;
	virtual void SetKeyboardRepetition(bool value);
	virtual bool IsKeyDown(KeyCode keyCode, ulong* keyTime = NULL) const;
	virtual bool IsKeyUp(KeyCode keyCode) const;
	virtual bool IsKeyToggled(KeyCode keyCode) const;

protected:
	static KeyCode Win32InputKeyboard::VKToKey(uint8 keyCode);
	static uint8 Win32InputKeyboard::KeyToVK(KeyCode keyCode);
	void Init();
	bool ReadBuffered();
	bool ReadImmediate();

	ulong _KeyState[256];
	ulong _PrevKeyState[256];
	bool _Repetition;
};

}

#endif 
