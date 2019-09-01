/*=============================================================================
Archive.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ARCHIVE_H_
#define _SE_ARCHIVE_H_

#include "Core/Common.h"
#include "Core/Containers/BaseArray.h"
#include "Core/System/DateTime.h"
#include "Core/IO/Directory.h"
#include "Core/IO/File.h"
#include "Core/IO/Path.h"

namespace SonataEngine
{

/** Archive entry. */
struct ArchiveEntry
{
	String Name;
	int Index;
	int Offset;
	int Size;
	int CompressedSize;
	DateTime CreationDate;
	DateTime ModificationDate;
	DateTime AccessDate;
	String Comment;
};


/**
	@class Archive.
	@brief Represents an archive file in a file system.
	An archive represents a directory in a virtual file system.
*/
class SE_CORE_EXPORT Archive
{
public:
	typedef BaseArray<ArchiveEntry> ArchiveEntryList;

protected:
	String _name;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	Archive();

	/** Destructor. */
	virtual ~Archive();
	//@}

	/** @name Creation / Destruction. */
	//@{
	/**
		Opens an archive file for reading and writing.
		@return true is successful; otherwise, false.
	*/
	virtual bool Open() = 0;

	/**
		Creates a new archive file.
		@return true is successful; otherwise, false.
		@remark The file is overwritten if already existing, a new file is created
		otherwise.
	*/
	virtual bool Create() = 0;

	/**
		Closes the archive file.
		@return true is successful; otherwise, false.
	*/
	virtual bool Close() = 0;
	//@}

	/** @name Properties. */
	//@{
	/**
		Retrieves the name of the archive.
		@return Name of the archive.
	*/
	const String& GetName() const { return _name; }

	/**
		Sets the name of the archive.
		@param name Name of the archive.
	*/
	void SetName(const String& name);

	/**
		Returns whether the archive is a directory.
		@return
			true if the archive is a directory, false otherwise.
		@remark
			If the archive is a directory, its entries are contained in
			the virtual folder given
			by the name of the archive without its extension.
	*/
	virtual bool IsDirectory() const = 0;

	/**
		Sets the archive as a directory.
		@param value to set the archive as a directory.
		@see IsDirectory
	*/
	virtual void SetDirectory(bool value) = 0;

	/**
		Gets the maximum number of entries that the archive can contain.
		@return
			The maximum number of entries that the archive can contain
			or 0 if the number of entries is not limited.
		@remark
			If the archive can't contain file entries, its entries are unnamed blocks of data.
	*/
	virtual int GetMaximumEntries() const { return 0; }

	/**
		Returns whether the archive can store directories.
		@return TRUE if the archive can store directories, FALSE otherwise.
		@remark If the archive can't store directories, its entries are in at the root.
	*/
	virtual bool CanStoreDirectories() const { return true; }

	/**
		Returns whether the archive can store file entries.
		@return TRUE if the archive can store file entries, FALSE otherwise.
		@remark If the archive can't store file entries, its entries are unnamed blocks of data.
	*/
	virtual bool CanStoreFiles() const { return true; }

	/**
		Returns whether the maximum length of the path of an entry.
		@return The maximum path length otherwise, or 0 if the path length is not limited.
	*/
	virtual int GetMaximumPathLength() const { return 0; }

	/**
		Gets the number of entries in the archive.
		@return Number of entries in the archive.
	*/
	virtual int GetEntryCount() const = 0;

	/**
		Gets the list of the name of entries contained in the archive.
		@return The list of the entries if successful.
	*/
	virtual bool GetEntryNames(Array<String>& entries) = 0;

	/**
		Gets the list of the entries contained in the archive.
		@return The list of the entries if successful.
	*/
	virtual bool GetEntries(ArchiveEntryList& entries) = 0;

	/**
		Gets an entry in the archive.
		@param name Name of the file to retrieve the entry information from.
		@param entry Entry retrieved if successful.
		@return true is successful; otherwise, false.
	*/
	virtual bool GetEntry(const String& name, ArchiveEntry& entry) = 0;

	/**
		Gets an entry in the archive.
		@param index Index of the entry to get.
		@param entry Entry retrieved if successful.
		@return true is successful; otherwise, false.
	*/
	virtual bool GetEntry(int index, ArchiveEntry& entry) = 0;

	/**
		Returns whether the archive contains the given directory.
		@param name Path of the directory to search.
		@return TRUE if the directory is found, FALSE otherwise.
	*/
	virtual bool ContainsDirectory(const String& name) = 0;

	/**
		Returns whether the archive contains the given file.
		@param name Name of the file to search.
		@return TRUE if the file is found, FALSE otherwise.
	*/
	virtual bool ContainsFile(const String& name) = 0;
	//@}


	/** @name Operations */
	//@{
	/**
		Opens a stream to a file.
		@param name Name of the file to read.
		@return The stream to the file; otherwise, NULL.
	*/
	virtual Stream* OpenFile(const String& name) = 0;

	/**
		Opens a stream to a file.
		@param index Index of the file to read.
		@return The stream to the file; otherwise, NULL.
	*/
	virtual Stream* OpenFile(int index) = 0;

	/**
		Adds a file to the archive.
		@param pFile The file to add.
		@param destination Name of the destination path in the archive (including destination filename).
		@return true is successful; otherwise, false.
	*/
	virtual bool AddFile(File* file, const String& destination) = 0;

	/**
		Adds a directory to the archive.
		@param pDirectory The directory to add.
		@param destination Name of the destination path in the archive (including destination filename).
		@param bRecursive TRUE to add all the child directories and files.
		@return true is successful; otherwise, false.
	*/
	virtual bool AddDirectory(Directory* directory, const String& destination, bool bRecursive) = 0;


	/**
		Removes a file from the archive.
		@param name Name of the file to remove.
		@return true is successful; otherwise, false.
	*/
	virtual bool RemoveFile(const String& name) = 0;

	/**
		Removes a file from the archive.
		@param index Index of the file to remove.
		@return true is successful; otherwise, false.
	*/
	virtual bool RemoveFile(int index) = 0;

	/**
		Removes a directory from the archive.
		@param name Name of the path to remove.
		@return true is successful; otherwise, false.
	*/
	virtual bool RemoveDirectory(const String& name) = 0;

	/**
		Performs all the pending operations.
		@return true is successful; otherwise, false.
	*/
	virtual bool Flush() = 0;

	/**
		Extracts a file to a given destination.
		@param name Name of the file to read.
		@param destination Name of the destination path (including destination file name).
		@return true is successful; otherwise, false.
	*/
	virtual bool ExtractFile(const String& name, const String& destination);

	/**
		Extracts a file to a given destination.
		@param index Index of the file to extract.
		@param destination Name of the destination path (including destination file name).
		@return true is successful; otherwise, false.
	*/
	virtual bool ExtractFile(int index, const String& destination);

	/**
		Extracts a directory to a given destination.
		@param name Path of the directory to extract.
		@param destination Name of the destination path.
		@param isRecursive true to extract all the child directories and files.
		@return true is successful; otherwise, false.
	*/
	virtual bool ExtractDirectory(const String& name, const String& destination, bool isRecursive);
	//@}
};

}

#endif 
