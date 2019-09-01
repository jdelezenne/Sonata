/*=============================================================================
DI8InputSystem.h
Project: Sonata Engine
Copyright (c) 2005
Julien Delezenne
=============================================================================*/

#ifndef _SE_pDI8INPUTSYSTEM_H_
#define _SE_pDI8INPUTSYSTEM_H_

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <Core/Core.h>
#include <Input/Input.h>

using namespace SonataEngine;
using namespace SonataEngine::Input;

#define DI8_INPUT_BUFFER_SIZE 128

namespace SE_pDI8
{

/** DirectInput8 Input System. */
class DI8InputSystem : public InputSystem
{
protected:
	IDirectInput8* _pDI;
	bool _cleanupCOM;
	bool _isCursorLocked;

public:
	DI8InputSystem();
	virtual ~DI8InputSystem();

	virtual bool Create();
	virtual void Destroy();

	virtual bool EnumDevices();
	virtual void SetCursorVisible(bool value);
	virtual void SetCursorLocked(bool value);

	IDirectInput8* GetDIObject() const { return _pDI; }

	bool AddDIDevice(LPCDIDEVICEINSTANCE pDIDI);

private:
	virtual void _DestroyInputDevice(InputDevice* inputDevice);

	void Initialize();

	// Callback function that receives DirectInput enumerated devices.
	static BOOL CALLBACK DIEnumDevicesCallback(LPCDIDEVICEINSTANCE pDIDI, LPVOID pvRef);
};

}

#endif 
