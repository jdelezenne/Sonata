/*=============================================================================
BinaryStream.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_BINARYSTREAM_H_
#define _SE_BINARYSTREAM_H_

#include "Core/Common.h"
#include "Core/IO/Stream.h"

namespace SonataEngine
{

class File;

/**
	@brief Binary stream.

	This class can stream data types.
	@see Stream TextStream.
*/
class SE_CORE_EXPORT BinaryStream
{
private:
	Stream* _stream;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor.
		@param stream The source steam.
	*/
	BinaryStream(Stream* stream);

	/** Destructor. */
	virtual ~BinaryStream();
	//@}

public:
	/** Returns the source stream. */
	Stream* GetStream() const { return _stream; }

	/** Closes the stream. */
	virtual void Close();

	/** Returns the next available byte from the stream if seeking is possible. */
	virtual SEbyte Peek();

	/** Reads the next byte from the stream. */
	virtual SEbyte Read();

	virtual int8 ReadInt8();
	virtual uint8 ReadUInt8();
	virtual int16 ReadInt16();
	virtual uint16 ReadUInt16();
	virtual int32 ReadInt32();
	virtual uint32 ReadUInt32();
	virtual int64 ReadInt64();
	virtual uint64 ReadUInt64();
	virtual real32 ReadReal32();
	virtual real64 ReadReal64();
	virtual String ReadString();

	/** Reads the next set of bytes from the stream. */
	virtual int Read(SEbyte* buffer, int32 count);

	/** Writes a byte to the text stream. */
	virtual void Write(SEbyte value);

	virtual void WriteInt8(int8 value);
	virtual void WriteUInt8(uint8 value);
	virtual void WriteInt16(int16 value);
	virtual void WriteUInt16(uint16 value);
	virtual void WriteInt32(int32 value);
	virtual void WriteUInt32(uint32 value);
	virtual void WriteInt64(int64 value);
	virtual void WriteUInt64(uint64 value);
	virtual void WriteReal32(real32 value);
	virtual void WriteReal64(real64 value);
	virtual void WriteString(const String& value);

	/** Writes an array of bytes to the stream. */
	virtual void Write(SEbyte* buffer, int32 count);
};

}

#endif 
