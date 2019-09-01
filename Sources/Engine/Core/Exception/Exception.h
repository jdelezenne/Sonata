/*=============================================================================
Exception.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_EXCEPTION_H_
#define _SE_EXCEPTION_H_

#include "Core/Common.h"
#include "Core/String.h"

namespace SonataEngine
{

/**
	@brief Exception.

	Represents errors that occur during application execution.
*/
class SE_CORE_EXPORT Exception
{
protected:
	String _message;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Initializes a new exception. */
	Exception();

	/**
		Initializes a new exception with a specified error message.
		@param message The message that describes the error.
	*/
	Exception(const String& message);

	/** Copy constructor. */
	Exception(const Exception& exception);

	/** Destructor. */
	virtual ~Exception();
	//@}

	/** Gets a message that describes the current exception.
		@return The message that describes the current exception.
	*/
	virtual String GetMessage() const { return _message; }
};

}

#endif 
