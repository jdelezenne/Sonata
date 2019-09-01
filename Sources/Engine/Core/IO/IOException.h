/*=============================================================================
IOException.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_IOEXCEPTION_H_
#define _SE_IOEXCEPTION_H_

#include "Core/Common.h"
#include "Core/Exception/Exception.h"

namespace SonataEngine
{

/** The exception that is thrown when an I/O error occurs. */
class SE_CORE_EXPORT IOException : public Exception
{
public:
	typedef Exception super;
	static String DefaultMessage;

protected:
	int32 _lineNumber;
	int32 _linePosition;

public:
	IOException() :
		super(),
		_lineNumber(0),
		_linePosition(0)
	{
	}

	IOException(const String& message) :
		super(message),
		_lineNumber(0),
		_linePosition(0)
	{
	}

	IOException(const String& message, int32 lineNumber, int32 linePosition) :
		super(message),
		_lineNumber(lineNumber),
		_linePosition(linePosition)
	{
	}

	IOException(const IOException& exception) :
		super(exception),
		_lineNumber(exception.GetLineNumber()),
		_linePosition(exception.GetLinePosition())
	{
	}

	virtual ~IOException()
	{
	}

	/** Gets the line number indicating where the error occurred.
		@return The line number indicating where the error occurred.
	*/
	int32 GetLineNumber() const { return _lineNumber; }

	/** Gets the line position indicating where the error occurred.
		@return The line position indicating where the error occurred.
	*/
	int32 GetLinePosition() const { return _linePosition; }
};

}

#endif 
