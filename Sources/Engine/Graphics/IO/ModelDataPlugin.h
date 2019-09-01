/*=============================================================================
ModelDataPlugin.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_MODELDATAPLUGIN_H_
#define _SE_MODELDATAPLUGIN_H_

#include "Core/Plugins/DataPlugin.h"
#include "Graphics/Common.h"

namespace SonataEngine
{

#define SE_ID_DATA_MODEL SonataEngine::SE_ID(0x954dc3ea,0x43d2baf1)

class ModelReader;
class ModelWriter;

/**
	@brief Base class for model plugins.

	This class must be implemented to read/write models.
*/
class SE_GRAPHICS_EXPORT ModelDataPlugin : public DataPlugin
{
public:
	virtual SE_ID GetDataType() const { return SE_ID_DATA_MODEL; }

	/** @name Operations. */
	//@{
	/**
		Creates a model reader for a given stream.
		@return A model reader if successful, NULL otherwise.
		@see DestroyReader
	*/
	virtual ModelReader* CreateReader() = 0;

	/**
		Creates a model writer for a given stream.
		@return A model writer if successful, NULL otherwise.
		@see DestroyWriter
	*/
	virtual ModelWriter* CreateWriter() = 0;

	/**
		Destroys a model reader.
		@param reader A model reader to destroy.
		@see CreateReader
	*/
	virtual void DestroyReader(ModelReader* reader) = 0;

	/**
		Creates a model writer for a given stream.
		@writer A model writer to destroy.
		@see CreateWriter
	*/
	virtual void DestroyWriter(ModelWriter* writer) = 0;
	//@}
};

}

#endif 
