/*=============================================================================
OverflowException.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_OVERFLOWEXCEPTION_H_
#define _SE_OVERFLOWEXCEPTION_H_

#include "Core/Common.h"
#include "Core/Exception/ArithmeticException.h"

namespace SonataEngine
{

/** The exception that is thrown when an arithmetic, casting, or conversion operation in a checked context results in an overflow.
*/
class SE_CORE_EXPORT OverflowException : public ArithmeticException
{
	typedef ArithmeticException super;
	static String DefaultMessage;

public:
	OverflowException() :
		ArithmeticException()
	{
	}

	OverflowException(const String& message) :
		ArithmeticException(message)
	{
	}

	OverflowException(const OverflowException& exception) :
		ArithmeticException(exception)
	{
	}

	virtual ~OverflowException()
	{
	}
};

}

#endif 
