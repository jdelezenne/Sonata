/*=============================================================================
AccessViolationException.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ACCESSVIOLATIONEXCEPTION_H_
#define _SE_ACCESSVIOLATIONEXCEPTION_H_

#include "Core/Common.h"
#include "Core/Exception/Exception.h"

namespace SonataEngine
{

/** The exception that is thrown when the thread tried to read from or write to a virtual address for which it does not have the appropriate access.
*/
class SE_CORE_EXPORT AccessViolationException : public Exception
{
	typedef Exception super;
	static String DefaultMessage;

public:
	AccessViolationException() :
		Exception()
	{
	}

	AccessViolationException(const String& message) :
		Exception(message)
	{
	}

	AccessViolationException(const AccessViolationException& exception) :
		Exception(exception)
	{
	}

	virtual ~AccessViolationException()
	{
	}
};

}

#endif 
