/*=============================================================================
Path.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_PATH_H_
#define _SE_PATH_H_

#include "Core/Common.h"
#include "Core/String.h"

namespace SonataEngine
{

/**
	@class Path.
	@brief Provides operations on String that represents a file name or directory path.
*/
class SE_CORE_EXPORT Path
{
public:
	/** Converts the '/' separators in path to system native separators. */
	static String ConvertSeparators(const String& path, bool toSlash);

	/** Returns the file name and extension of the specified path string. */
	static String GetFileName(const String& path); 

	/** Returns the directory information for the specified path string. */
	static String GetDirectoryName(const String& path, bool absPath = false);

	/** Returns the absolute path for the specified path string. */
	static String GetFullPath(const String& path);

	/** Gets the root directory information of the specified path. */
	static String GetPathRoot(const String& path);

	/** Returns the extension of the specified path string. */
	static String GetExtension(const String& path, bool complete = true);

	/** Returns the file name of the specified path string without the extension. */
	static String GetFileNameWithoutExtension(const String& path, bool complete = true);

	/** Changes the extension of a path string. */
	static String ChangeExtension(const String& path, const String& ext, bool complete = true);

	/** Combines two path strings. */
	static String Combine(const String& left, const String& right);

	/** Determines whether a path includes a file name extension. */
	static bool HasExtension(const String& path);

	/** Determines whether a path is absolute . */
	static bool IsAbsolutePath(const String& path);

	/** Determines whether a path is relative. */
	static bool IsRelativePath(const String& path);
};

}

#endif 
