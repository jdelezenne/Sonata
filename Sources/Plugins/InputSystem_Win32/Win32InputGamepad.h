/*=============================================================================
Win32InputGamepad.h
Project: Sonata Engine
Copyright (c) 2005
Julien Delezenne
=============================================================================*/

#ifndef _SE_WIN32INPUTGAMEPAD_H_
#define _SE_WIN32INPUTGAMEPAD_H_

#include "Win32InputSystem.h"

namespace SE_Win32
{

/** Win32 gamepad device. */
class Win32InputGamepad : public InputGamepad
{
public:
	Win32InputGamepad(InputSystem* inputManager);
	virtual ~Win32InputGamepad();

	virtual bool Create(Window* window);
	virtual bool Destroy();
	virtual void SetBuffered(bool value);
	virtual String GetProductName() const;
	virtual String GetGUID() const;
	virtual bool Acquire();
	virtual bool Unacquire();
	virtual bool Read();

	virtual void GetGamepadObjectList(GamepadObjectList& objectList) const;
	virtual void GetGamepadObjectList(GamepadObjectType type, GamepadObjectList& objectList) const;
	virtual int32 GetButtonCount() const;
	virtual int32 GetAxisCount() const;
	virtual int32 GetPOVCount() const;
	virtual bool IsButtonDown(int32 button, ulong* buttonTime = NULL) const;
	virtual bool IsButtonUp(int32 button) const;
	virtual bool IsButtonPushed(int32 button) const;
	virtual bool IsButtonToggled(int32 button) const;
	virtual bool IsAxisPosition(GamepadAxis axis) const;
	virtual bool IsAxisRotation(GamepadAxis axis) const;
	virtual int32 GetAxisPosition(GamepadAxis axis) const;
	virtual int32 GetAxisRotation(GamepadAxis axis) const;
	virtual int32 GetSlider(int32 slider) const;
	virtual int32 GetPointOfView(int32 pov) const;

protected:
	bool ReadBuffered();
	bool ReadImmediate();
};

}

#endif 
