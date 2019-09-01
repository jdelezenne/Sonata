/*=============================================================================
InputMouse.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_INPUTMOUSE_H_
#define _SE_INPUTMOUSE_H_

#include "Core/Core.h"
#include "Input/InputDevice.h"

namespace SonataEngine
{
namespace Input
{

/** Mouse buttons. */
enum MouseButton
{
	MouseButton_0 = 0,
	MouseButton_Left = 0,
	MouseButton_1 = 1,
	MouseButton_Right = 1,
	MouseButton_2 = 2,
	MouseButton_Middle = 2,
	MouseButton_3 = 3,
	MouseButton_4 = 4,
	MouseButton_5 = 5,
	MouseButton_6 = 6,
	MouseButton_7 = 7
};

/**
	@brief Mouse device.
	@ingroup InputDevices
*/
class SE_INPUT_EXPORT InputMouse : public InputDevice
{
public:
	/** @name Constructor / Destructor. */
	//@{
	/**
		Constructor.
		@param inputSystem Pointer to the owner input context.
		@param name The name of the input device.
		@see InputSystem
	*/
	InputMouse(InputSystem* inputSystem) :
		InputDevice(inputSystem)
	{
	}

	/** Destructor. */
	virtual ~InputMouse()
	{
	}
	//@}

	virtual InputDeviceType GetDeviceType() const { return InputDeviceType_Mouse; }

	/** @name Mouse. */
	//@{
	/**
		Returns whether a mouse button is down (and from how long).
		@param button Button constant.
		@param buttonTime Pointer to be filled with the time the button has been pressed.
		@return true if the mouse button is down; otherwise, false.
	*/
	virtual bool IsMouseButtonDown(MouseButton button, uint32* buttonTime = NULL) const = 0;

	/** 
		Returns whether a mouse button is up.
		@param button Button constant.
		@return true if the mouse button is up; otherwise, false.
	*/
	virtual bool IsMouseButtonUp(MouseButton button) const = 0;

	/** 
		Returns whether a mouse button is clicked.
		@param button Button constant.
		@return true if the mouse button is clicked; otherwise, false.
	*/
	virtual bool IsMouseClicked(MouseButton button) const = 0;

	/** 
		Returns whether a mouse button is toggled.
		@param button Button constant.
		@return true if the mouse button is toggled; otherwise, false.
	*/
	virtual bool IsMouseToggled(MouseButton button) const = 0;

	/** 
		Returns the mouse position in screen coordinates.
		@param x X coordinate.
		@param y Y coordinate.
		@return Mouse position in screen coordinates.
	*/
	virtual void GetMousePosition(int32& x, int32& y) const = 0;

	/** 
		Returns the relative mouse position.
		@param x X coordinate.
		@param y Y coordinate.
		@return Mouse relative position.
	*/
	virtual void GetMouseRelativePosition(int32& x, int32& y) const = 0;

	/** 
		Returns the mouse wheel delta.
		@return Mouse wheel delta.
	*/
	virtual int32 GetMouseWheel() const = 0;
	//@}
};

}
}

#endif 
