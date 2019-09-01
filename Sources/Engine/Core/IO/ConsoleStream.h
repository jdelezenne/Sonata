/*=============================================================================
ConsoleStream.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_CONSOLESTREAM_H_
#define _SE_CONSOLESTREAM_H_

#include "Core/Common.h"
#include "Core/IO/Stream.h"

namespace SonataEngine
{

enum StdConsoleType
{
	StdConsoleType_Input = 1,
	StdConsoleType_Output = 2,
	StdConsoleType_Error = 3
};

/**
	@brief Console stream.

	This stream is the operating-system console stream.
	This class is platform-specific and needs to be implemented in the
	corresponding Platforms package.
	This class is only intended to be used internally.
*/
class SE_CORE_EXPORT ConsoleStream : public Stream
{
public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	ConsoleStream();

	/**
		Constructor with a console type.
		@param type The type of the console.
	*/
	ConsoleStream(StdConsoleType type);

	/** Destructor. */
	virtual ~ConsoleStream();
	//@}

public:
	virtual StreamType GetStreamType() const { return StreamType_Unknown; }

	virtual bool CanRead() const;
	virtual bool CanWrite() const;
	virtual bool CanSeek() const;

	virtual int32 GetLength() const;
	virtual void SetLength(int32 value);
	virtual int32 GetPosition() const;
	virtual void SetPosition(int32 value);
	virtual void Close();
	virtual void Flush();
	virtual int32 Seek(int32 offset, SeekOrigin origin);
	virtual bool IsEOF() const;
	virtual SEbyte ReadByte();
	virtual int32 Read(SEbyte* buffer, int32 count);
	virtual void WriteByte(SEbyte value);
	virtual int32 Write(const SEbyte* buffer, int32 count);

public:
	static ConsoleStream* StandardInput;
	static ConsoleStream* StandardOutput;
	static ConsoleStream* StandardError;

private:
	StdConsoleType _ConsoleType;
	void* _handle;
};

}

#endif 
