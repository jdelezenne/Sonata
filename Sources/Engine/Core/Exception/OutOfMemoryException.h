/*=============================================================================
OutOfMemoryException.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_OUTOFMEMORYEXCEPTION_H_
#define _SE_OUTOFMEMORYEXCEPTION_H_

#include "Core/Common.h"
#include "Core/Exception/Exception.h"

namespace SonataEngine
{

/** The exception that is thrown when there is not enough memory to continue the execution of a program.
*/
class SE_CORE_EXPORT OutOfMemoryException : public Exception
{
	typedef Exception super;
	static String DefaultMessage;

public:
	OutOfMemoryException() :
		Exception()
	{
	}

	OutOfMemoryException(const String& message) :
		Exception(message)
	{
	}

	OutOfMemoryException(const OutOfMemoryException& exception) :
		Exception(exception)
	{
	}

	virtual ~OutOfMemoryException()
	{
	}
};

}

#endif 
