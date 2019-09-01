/*=============================================================================
Timer.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_TIMER_H_
#define _SE_TIMER_H_

#include "Core/Common.h"

namespace SonataEngine
{

/**
	@brief Plateform-independant Timer.

	The Timer is initially stopped.
*/
class SE_CORE_EXPORT Timer
{
public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	Timer();

	/** Destructor. */
	virtual ~Timer();
	//@}

	/** @name Properties. */
	//@{
	/** Returns whether the Timer is running. */
	bool IsRunning() const { return _IsRunning; }

	/** Gets the total elapsed time measured by the Timer. */
	real64 Elapsed() const;
	//@}

	/** @name Operations. */
	//@{
	/** Starts the Timer.
		@remarks Resumes the Timer if stopped.
	*/
	void Start();

	/** Stops the Timer. */
	void Stop();

	/** Resets the Timer. */
	void Reset();
	//@}

protected:
	bool _IsRunning;
	real64 _Elapsed;
	real64 _StartTime;
};

}

#endif 
