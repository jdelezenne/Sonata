/*=============================================================================
StackOverflowException.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_STACKOVERFLOWEXCEPTION_H_
#define _SE_STACKOVERFLOWEXCEPTION_H_

#include "Core/Common.h"
#include "Core/Exception/Exception.h"

namespace SonataEngine
{

/** The exception that is thrown when the execution stack overflows by having too many pending method calls.
*/
class SE_CORE_EXPORT StackOverflowException : public Exception
{
	typedef Exception super;
	static String DefaultMessage;

public:
	StackOverflowException() :
		Exception()
	{
	}

	StackOverflowException(const String& message) :
		Exception(message)
	{
	}

	StackOverflowException(const StackOverflowException& exception) :
		Exception(exception)
	{
	}

	virtual ~StackOverflowException()
	{
	}
};

}

#endif 
