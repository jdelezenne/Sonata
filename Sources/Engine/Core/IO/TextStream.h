/*=============================================================================
TextStream.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_TEXTSTREAM_H_
#define _SE_TEXTSTREAM_H_

#include "Core/Common.h"
#include "Core/IO/Stream.h"

namespace SonataEngine
{

/**
	@class TextStream.
	@brief This class can stream characters using a binary stream.
	@see Stream.
*/
class SE_CORE_EXPORT TextStream
{
private:
	Stream* _stream;
	String _newLine;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor.
		@param stream The source steam.
	*/
	TextStream(Stream* stream);

	/** Destructor. */
	virtual ~TextStream();
	//@}

	/** Returns the source stream. */
	Stream* GetStream() const { return _stream; }

	/** Gets the line terminator string used by the stream. */
	String GetNewLine() const { return _newLine; }

	/** Sets the line terminator string used by the stream. */
	void SetNewLine(const String& value) { _newLine = value; }

	/** Closes the stream. */
	virtual void Close();

	/** Returns the next available character from the stream if seeking is possible. */
	virtual SEchar Peek();

	/** Reads the next character from the stream. */
	virtual SEchar Read();

	/** Reads the next set of characters from the stream. */
	virtual int Read(SEchar* buffer, int32 count);

	/** Reads a line of characters from the stream. */
	virtual String ReadLine();

	/**  Reads the characters from the current position to the end of the stream. */
	virtual String ReadToEnd();

	/** Writes a character to the text stream. */
	virtual void Write(SEchar value);

	/** Writes a string to the text stream. */
	virtual void Write(const String& value);

	/** Writes a line terminator to the text stream. */
	virtual void WriteLine();

	/** Writes a character followed by a line terminator to the text stream. */
	virtual void WriteLine(SEchar value);

	/** Writes a string followed by a line terminator to the stream. */
	virtual void WriteLine(const String& value);
};

}

#endif 
