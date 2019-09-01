/*=============================================================================
MemoryStream.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_MEMORYSTREAM_H_
#define _SE_MEMORYSTREAM_H_

#include "Core/Common.h"
#include "Core/IO/Stream.h"

namespace SonataEngine
{

class File;

/**
	@brief Memory stream.

	Provides a memory stream.
*/
class SE_CORE_EXPORT MemoryStream : public Stream
{
private:
	SEbyte* _buffer;
	int32 _capacity;
	bool _writable;
	bool _expandable;
	bool _exposable;
	bool _isOpen;
	int32 _origin;
	int32 _length;
	int32 _position;

public:
	/** @name Constructor / Destructor */
	//@{
	/** Constructor. */
	MemoryStream();

	/** Initializes a new instance of a memory stream with an expandable capacity initialized to zero.
		@param capacity The initial size of the internal array in bytes.
	*/
	MemoryStream(int32 capacity);

	/** Initializes a new instance of a memory stream based on the specified byte array.
		@param buffer The array of unsigned bytes from which to create the current stream.
		@param count The length of the stream in bytes.
		@param writable Determines whether the stream supports writing.
	*/
	MemoryStream(SEbyte* buffer, int32 count, bool writable = true);

	/** Initializes a new instance of a memory stream based on the specified byte array.
		@param buffer The array of unsigned bytes from which to create the current stream.
		@param index The index into buffer at which the stream begins.
		@param count The length of the stream in bytes.
		@param writable Determines whether the stream supports writing.
	*/
	MemoryStream(SEbyte* buffer, int32 index, int32 count, bool writable = true);

	/** Destructor. */
	virtual ~MemoryStream();
	//@}

	virtual StreamType GetStreamType() const { return StreamType_Memory; }

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

	/** Returns the capacity of the stream.
		@return The capacity of the stream.
	*/
	int32 GetCapacity() const;

	/** Sets the capacity of the stream.
		@param value The capacity of the stream.
	*/
	void SetCapacity(int32 value);

	/// Returns the array of unsigned bytes from which this stream was created.
	SEbyte* GetBuffer() const;

private:
	void Initialize();
};

typedef SmartPtr<MemoryStream> MemoryStreamPtr;

}

#endif 
