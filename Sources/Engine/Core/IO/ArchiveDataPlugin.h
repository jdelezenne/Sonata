/*=============================================================================
ArchiveDataPlugin.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ARCHIVEDATAPLUGIN_H_
#define _SE_ARCHIVEDATAPLUGIN_H_

#include "Core/Common.h"
#include "Core/Plugins/DataPlugin.h"

namespace SonataEngine
{

#define SE_ID_DATA_ARCHIVE SonataEngine::SE_ID(0xa6a1b8ac,0xd4e2447d)

/**
	@brief Base class for archive plugins.

	This class must be implemented to read/write archives.
*/
class SE_CORE_EXPORT ArchiveDataPlugin : public DataPlugin
{
public:
	virtual SE_ID GetDataType() const { return SE_ID_DATA_ARCHIVE; }

	/** @name Operations. */
	//@{
	/**
		Creates an archive.
		@return An archive if successful, NULL otherwise.
		@see DestroyArchive
	*/
	virtual Archive* CreateArchive() = 0;

	/**
		Destroys an archive.
		@param archive An archive to destroy.
		@see CreateArchive
	*/
	virtual void DestroyArchive(Archive* archive) = 0;
	//@}
};

}

#endif 
