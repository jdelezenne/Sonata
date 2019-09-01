/*=============================================================================
InputDevice.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_INPUTDEVICE_H_
#define _SE_INPUTDEVICE_H_

#include "Core/Core.h"
#include "Input/Common.h"
#include "Input/InputSystem.h"

namespace SonataEngine
{
namespace Input
{

/** Input Device States. */
enum InputDeviceState
{
	/// Unknown.
	InputDeviceState_Unknown,

	/// Ready.
	InputDeviceState_Ready,

	/// Busy.
	InputDeviceState_Busy,

	/// Removed.
	InputDeviceState_Removed,

	/// Lost.
	InputDeviceState_Lost
};

/**
	@ingroup InputDevices
	@page InputDevicesOverview Overview

	The InputDevice and associated classes provide a way of managing
	the input devices of a system..
*/

/**
	@brief Input device.
	@ingroup InputDevices

	Base class for input device implementations.
	This class provides the common methods and properties of all input devices.
*/
class SE_INPUT_EXPORT InputDevice : public RefObject
{
protected:
	InputSystem* _inputSystem;
	Window* _window;
	InputDeviceState _state;
	uint32 _port;
	uint32 _slot;

public:
	/** @name Constructor / Destructor. */
	//@{
	/**
		Constructor.
		@param inputSystem Pointer to the owner input context.
		@param name The name of the input device.
		@see InputSystem
	*/
	InputDevice(InputSystem* inputSystem);

	/** Destructor. */
	virtual ~InputDevice();
	//@}

	/** @name Creation / Destruction. */
	//@{
	/**
		Creates the input device.
		@param window Pointer to the window to attach the context to.
		@return true if successful; otherwise, false.
		@see Destroy
		@remark This method is automatically called by the Engine.
	*/
	virtual bool Create(Window* window) = 0;

	/**
		Destroys the input context.
		@return true if successful; otherwise, false.
		@see Destroy
		@remark This method is automatically called by the Engine. The context must not be used before a new creation.
	*/
	virtual bool Destroy() = 0;
	//@}

	/** @name Properties */
	//@{
	/**
		Returns the device type.
		@return Type of the device.
	*/
	virtual InputDeviceType GetDeviceType() const = 0;

	/** Returns the owner input context of this device.
		@return The owner input system.
	*/
	InputSystem* GetInputSystem() { return _inputSystem; }

	/** Gets the window to which this input device is attached.
		@return The window.
	*/
	Window* GetWindow() const { return _window; }

	/** Gets the state of the input device.
		@return The state of the input device.
	*/
	InputDeviceState GetState() const { return _state; }

	/** 
		Returns the port the device is connected to.
		@return The port the device is connected to.
	*/
	uint32 GetPort() const { return _port; }

	/** 
		Returns the slot the device is connected to.
		@return The slot the device is connected to.
	*/
	uint32 GetSlot() const { return _slot; }
	//@}

	/** @name Device information. */
	//@{
	/** 
		Gets the product name of the device.
		@return Product name of the device.
	*/
	virtual String GetProductName() const = 0;

	/** 
		Gets the Global Unique IDentifier of the device.
		@return String representing the Global Unique IDentifier of the device.
	*/
	virtual String GetGUID() const = 0;
	//@}

	/** @name Device acess. */
	//@{
	/** 
		Obtains access to the input device.
		@return true if successful; otherwise, false.
	*/
	virtual bool Acquire() = 0;

	/** 
		Releases access to the device.
		@return true if successful; otherwise, false.
	*/
	virtual bool Unacquire() = 0;
	//@}

	/** @name Reading. */
	//@{
	/** 
		Reads the input device state.
		@return true if successful; otherwise, false.
	*/
	virtual bool Read() = 0;
	//@}
};

}
}

#endif 
