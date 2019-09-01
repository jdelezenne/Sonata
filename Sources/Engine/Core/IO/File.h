/*=============================================================================
File.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_FILE_H_
#define _SE_FILE_H_

#include "Core/Common.h"
#include "Core/IO/FileSystem.h"
#include "Core/IO/FileStream.h"

namespace SonataEngine
{

class FileInternal;

#if defined(WIN32)
	typedef void* FileHandle;
#elif defined(LINUX)
	typedef unsigned long FileHandle;
#elif defined(__APPLE__)
	typedef void* FileHandle;
#elif defined(XBOX)
	typedef void* FileHandle;
#elif defined(XENON)
	typedef void* FileHandle;
#else
	typedef void* FileHandle;
#endif

/**
	@class File.
	@brief Represents a file in a file system.
*/
class SE_CORE_EXPORT File
{
public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	File();

	/**
		Creates a new file.
		@param name
			The path of the file.
	*/
	File(const String& name);

	/** Destructor. */
	virtual ~File();
	//@}

	/**
		Retrieves the name of the file.
		@return The name of the file.
	*/
	const String& GetName() const;

	/**
		Sets the name of the file.
		@param name
			The name of the file.
	*/
	void SetName(const String& name);

	/** Gets the file access value. */
	FileAccess GetFileAccess() const;

	/** Gets the file sharing value. */
	FileShare GetFileShare() const;

	/** Gets the file mode value. */
	FileMode GetFileMode() const;

	/** @name Platform specific. */
	//@{
	/**
		Gets the handle to a file.
		@return The handle to a file.
	*/
	FileHandle GetHandle() const;

	/**
		Gets the length of a file.
		@return The length of a file.
	*/
	int32 GetLength() const;

	/**
		Gets whether a file is opened.
		@return true if a file is opened; otherwise, false.
	*/
	bool IsOpen() const;

	/**
		Returns whether the file exists.
		@return true if the file exists; otherwise, false.
	*/
	bool Exists() const;

	/**
		Returns whether the file exists.
		@param fileName The name of the file to check.
		@return true if the file exists; otherwise, false.
	*/
	static bool Exists(const String& fileName);

	/**
		Deletes a file.
		@return true if successful; otherwise, false.
	*/
	bool Delete();

	/**
		Deletes a file.
		@param fileName The name of the file to delete.
		@return true if successful; otherwise, false.
	*/
	static bool Delete(const String& fileName);

	/**
		Copies an existing file to a new file.
		@param fileName The name of the destination file. 
		@param overwrite true to allow an existing file to be overwritten; otherwise, false. 
		@return true if successful; otherwise, false.
	*/
	bool Copy(const String& fileName, bool overwrite = false);

	/**
		Copies an existing file to a new file.
		@param source The name of the source file. 
		@param destination The name of the destination file. 
		@param overwrite true to allow an existing file to be overwritten; otherwise, false. 
		@return true if successful; otherwise, false.
	*/
	static bool Copy(const String& source, const String& destination, bool overwrite = false);

	/**
		Moves a file to a new location.
		@param fileName The name of the new location. 
		@return true if successful; otherwise, false.
	*/
	bool Move(const String& fileName);

	/**
		Moves a file to a new location.
		@param source The name of the source file. 
		@param destination The name of the destination file. 
		@return true if successful; otherwise, false.
	*/
	static bool Move(const String& source, const String& destination);

	/**
		Creates a new file.
		@return A new file stream.
	*/
	FileStreamPtr Create();

	/**
		Opens a file.
		@param FileMode The mode in which to open the file.
		@param FileAccess The file access.
		@param FileShare The type of access.
		@return A file stream if successful, NULL otherwise.
	*/
	FileStreamPtr Open(FileMode fileMode, FileAccess fileAccess = FileAccess_ReadWrite, FileShare fileShare = FileShare_None);

	/**
		Closes a file.
		@return true if successful; otherwise, false.
	*/
	bool Close();
	//@}

private:
	FileInternal* _internal;
};

#include "File.inl"

}

#endif 
