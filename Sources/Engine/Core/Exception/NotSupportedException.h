/*=============================================================================
NotSupportedException.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_NOTSUPPORTEDEXCEPTION_H_
#define _SE_NOTSUPPORTEDEXCEPTION_H_

#include "Core/Common.h"
#include "Core/Exception/Exception.h"

namespace SonataEngine
{

/** The exception that is thrown when an invoked method is not supported.
*/
class SE_CORE_EXPORT NotSupportedException : public Exception
{
	typedef Exception super;
	static String DefaultMessage;

public:
	NotSupportedException() :
		Exception()
	{
	}

	NotSupportedException(const String& message) :
		Exception(message)
	{
	}

	NotSupportedException(const NotSupportedException& exception) :
		Exception(exception)
	{
	}

	virtual ~NotSupportedException()
	{
	}
};

}

#endif 
