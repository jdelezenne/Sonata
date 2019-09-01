/*=============================================================================
ArgumentOutOfRangeException.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ARGUMENTOUTOFRANGEEXCEPTION_H_
#define _SE_ARGUMENTOUTOFRANGEEXCEPTION_H_

#include "Core/Common.h"
#include "Core/Exception/ArgumentException.h"
#include "Core/Variant.h"

namespace SonataEngine
{

/** The exception that is thrown when the value of an argument is outside the allowable range of values as defined by the invoked method.
*/
class SE_CORE_EXPORT ArgumentOutOfRangeException : public ArgumentException
{
	typedef ArgumentException super;
	static String DefaultMessage;

protected:
	Variant _paramValue;

public:
	ArgumentOutOfRangeException() :
		ArgumentException()
	{
	}

	ArgumentOutOfRangeException(const String& paramName) :
		ArgumentException(paramName)
	{
	}

	ArgumentOutOfRangeException(const String& paramName, const String& message) :
		ArgumentException(paramName, message)
	{
	}

	/** Initializes a new exception with a specified error message, the name of the parameter and the value of the argument.
		@param paramName The name of the parameter that causes this exception.
		@param paramValue The value of the parameter that causes this exception.
		@param message The message that describes the error.
	*/
	ArgumentOutOfRangeException(const String& paramName, const Variant& paramValue, const String& message) :
		ArgumentException(paramName, message),
		_paramValue(paramValue)
	{
	}

	ArgumentOutOfRangeException(const ArgumentOutOfRangeException& exception) :
		ArgumentException(exception)
	{
	}

	virtual ~ArgumentOutOfRangeException()
	{
	}

public:
	virtual String GetMessage() const
	{
		String str = super::GetMessage();
		if (!_paramName.IsEmpty())
		{
			str = str + Environment::NewLine() + _T("Parameter name: ") + _paramName + (".");

			if (_paramValue != Variant_Invalid)
			{
				str = str + Environment::NewLine() + _T("Parameter value: %p.") + _paramValue.ToString();
			}
		}

		return str;
	}

	/** Gets the value of the parameter that causes this exception.
		@return The value of the parameter that causes this exception.
	*/
	virtual Variant GetParamValue() const { return _paramValue; }
};

}

#endif 
