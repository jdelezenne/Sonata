/*=============================================================================
InputSystem.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_INPUTSYSTEM_H_
#define _SE_INPUTSYSTEM_H_

#include "Core/Core.h"
#include "Input/Common.h"

namespace SonataEngine
{
namespace Input
{

class InputDevice;
class InputMouse;
class InputKeyboard;
class InputGamepad;

/** Input Device Types. */
enum InputDeviceType
{
	/// Unknown.
	InputDeviceType_Unknown,

	/// Keyboard.
	InputDeviceType_Keyboard,

	/// Mouse.
	InputDeviceType_Mouse,

	/// Gamepad. Represents any type of game controller such as Joysticks or LightGuns.
	InputDeviceType_Gamepad
};

/**
	@brief Base class for input system implementations.
	
	An input system manages the active inputs devices.
	There three different types of input devices: keyboards, mouses and gamepads.
	You can create several input devices, but usually,
	there will be only one keyboard and one mouse.
*/
class SE_INPUT_EXPORT InputSystem : public Manager, public Context<InputSystem>
{
public:
	typedef Array<InputDevice*> InputDeviceList;

protected:
	String _name;
	InputDeviceList _inputDevices;

public:
	/** @name Constructors / Destructor. */
	/** Constructor. */
	InputSystem();

	/**
		Constructor.
		@param name The name of the input driver.
	*/
	InputSystem(const String& name);

	/** Destructor. */
	virtual ~InputSystem();

	/** @name Creation / Destruction. */
	//@{
	/**
		Creates the input context.
		@return true if successful; otherwise, false.
		@see Destroy
		@remark This method is automatically called by the Engine.
	*/
	virtual bool Create() = 0;

	/**
		Destroys the input context.
		@return true if successful; otherwise, false.
		@see Create
		@remark This method is automatically called by the Engine. The context must not be used before a new creation.
	*/
	virtual void Destroy() = 0;
	//@}

	virtual void Update(real64 elapsed);

	/** @name Input Devices. */
	//@{
	int GetInputDeviceCount() const;
	InputDeviceList::Iterator GetInputDeviceIterator() const;
	bool RemoveInputDevice(InputDevice* inputDevice);
	void RemoveInputDevices(InputDeviceType type);
	void RemoveAllInputDevices();
	InputDevice* GetInputDevice(int index) const;
	InputDevice* GetInputDeviceByType(InputDeviceType type, int index = 0) const;
	//@}

	/**
		Reads all the input devices of a given type.
		@param type Type of the device to read.
	*/
	void ReadDevices(InputDeviceType type);

	/**
		Reads all the input devices.
	*/
	void ReadAllDevices();

	/**
		Enumerates available devices.
		@return true if successful; otherwise, false.
	*/
	virtual bool EnumDevices() = 0;
	//@}

	/**
		Shows or hides the mouse cursor.
		@param value TRUE to show the mouse cursor, FALSE to hide it.
	*/
	virtual void SetCursorVisible(bool value) = 0;

	/**
		Locks or unlocks the mouse cursor.
		When locked, the mouse cursor is always centered in the current window.
		@param value TRUE to lock the mouse cursor; otherwise, false.
	*/
	virtual void SetCursorLocked(bool value) = 0;

protected:
	bool _AddInputDevice(InputDevice* inputDevice);

	virtual void _DestroyInputDevice(InputDevice* inputDevice) = 0;
};

SEPointer(InputSystem);

}
}

#endif 
