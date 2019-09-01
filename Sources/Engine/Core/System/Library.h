/*=============================================================================
Library.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_LIBRARY_H_
#define _SE_LIBRARY_H_

#include "Core/Common.h"
#include "Core/String.h"

namespace SonataEngine
{

class LibraryInternal;

/**
	@class Library.
	@brief Dynamic library.
*/
class SE_CORE_EXPORT Library
{
public:
	/** @name Constructor / Destructor */
	//@{
	/** Constructor.
		@param name The path of the library.
	*/
	Library(const String& name);

	/** Destructor. */
	virtual ~Library();
	//@}

	/** Returns the library name.
		@return The library name.
	*/
	String GetName() const;

	/** Loads the library.
		@return true if successful; otherwise, false.
	*/
	bool Load();

	/** Unloads the library.
		@return true if successful; otherwise, false.
	*/
	bool Unload();

	/** Returns whether the library is loaded.
		@return true if the library is loaded; otherwise, false.
	*/
	bool IsLoaded();

	/** Gets a symbol handle from the library.
		@param name The name of the symbol.
		@return A handle to the symbol if successful, NULL otherwise.
	*/
	void* GetSymbol(const String& name);

	/** Gets a symbol handle from a library.
		@param library The path of the library.
		@param name The name of the symbol.
		@return A handle to the symbol if successful, NULL otherwise.
	*/
	static void* GetSymbol(const String& library, const String& name);

private:
	LibraryInternal* _internal;
};

}

#endif 
