/*=============================================================================
ArgumentNullException.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ARGUMENTNULLEXCEPTION_H_
#define _SE_ARGUMENTNULLEXCEPTION_H_

#include "Core/Common.h"
#include "Core/Exception/ArgumentException.h"

namespace SonataEngine
{

/** The exception that is thrown when a null pointer is passed to a method that does not accept it as a valid argument.
*/
class SE_CORE_EXPORT ArgumentNullException : public ArgumentException
{
	typedef ArgumentException super;
	static String DefaultMessage;

public:
	ArgumentNullException() :
		ArgumentException()
	{
	}

	ArgumentNullException(const String& paramName) :
		ArgumentException(paramName)
	{
	}

	ArgumentNullException(const String& paramName, const String& message) :
		ArgumentException(paramName, message)
	{
	}

	ArgumentNullException(const ArgumentNullException& exception) :
		ArgumentException(exception)
	{
	}

	virtual ~ArgumentNullException()
	{
	}
};

}

#endif 
