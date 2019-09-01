/*=============================================================================
InputGamepad.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_INPUTJOYSTICK_H_
#define _SE_INPUTJOYSTICK_H_

#include "Core/Core.h"
#include "Input/InputDevice.h"

namespace SonataEngine
{
namespace Input
{

/** Gamepad axis. */
enum GamepadAxis
{
	GamepadAxis_XAxis = 0, 
	GamepadAxis_YAxis = 1, 
	GamepadAxis_ZAxis = 2
};

/** Gamepad object types. */
enum GamepadObjectType
{
	GamepadObjectType_Unknown = 0,
	GamepadObjectType_XAxis = 1,
	GamepadObjectType_YAxis = 2,
	GamepadObjectType_ZAxis = 3,
	GamepadObjectType_RxAxis = 4,
	GamepadObjectType_RyAxis = 5,
	GamepadObjectType_RzAxis = 6,
	GamepadObjectType_Slider = 7,
	GamepadObjectType_Button = 8,
	GamepadObjectType_Key = 9,
	GamepadObjectType_POV = 10
};

/** Gamepad object. */
struct GamepadObject
{
	GamepadObjectType Type;
	int Index;
	String Name;
	int32 RangeMin;
	int32 RangeMax;
};

typedef BaseArray<GamepadObject> GamepadObjectList;

/**
	@brief Gamepad device.
	@ingroup InputDevices
*/
class SE_INPUT_EXPORT InputGamepad : public InputDevice
{
public:
	/** @name Constructors / Destructor. */
	//@{
	/** 
		Constructor.
		@param inputSystem Pointer to the owner input context.
		@param name The name of the input device.
		@see InputSystem
	*/
	InputGamepad(InputSystem* inputSystem) :
		InputDevice(inputSystem)
	{
	}

	/** Destructor. */
	virtual ~InputGamepad()
	{
	}
	//@}

	virtual InputDeviceType GetDeviceType() const { return InputDeviceType_Gamepad; }

	/** @name Device information. */
	//@{
	/** 
		Retrieves the gamepad object list of the device.
		@param objectList Gamepad object list of the device.
	*/
	virtual void GetGamepadObjectList(GamepadObjectList& objectList) const = 0;

	/** 
		Builds a list with the gamepad object of the device.
		@param type Type of the input objects to retrieve.
		@param objectList Gamepad object list of the device.
	*/
	virtual void GetGamepadObjectList(GamepadObjectType type, GamepadObjectList& objectList) const = 0;

	/** 
		Gets the number of buttons available on the device.
		@return Number of buttons available on the device.
	*/
	virtual int32 GetButtonCount() const = 0;

	/** 
		Retrieves the number of axis available on the device.
		@return Number of axis available on the device.
	*/
	virtual int32 GetAxisCount() const = 0;

	/** 
		Retrieves the number of point-of-view controllers available on the device.
		@return Number of point-of-view controllers available on the device.
	*/
	virtual int32 GetPOVCount() const = 0;
	//@}

	/** @name Gamepad. */
	//@{
	/** 
		Returns whether a gamepad button is down (and from how long).
		@param button Button index (0 to 31).
		@param buttonTime Pointer to be filled with the time the button has been pressed.
		@return true if the gamepad button is down; otherwise, false.
	*/
	virtual bool IsButtonDown(int32 button, uint32* buttonTime = NULL) const = 0;

	/** 
		Returns whether a gamepad button is up.
		@param button Button index (0 to 31).
		@return true if the gamepad button is up; otherwise, false.
	*/
	virtual bool IsButtonUp(int32 button) const = 0;

	/** 
		Returns whether a gamepad button is pushed.
		@param button Button index (0 to 31).
		@return true if the gamepad button is pushed; otherwise, false.
	*/
	virtual bool IsButtonPushed(int32 button) const = 0;

	/** 
		Returns whether a gamepad button is toggled.
		@param button Button index (0 to 31).
		@return true if the gamepad button is toggled; otherwise, false.
	*/
	virtual bool IsButtonToggled(int32 button) const = 0;

	/** 
		Returns whether the given gamepad position axis is available on the device.
		@param axis Gamepad axis.
		@return true if the gamepad position axis is available; otherwise, false.
	*/
	virtual bool IsAxisPosition(GamepadAxis axis) const = 0;

	/** 
		Returns whether the given gamepad rotation axis is available on the device.
		@param axis Gamepad axis.
		@return true if the gamepad rotation axis is available; otherwise, false.
	*/
	virtual bool IsAxisRotation(GamepadAxis axis) const = 0;

	/** 
		Retrieves the value of the given gamepad position axis.
		@param axis Gamepad axis.
		@return Gamepad position axis value.
	*/
	virtual int32 GetAxisPosition(GamepadAxis axis) const = 0;

	/** 
		Retrieves the value of the given gamepad rotation axis.
		@param axis Gamepad rotation axis.
		@return Gamepad rotation axis value.
	*/
	virtual int32 GetAxisRotation(GamepadAxis axis) const = 0;

	/** 
		Retrieves the u-axis or v-axis value.
		@param slider Slider index (0 to 1).
		@return Slider value.
	*/
	virtual int32 GetSlider(int32 slider) const = 0;

	/** 
		Retrieves the direction controller value.
		@param pov Controller index (0 to 3).
		@return POV value.
	*/
	virtual int32 GetPointOfView(int32 pov) const = 0;
	//@}
};

}
}

#endif 
