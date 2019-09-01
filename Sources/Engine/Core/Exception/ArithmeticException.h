/*=============================================================================
ArithmeticException.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ARITHMETICEXCEPTION_H_
#define _SE_ARITHMETICEXCEPTION_H_

#include "Core/Common.h"
#include "Core/Exception/Exception.h"

namespace SonataEngine
{

/** The exception that is thrown for errors in an arithmetic, casting, or conversion operation.
*/
class SE_CORE_EXPORT ArithmeticException : public Exception
{
	typedef Exception super;
	static String DefaultMessage;

public:
	ArithmeticException() :
		Exception()
	{
	}

	ArithmeticException(const String& message) :
		Exception(message)
	{
	}

	ArithmeticException(const ArithmeticException& exception) :
		Exception(exception)
	{
	}

	virtual ~ArithmeticException()
	{
	}
};

}

#endif 
