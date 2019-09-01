/*=============================================================================
Win32InputMouse.h
Project: Sonata Engine
Copyright (c) 2005
Julien Delezenne
=============================================================================*/

#ifndef _SE_WIN32INPUTMOUSE_H_
#define _SE_WIN32INPUTMOUSE_H_

#include "Win32InputSystem.h"

namespace SE_Win32
{

/** Win32 mouse device. */
class Win32InputMouse : public InputMouse
{
public:
	Win32InputMouse(InputSystem* inputManager);
	virtual ~Win32InputMouse();

	virtual bool Create(Window* window);
	virtual bool Destroy();
	virtual void SetBuffered(bool value);
	virtual String GetProductName() const;
	virtual String GetGUID() const;
	virtual bool Acquire();
	virtual bool Unacquire();
	virtual bool Read();

	virtual bool IsMouseButtonDown(MouseButton button, ulong* buttonTime = NULL) const;
	virtual bool IsMouseButtonUp(MouseButton button) const;
	virtual bool IsMouseClicked(MouseButton button) const;
	virtual bool IsMouseToggled(MouseButton button) const;
	virtual void GetMousePosition(int32& x, int32& y) const;
	virtual void GetMouseRelativePosition(int32& x, int32& y) const;
	virtual int32 GetMouseWheel() const;

protected:
	void Init();
	bool ReadBuffered();
	bool ReadImmediate();

	ulong _Buttons[3];
	ulong _PrevButtons[3];
	PointInt _Position;
	PointInt _PrevPosition;
	int32 _Wheel;
};

}

#endif 
