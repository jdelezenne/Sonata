/*=============================================================================
DI8InputGamepad.h
Project: Sonata Engine
Copyright (c) 2005
Julien Delezenne
=============================================================================*/

#ifndef _SE_pDI8INPUTGAMEPAD_H_
#define _SE_pDI8INPUTGAMEPAD_H_

#include "DI8InputSystem.h"

namespace SE_pDI8
{

/** DirectInput8 gamepad device. */
class DI8InputGamepad : public InputGamepad
{
protected:
	LPCDIDEVICEINSTANCE _pDIDI;
	IDirectInputDevice8* _pDID;
	DIDEVCAPS _DIDevCaps;
	GamepadObjectList _GamepadObjects;
	int32 _sliderCount;
	bool _axisPosition[3];
	bool _axisRotation[3];
	DIJOYSTATE2 _DIJoyState;
	DIJOYSTATE2 _DILastJoyState;

public:
	DI8InputGamepad(InputSystem* inputManager, LPCDIDEVICEINSTANCE pDIDI);
	virtual ~DI8InputGamepad();

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
	virtual bool IsButtonDown(int32 button, uint32* buttonTime = NULL) const;
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
	// Callback function that receives DirectInput enumerated device objects.
	static BOOL CALLBACK DIEnumDeviceObjectsCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);
	bool EnumObjects();
	void AddInputObject(LPCDIDEVICEOBJECTINSTANCE lpddoi);
	bool ReadBuffered();
	bool ReadImmediate();
};

}

#endif 
