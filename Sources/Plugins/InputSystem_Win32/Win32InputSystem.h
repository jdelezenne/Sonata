/*=============================================================================
Win32InputSystem.h
Project: Sonata Engine
Copyright (c) 2005
Julien Delezenne
=============================================================================*/

#ifndef _SE_WIN32INPUTSYSTEM_H_
#define _SE_WIN32INPUTSYSTEM_H_

#include <windows.h>

#include <Core/Core.h>
#include <Input/Input.h>

using namespace SonataEngine;
using namespace SonataEngine::Input;

namespace SE_Win32
{

/** Win32 Input System. */
class Win32InputSystem : public InputSystem
{
public:
	Win32InputSystem();
	virtual ~Win32InputSystem();

	virtual bool Create();
	virtual void Destroy();

	virtual bool EnumDevices();
	virtual void SetCursorVisible(bool value);
	virtual void SetCursorLocked(bool value);

private:
	void Init();
	virtual void DestroyInputDevice(InputDevice* inputDevice);

protected:
	bool _CursorLocked;
};

}

#endif 
