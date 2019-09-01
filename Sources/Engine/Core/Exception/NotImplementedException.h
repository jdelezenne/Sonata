/*=============================================================================
NotImplementedException.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_NOTIMPLEMENTEDEXCEPTION_H_
#define _SE_NOTIMPLEMENTEDEXCEPTION_H_

#include "Core/Common.h"
#include "Core/Exception/Exception.h"

namespace SonataEngine
{

/** The exception that is thrown when a requested method or operation is not implemented.
*/
class SE_CORE_EXPORT NotImplementedException : public Exception
{
	typedef Exception super;
	static String DefaultMessage;

public:
	NotImplementedException() :
		Exception()
	{
	}

	NotImplementedException(const String& message) :
		Exception(message)
	{
	}

	NotImplementedException(const NotImplementedException& exception) :
		Exception(exception)
	{
	}

	virtual ~NotImplementedException()
	{
	}
};

}

#endif 
