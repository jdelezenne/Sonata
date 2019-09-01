/*=============================================================================
SceneDataPlugin.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SCENEDATAPLUGIN_H_
#define _SE_SCENEDATAPLUGIN_H_

#include "Core/Plugins/DataPlugin.h"
#include "Graphics/Common.h"

namespace SonataEngine
{

#define SE_ID_DATA_SCENE SonataEngine::SE_ID(0x20c620ed,0x9cf64ab4)

class SceneReader;
class SceneWriter;

/**
	@brief Base class for scene plugins.

	This class must be implemented to read/write scenes.
*/
class SE_GRAPHICS_EXPORT SceneDataPlugin : public DataPlugin
{
public:
	virtual SE_ID GetDataType() const { return SE_ID_DATA_SCENE; }

	/** @name Operations. */
	//@{
	/**
		Creates a scene reader for a given stream.
		@return A scene reader if successful, NULL otherwise.
		@see DestroyReader
	*/
	virtual SceneReader* CreateReader() = 0;

	/**
		Creates a scene writer for a given stream.
		@return A scene writer if successful, NULL otherwise.
		@see DestroyWriter
	*/
	virtual SceneWriter* CreateWriter() = 0;

	/**
		Destroys a scene reader.
		@param reader A scene reader to destroy.
		@see CreateReader
	*/
	virtual void DestroyReader(SceneReader* reader) = 0;

	/**
		Creates a scene writer for a given stream.
		@writer A scene writer to destroy.
		@see CreateWriter
	*/
	virtual void DestroyWriter(SceneWriter* writer) = 0;
	//@}
};

}

#endif 
