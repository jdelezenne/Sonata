/*=============================================================================
UnderflowException.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UNDERFLOWEXCEPTION_H_
#define _SE_UNDERFLOWEXCEPTION_H_


#include "Core/Common.h"
#include "Core/Exception/ArithmeticException.h"


namespace SonataEngine
{


/** The exception that is thrown when an arithmetic, casting, or conversion operation in a checked context results in an underflow.
*/
class SE_CORE_EXPORT UnderflowException : public ArithmeticException
{
	typedef ArithmeticException super;
	static String DefaultMessage;

public:
	UnderflowException() :
		ArithmeticException()
	{
	}

	UnderflowException(const String& message) :
		ArithmeticException(message)
	{
	}

	UnderflowException(const UnderflowException& exception) :
		ArithmeticException(exception)
	{
	}

	virtual ~UnderflowException()
	{
	}
};

}

#endif 
