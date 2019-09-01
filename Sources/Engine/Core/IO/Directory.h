/*=============================================================================
Directory.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_DIRECTORY_H_
#define _SE_DIRECTORY_H_

#include "Core/Common.h"
#include "Core/IO/FileSystem.h"
#include "Core/Containers/Array.h"

namespace SonataEngine
{

class DirectoryInternal;

/**
	@class Directory.
	@brief Represents a directory in a file system.
*/
class SE_CORE_EXPORT Directory : public FileSystem
{
public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	Directory();

	/**
		Creates a new directory.
		@param name The path of the directory.
	*/
	Directory(const String& name);

	/** Destructor. */
	virtual ~Directory();
	//@}

	/**
		Retrieves the name of the directory.
		@return Name of the directory.
	*/
	const String& GetName() const;

	/**
		Sets the name of the directory.
		@param name Name of the directory.
	*/
	void SetName(const String& name);

	/** @name Platform specific. */
	//@{
	/**
		Gets the parent directory.
		@return The parent directory if successful, NULL otherwise.
	*/
	Directory GetParent() const;

	/**
		Returns whether the directory exists.
		@return true if the directory exists; otherwise, false.
	*/
	bool Exists() const;

	/**
		Returns whether the directory exists.
		@param path The path of the directory to check.
		@return true if the directory exists; otherwise, false.
	*/
	static bool Exists(const String& path);

	/**
		Creates a directory.
		@return true if successful; otherwise, false.
		@remarks If the directory already exists, this method does nothing.
	*/
	bool Create();

	/**
		Creates a directory.
		@param path The name of the directory to delete.
		@remarks If the directory already exists, this method does nothing.
	*/
	static bool Create(const String& path);

	/**
		Creates a subdirectory on the specified path.
		@param name Name of the subdirectory to create.
		@return true if successful; otherwise, false.
	*/
	bool CreateSubdirectory(const String& name);

	/**
		Deletes a directory and its content.
		@return true if successful; otherwise, false.
	*/
	bool Delete();

	/**
		Deletes a directory and its content.
		@param path The name of the directory to delete.
		@return true if successful; otherwise, false.
	*/
	static bool Delete(const String& path);

	/**
		Moves a directory and its content to a new path.
		@param path The path to which to move this directory.
		@return true if successful; otherwise, false.
	*/
	bool Move(const String& path);

	/**
		Moves a directory and its content to a new path.
		@param source The name of the source directory. 
		@param destination The name of the destination directory. 
		@return true if successful; otherwise, false.
	*/
	static bool Move(const String& source, const String& destination);

	/**
		Returns the names of the directories in the directory.
		@return The list of directory names.
	*/
	const Array<String> GetDirectories() const;

	/**
		Returns the names of the directories in the specified directory.
		@return The list of directory names.
	*/
	static const Array<String> GetDirectories(const String& path);

	/**
		Returns the names of the files in the directory.
		@return The list of file names.
	*/
	const Array<String> GetFiles() const;

	/**
		Returns the names of the files in the specified directory.
		@return The list of file names.
	*/
	static const Array<String> GetFiles(const String& path);
	//@}

private:
	DirectoryInternal* _internal;
};

#include "Directory.inl"

}

#endif 
