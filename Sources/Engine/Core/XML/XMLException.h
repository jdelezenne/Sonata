/*=============================================================================
XMLException.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_XMLEXCEPTION_H_
#define _SE_XMLEXCEPTION_H_

#include "Core/Common.h"
#include "Core/Exception/Exception.h"

namespace SonataEngine
{

/**
	Returns detailed information about the last exception.
*/
class SE_CORE_EXPORT XMLException : public Exception
{
	typedef Exception super;
	static String DefaultMessage;

public:
	XMLException() :
		super(),
		_LineNumber(0),
		_LinePosition(0)
	{}

	XMLException(const String& message) :
		super(message),
		_LineNumber(0),
		_LinePosition(0)
	{}

	XMLException(const String& message, Exception* innerException) :
		super(message),
		_LineNumber(0),
		_LinePosition(0)
	{}

	XMLException(const String& message, int32 lineNumber, int32 linePosition) :
		super(message),
		_LineNumber(lineNumber),
		_LinePosition(linePosition)
	{}

	XMLException(const XMLException& exception) :
		super(exception),
		_LineNumber(exception.GetLineNumber()),
		_LinePosition(exception.GetLinePosition())
	{}

	virtual ~XMLException()
	{}

	/** Gets the line number indicating where the error occurred.
		@return The line number indicating where the error occurred.
	*/
	int32 GetLineNumber() const { return _LineNumber; }

	/** Gets the line position indicating where the error occurred.
		@return The line position indicating where the error occurred.
	*/
	int32 GetLinePosition() const { return _LinePosition; }

protected:
	int32 _LineNumber;
	int32 _LinePosition;
};

}

#endif 
