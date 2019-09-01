/*=============================================================================
InputSystem.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "InputSystem.h"
#include "Core/Exception/ArgumentNullException.h"
#include "Input/InputDevice.h"
#include "Input/InputMouse.h"
#include "Input/InputKeyboard.h"
#include "Input/InputGamepad.h"

namespace SonataEngine
{
namespace Input
{

InputSystem::InputSystem()
{
}

InputSystem::InputSystem(const String& name) :
	Manager(name)
{
}

InputSystem::~InputSystem()
{
}

void InputSystem::Update(real64 elapsed)
{
	ReadAllDevices();
}

int InputSystem::GetInputDeviceCount() const
{
	return _inputDevices.Count();
}

InputSystem::InputDeviceList::Iterator InputSystem::GetInputDeviceIterator() const
{
	return _inputDevices.GetIterator();
}

bool InputSystem::RemoveInputDevice(InputDevice* inputDevice)
{
	if (inputDevice == NULL)
	{
		SEthrow(ArgumentNullException());
		return false;
	}

	_DestroyInputDevice(inputDevice);
	_inputDevices.Remove(inputDevice);
	return true;
}

void InputSystem::RemoveInputDevices(InputDeviceType type)
{
	InputDevice* inputDevice;

	InputDeviceList::Iterator it = _inputDevices.GetIterator();
	while (it.Next())
	{
		inputDevice = it.Current();
		if (inputDevice->GetDeviceType() == type)
		{
			inputDevice = it.Current();
			_DestroyInputDevice(inputDevice);
			_inputDevices.Remove(inputDevice);
		}
	}
}

void InputSystem::RemoveAllInputDevices()
{
	InputDeviceList::Iterator it = _inputDevices.GetIterator();
	while (it.Next())
	{
		_DestroyInputDevice(it.Current());
	}

	_inputDevices.Clear();
}

InputDevice* InputSystem::GetInputDevice(int index) const
{
	return _inputDevices[index];
}

InputDevice* InputSystem::GetInputDeviceByType(InputDeviceType type, int index) const
{
	InputDevice* inputDevice;
	int count = 0;

	InputDeviceList::Iterator it = _inputDevices.GetIterator();
	while (it.Next())
	{
		inputDevice = it.Current();
		if (inputDevice->GetDeviceType() == type)
		{
			if (count == index)
				return inputDevice;
			count++;
		}
	}

	return NULL;
}

void InputSystem::ReadDevices(InputDeviceType type)
{
	InputDevice* inputDevice;

	InputDeviceList::Iterator it = _inputDevices.GetIterator();
	while (it.Next())
	{
		inputDevice = it.Current();
		if (inputDevice->GetDeviceType() == type)
		{
			inputDevice->Read();
		}
	}
}

void InputSystem::ReadAllDevices()
{
	InputDeviceList::Iterator it = _inputDevices.GetIterator();
	while (it.Next())
	{
		it.Current()->Read();
	}
}

bool InputSystem::_AddInputDevice(InputDevice* inputDevice)
{
	if (inputDevice == NULL)
	{
		SEthrow(ArgumentNullException());
		return false;
	}

	_inputDevices.Add(inputDevice);
	return true;
}

}
}
