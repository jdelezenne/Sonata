/*=============================================================================
FileSystem.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_FILESYSTEM_H_
#define _SE_FILESYSTEM_H_

#include "Core/Common.h"
#include "Core/Singleton.h"
#include "Core/String.h"
#include "Core/Containers/Array.h"

namespace SonataEngine
{

class Directory;

/** File Access. */
enum FileAccess
{
	/** Read access to the file. */
	FileAccess_Read,

	/** Write access to the file. */
	FileAccess_Write,

	/** Read and write access to the file. */
	FileAccess_ReadWrite
};

/** File Share. */
enum FileShare
{
	/** Declines sharing of the current file. */
	FileShare_None,

	/** Allows subsequent opening of the file for reading. */
	FileShare_Read,

	/** Allows subsequent opening of the file for writing. */
	FileShare_Write,

	/** Allows subsequent opening of the file for reading or writing. */
	FileShare_ReadWrite
};

/** File Mode. */
enum FileMode
{
	/** Opens the file if it exists and seeks to the end of the file,
	or creates a new file. */
	FileMode_Append,

	/** Specifies that the operating system should create a new file.
	If the file already exists, it will be overwritten. */
	FileMode_Create,

	/** Specifies that the operating system should create a new file. */
	FileMode_CreateNew,

	/** Specifies that the operating system should open an existing file. */
	FileMode_Open,

	/** Specifies that the operating system should open a file if it exists;
	otherwise, a new file should be created. */
	FileMode_OpenOrCreate,

	/** Specifies that the operating system should open an existing file.
	Once opened, the file should be truncated so that its size is zero bytes. */
	FileMode_Truncate
};

/**
	@class FileSystem.
	@brief Provides file system operations.
	An ordered list of root paths can be added to the file system and the file
	system objects will try to locate their resources using them.
*/
class SE_CORE_EXPORT FileSystem : public Singleton<FileSystem>
{
public:
	typedef Array<String> RootPathList;

protected:
	RootPathList _rootPaths;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	FileSystem();

	/** Destructor. */
	virtual ~FileSystem();
	//@}

	/** @name Root paths. */
	//@{
	int GetRootPathCount() const;
	RootPathList::Iterator GetRootPathIterator() const;
	bool AddRootPath(const String& value);
	bool InsertRootPath(int index, const String& value);
	bool ReplaceRootPath(const String& previous, const String& value);
	bool RemoveRootPath(const String& value);
	void RemoveAllRootPathes();
	int GetRootPathIndex(const String& value) const;
	String GetRootPath(int index) const;
	bool SetRootPath(int index, const String& value);
	//@}

	/** Returns a full path from the specified path using the mounted root paths. */
	String GetFullPath(const String& path, bool check = true);
};

}

#endif 
