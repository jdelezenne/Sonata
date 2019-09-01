/*=============================================================================
FormatException.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_FORMATEXCEPTION_H_
#define _SE_FORMATEXCEPTION_H_

#include "Core/Common.h"
#include "Core/Exception/Exception.h"

namespace SonataEngine
{

/** The exception that is thrown when the format of an argument does not meet the parameter specifications of the invoked method.
*/
class SE_CORE_EXPORT FormatException : public Exception
{
	typedef Exception super;
	static String DefaultMessage;

public:
	FormatException() :
		Exception()
	{
	}

	FormatException(const String& message) :
		Exception(message)
	{
	}

	FormatException(const FormatException& exception) :
		Exception(exception)
	{
	}

	virtual ~FormatException()
	{
	}
};

}

#endif 
