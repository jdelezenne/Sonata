/*=============================================================================
InvalidOperationException.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_INVALIDOPERATIONEXCEPTION_H_
#define _SE_INVALIDOPERATIONEXCEPTION_H_

#include "Core/Common.h"
#include "Core/Exception/Exception.h"

namespace SonataEngine
{

/** The exception that is thrown when a method call is invalid for the object's current state.
*/
class SE_CORE_EXPORT InvalidOperationException : public Exception
{
	typedef Exception super;
	static String DefaultMessage;

public:
	InvalidOperationException() :
		Exception()
	{
	}

	InvalidOperationException(const String& message) :
		Exception(message)
	{
	}

	InvalidOperationException(const InvalidOperationException& exception) :
		Exception(exception)
	{
	}

	virtual ~InvalidOperationException()
	{
	}
};

}

#endif 
