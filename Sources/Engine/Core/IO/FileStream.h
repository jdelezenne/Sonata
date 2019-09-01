/*=============================================================================
FileStream.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_FILESTREAM_H_
#define _SE_FILESTREAM_H_

#include "Core/Common.h"
#include "Core/IO/Stream.h"
#include "Core/IO/FileSystem.h"

namespace SonataEngine
{

class File;
class FileStreamInternal;

const int32 FileStream_BufferSize = 4096;

/**
	@class FileStream.
	@brief Provides data streaming with files.
*/
class SE_CORE_EXPORT FileStream : public Stream
{
public:
	/** @name Constructor / Destructor */
	//@{
	/** Constructor. */
	FileStream();

	/**
		Initializes a new instance of a file stream for the specified file.
		@param file A file
	*/
	FileStream(File* file);

	/** Destructor. */
	virtual ~FileStream();
	//@}

	virtual StreamType GetStreamType() const { return StreamType_File; }

	virtual bool CanRead() const { return true; }
	virtual bool CanWrite() const { return true; }
	virtual bool CanSeek() const { return true; }

	/** @name Platform specific. */
	//@{
	virtual int32 GetLength() const;
	virtual void SetLength(int32 value);
	virtual int32 GetPosition() const;
	virtual void SetPosition(int32 position);
	virtual void Close();
	virtual void Flush();
	virtual int32 Seek(int32 offset, SeekOrigin origin);
	virtual bool IsEOF() const;
	virtual SEbyte ReadByte();
	virtual int32 Read(SEbyte* buffer, int32 count);
	virtual void WriteByte(SEbyte value);
	virtual int32 Write(const SEbyte* buffer, int32 count);
	//@}

	/**
		Returns the file name of the file stream.
		@return The file name of the file stream.
	*/
	String GetFileName() const;

protected:
	FileStreamInternal* _internal;
};

typedef SmartPtr<FileStream> FileStreamPtr;

}

#endif 
