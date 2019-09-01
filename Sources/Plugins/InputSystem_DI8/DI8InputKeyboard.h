/*=============================================================================
DI8InputKeyboard.h
Project: Sonata Engine
Copyright (c) 2005
Julien Delezenne
=============================================================================*/

#ifndef _SE_pDI8INPUTKEYBOARD_H_
#define _SE_pDI8INPUTKEYBOARD_H_

#include "DI8InputSystem.h"

namespace SE_pDI8
{

/** DirectInput8 keyboard device. */
class DI8InputKeyboard : public InputKeyboard
{
protected:
	LPCDIDEVICEINSTANCE _pDIDI;
	IDirectInputDevice8* _pDID;
	DIDEVCAPS _DIDevCaps;
	uint32 _keyState[256];
	uint32 _lastKeyState[256];
	bool _repetition;

public:
	DI8InputKeyboard(InputSystem* inputManager, LPCDIDEVICEINSTANCE pDIDI);
	virtual ~DI8InputKeyboard();

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
	virtual bool IsKeyDown(KeyCode keyCode, uint32* keyTime = NULL) const;
	virtual bool IsKeyUp(KeyCode keyCode) const;
	virtual bool IsKeyToggled(KeyCode keyCode) const;

protected:
	static KeyCode DI8InputKeyboard::DIKToKey(uint8 keyCode);
	static uint8 DI8InputKeyboard::KeyToDIK(KeyCode keyCode);
	bool ReadBuffered();
	bool ReadImmediate();
};

}

#endif 
