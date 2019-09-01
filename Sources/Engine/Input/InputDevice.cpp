/*=============================================================================
InputDevice.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "InputDevice.h"
#include "Input/InputSystem.h"

namespace SonataEngine
{
namespace Input
{

InputDevice::InputDevice(InputSystem* inputSystem) :
	RefObject(),
	_inputSystem(inputSystem),
	_window(NULL),
	_state(InputDeviceState_Unknown),
	_port(0),
	_slot(0)
{
}

InputDevice::~InputDevice()
{
}

}
}
