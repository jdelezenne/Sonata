/*=============================================================================
DivideByZeroException.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_DIVIDEBYZeroEXCEPTION_H_
#define _SE_DIVIDEBYZeroEXCEPTION_H_

#include "Core/Common.h"
#include "Core/Exception/ArithmeticException.h"

namespace SonataEngine
{

/** The exception that is thrown when there is an attempt to divide an integral or decimal value by zero.
*/
class SE_CORE_EXPORT DivideByZeroException : public ArithmeticException
{
	typedef ArithmeticException super;
	static String DefaultMessage;

public:
	DivideByZeroException() :
		ArithmeticException()
	{
	}

	DivideByZeroException(const String& message) :
		ArithmeticException(message)
	{
	}

	DivideByZeroException(const DivideByZeroException& exception) :
		ArithmeticException(exception)
	{
	}

	virtual ~DivideByZeroException()
	{
	}
};

}

#endif 
