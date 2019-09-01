/*=============================================================================
ArgumentException.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ARGUMENTEXCEPTION_H_
#define _SE_ARGUMENTEXCEPTION_H_

#include "Core/Common.h"
#include "Core/Exception/Exception.h"
#include "Core/System/Environment.h"

namespace SonataEngine
{

/** The exception that is thrown when one of the arguments provided to a method is not valid..
*/
class SE_CORE_EXPORT ArgumentException : public Exception
{
	typedef Exception super;
	static String DefaultMessage;

protected:
	String _paramName;

public:
	/** Initializes a new exception. */
	ArgumentException() :
		Exception()
	{
	}

	/** Initializes a new exception with the name of the parameter that causes this exception.
		@param paramName The name of the parameter that causes this exception.
	*/
	ArgumentException(const String& paramName) :
		Exception(),
		_paramName(paramName)
	{
	}

	/** Initializes a new exception with a specified error message and the name of the parameter that causes this exception.
		@param paramName The name of the parameter that causes this exception.
		@param message The message that describes the error.
	*/
	ArgumentException(const String& paramName, const String& message) :
		Exception(message),
		_paramName(paramName)
	{
	}

	/** Copy constructor. */
	ArgumentException(const ArgumentException& exception) :
		Exception(exception)
	{
		_paramName = exception.GetParamName();
	}

	/** Destructor. */
	virtual ~ArgumentException()
	{
	}

public:
	virtual String GetMessage() const
	{
		String str = super::GetMessage();
		if (!_paramName.IsEmpty())
			return str + Environment::NewLine() + _T("Parameter name: ") + _paramName + (".");
		else
			return str;
	}

	/** Gets the name of the parameter that causes this exception.
		@return The name of the parameter that causes this exception.
	*/
	virtual String GetParamName() const { return _paramName; }
};

}

#endif 
