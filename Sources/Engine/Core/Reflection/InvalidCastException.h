/*=============================================================================
InvalidCastException.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_INVALIDCASTEXCEPTION_H_
#define _SE_INVALIDCASTEXCEPTION_H_

#include "Core/Common.h"
#include "Core/Exception/Exception.h"

namespace SonataEngine
{

/** The exception that is thrown for invalid casting or explicit conversion.
*/
class InvalidCastException : public Exception
{
	typedef Exception super;
	static String DefaultMessage;

public:
	InvalidCastException() :
		super()
	{}

	InvalidCastException(const String& message) :
		super(message)
	{}

	InvalidCastException(const String& message, Exception* innerException) :
		super(message, innerException)
	{}

	InvalidCastException(const InvalidCastException& exception) :
		super(exception)
	{}

	virtual ~InvalidCastException()
	{}
};

}

#endif 
