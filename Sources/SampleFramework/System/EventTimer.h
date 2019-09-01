/*=============================================================================
EventTimer.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SFW_EVENTTIMER_H_
#define _SFW_EVENTTIMER_H_

#include "SampleFramework/Common.h"

namespace SampleFramework
{

/** Provides data for the Elapsed event. */
class SE_SAMPLEFRAMEWORK_EXPORT ElapsedEventArgs : public EventArgs
{
public:
	/// The time the event was raised.
	real64 EventTime;

public:
	ElapsedEventArgs(real64 eventTime) :
		EventArgs(),
		EventTime(eventTime)
	{
	}
};

/**
	@brief Timer that generates events.

	This timer needs to be registered to the Application to be updated.
	Once registered, the Elapsed event will be automatically raised by the Application.
*/
class SE_SAMPLEFRAMEWORK_EXPORT EventTimer : public Object
{
	friend class Application;

protected:
	bool _isEnabled;
	bool _autoReset;
	real64 _interval;
	real64 _lastTime;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	EventTimer(real64 interval = 100.0);

	/** Destructor. */
	virtual ~EventTimer();
	//@}

	/** @name Properties. */
	//@{
	/** Gets a value indicating whether the Timer should raise the Elapsed event. */
	bool GetEnabled() const { return _isEnabled; }

	/** Sets a value indicating whether the Timer should raise the Elapsed event. */
	void SetEnabled(bool value);

	/** Gets a value indicating whether the Timer should raise the Elapsed event each
		time the specified interval elapses or only after the first time it elapses. */
	bool GetAutoReset() const { return _autoReset; }

	/** Sets a value indicating whether the Timer should raise the Elapsed event each
		time the specified interval elapses or only after the first time it elapses. */
	void SetAutoReset(bool value) { _autoReset = value; }

	/** Gets the interval at which to raise the Elapsed event. */
	real64 GetInterval() const { return _interval; }

	/** Sets the interval at which to raise the Elapsed event. */
	void SetInterval(real64 value) { _interval = value; }
	//@}

	/** @name Events. */
	//@{
	/** Occurs when the interval elapses. */
	Event<ElapsedEventArgs> Elapsed;
	//@}

	/** @name Operations. */
	//@{
	/** Starts the Timer. */
	void Start();

	/** Stops the Timer. */
	void Stop();

	/** Releases the resources used by the Timer. */
	void Close();
	//@}

protected:
	void Update();
};

}

#endif 
