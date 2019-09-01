/*=============================================================================
IndexOutOfRangeException.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_INDEXOUTOFRANGEEXCEPTION_H_
#define _SE_INDEXOUTOFRANGEEXCEPTION_H_

#include "Core/Common.h"
#include "Core/Exception/Exception.h"

namespace SonataEngine
{

/** The exception that is thrown when an attempt is made to access an element of an array with an index that is outside the bounds of the array.
*/
class SE_CORE_EXPORT IndexOutOfRangeException : public Exception
{
	typedef Exception super;
	static String DefaultMessage;

public:
	IndexOutOfRangeException() :
		Exception()
	{
	}

	IndexOutOfRangeException(const String& message) :
		Exception(message)
	{
	}

	IndexOutOfRangeException(const IndexOutOfRangeException& exception) :
		Exception(exception)
	{
	}

	virtual ~IndexOutOfRangeException()
	{
	}
};

}

#endif 
