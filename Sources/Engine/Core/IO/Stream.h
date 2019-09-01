/*=============================================================================
Stream.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_STREAM_H_
#define _SE_STREAM_H_

#include "Core/Common.h"
#include "Core/RefCounter.h"
#include "Core/String.h"

namespace SonataEngine
{

/** Stream type. */
enum StreamType
{
	/** Invalid stream. */
	StreamType_Unknown,

	/** Memory stream. */
	StreamType_Memory,

	/** File stream. */
	StreamType_File,

	/** Network stream. */
	StreamType_Network
};

/** Reference position in a stream. */
enum SeekOrigin
{
	/** Beginning of the stream. */
	SeekOrigin_Begin,
	
	/** Current position of the stream. */
	SeekOrigin_Current,

	/** End of the stream. */
	SeekOrigin_End
};

/**
	@class Stream.
	@brief Base class for binary stream implementations.
*/
class SE_CORE_EXPORT Stream : public RefCounter
{
protected:
	ByteOrder _byteOrder;
	bool _swap;

public:
	/** @name Constructor / Destructor. */
	//@{
	/** Constructor. */
	Stream();

	/** Destructor. */
	virtual ~Stream();
	//@}

	/**
		Retrieves the type of the stream.
		@return The type of the stream.
	*/
	virtual StreamType GetStreamType() const { return StreamType_Unknown; }

	/**
		Returns the byte order of the stream.
		@return The byte order of the stream.
	*/
	ByteOrder GetByteOrder() const { return _byteOrder; }

	/**
		Sets the byte order of the stream.
		@param value A byte order.
	*/
	void SetByteOrder(ByteOrder value);

	/**
		Returns whether the stream supports reading.
		@return true if the stream supports reading; otherwise, false.
	*/
	virtual bool CanRead() const = 0;

	/**
		Returns whether the stream supports writing.
		@return true if the stream supports writing; otherwise, false.
	*/
	virtual bool CanWrite() const = 0;

	/**
		Returns whether the stream supports seeking.
		@return true if the stream supports seeking; otherwise, false.
	*/
	virtual bool CanSeek() const = 0;

	/**
		Returns the length of the stream.
		@return The length of the stream.
	*/
	virtual int32 GetLength() const = 0;

	/**
		Sets the length of the stream.
		@param value Length of the stream.
	*/
	virtual void SetLength(int32 value) = 0;

	/**
		Returns the position within the stream.
		@return The position within the stream.
	*/
	virtual int32 GetPosition() const = 0;

	/**
		Sets the position within the stream.
		@param value Position within the stream.
	*/
	virtual void SetPosition(int32 value) = 0;

	/** Closes the stream. */
	virtual void Close() = 0;

	/** Clears the buffers of the stream. */
	virtual void Flush() = 0;

	/**
		Sets the position within the stream.
		@param offset The offset relative to the origin parameter.
		@param origin The origin of the new position.
		@return The new position within the stream.
	*/
	virtual int32 Seek(int32 offset, SeekOrigin origin) = 0;

	/**
		Returns whether the stream has reached the end position.
		@return true if the stream has reached the end position; otherwise, false.
	*/
	virtual bool IsEOF() const = 0;

	/**
		Reads a byte from the stream and advances the position within the stream by one byte.
		@return The byte read from the stream.
	*/
	virtual SEbyte ReadByte() = 0;

	/**
		Reads a sequence of bytes from the stream and advances the position within the stream by the number of bytes read.
		@param buffer The buffer preallocated to be filled with the bytes read from the stream .
		@param count The maximum number of bytes to be read from the stream .
		@return The number of bytes read from the stream.
	*/
	virtual int32 Read(SEbyte* buffer, int32 count) = 0;

	/**
		Writes a byte to the stream and advances the position within the stream by one byte.
		@param value The byte to write to the stream.
	*/
	virtual void WriteByte(SEbyte value) = 0;

	/**
		Writes a sequence of bytes to the stream and advances the position within the stream by the number of bytes written.
		@param buffer The buffer to be written to the stream .
		@param count The maximum number of bytes to be written to the stream .
	*/
	virtual int32 Write(const SEbyte* buffer, int32 count) = 0;

	Stream& operator>>(int8& i);
	Stream& operator>>(uint8& i);
	Stream& operator>>(int16& i);
	Stream& operator>>(uint16& i);
	Stream& operator>>(int32& i);
	Stream& operator>>(uint32& i);
	Stream& operator>>(int64& i);
	Stream& operator>>(uint64& i);
	Stream& operator>>(bool& b);
	Stream& operator>>(real32& r);
	Stream& operator>>(real64& r);
	Stream& operator>>(String& str);

	Stream& operator<<(int8 i);
	Stream& operator<<(uint8 i);
	Stream& operator<<(int16 i);
	Stream& operator<<(uint16 i);
	Stream& operator<<(int32 i);
	Stream& operator<<(uint32 i);
	Stream& operator<<(int64 i);
	Stream& operator<<(uint64 i);
	Stream& operator<<(bool b);
	Stream& operator<<(real32 r);
	Stream& operator<<(real64 r);
	Stream& operator<<(const String& str);
};

}

#endif 
