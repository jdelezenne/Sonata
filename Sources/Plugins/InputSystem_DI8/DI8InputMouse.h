/*=============================================================================
DI8InputMouse.h
Project: Sonata Engine
Copyright (c) 2005
Julien Delezenne
=============================================================================*/

#ifndef _SE_pDI8INPUTMOUSE_H_
#define _SE_pDI8INPUTMOUSE_H_

#include "DI8InputSystem.h"

namespace SE_pDI8
{

/** DirectInput8 mouse device. */
class DI8InputMouse : public InputMouse
{
protected:
	LPCDIDEVICEINSTANCE _pDIDI;
	IDirectInputDevice8* _pDID;
	DIDEVCAPS _DIDevCaps;
	DIMOUSESTATE2 _DIMouseState;
	DIMOUSESTATE2 _DILastMouseState;
	PointInt _position;
	int32 _wheel;

public:
	DI8InputMouse(InputSystem* inputManager, LPCDIDEVICEINSTANCE pDIDI);
	virtual ~DI8InputMouse();

	virtual bool Create(Window* window);
	virtual bool Destroy();
	virtual void SetBuffered(bool value);
	virtual String GetProductName() const;
	virtual String GetGUID() const;
	virtual bool Acquire();
	virtual bool Unacquire();
	virtual bool Read();

	virtual bool IsMouseButtonDown(MouseButton button, uint32* buttonTime = NULL) const;
	virtual bool IsMouseButtonUp(MouseButton button) const;
	virtual bool IsMouseClicked(MouseButton button) const;
	virtual bool IsMouseToggled(MouseButton button) const;
	virtual void GetMousePosition(int32& x, int32& y) const;
	virtual void GetMouseRelativePosition(int32& x, int32& y) const;
	virtual int32 GetMouseWheel() const;

protected:
	bool ReadBuffered();
	bool ReadImmediate();
};

}

#endif 
