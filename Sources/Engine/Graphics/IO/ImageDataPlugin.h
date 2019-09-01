/*=============================================================================
ImageDataPlugin.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_IMAGEDATAPLUGIN_H_
#define _SE_IMAGEDATAPLUGIN_H_

#include "Core/Plugins/DataPlugin.h"
#include "Graphics/Common.h"

namespace SonataEngine
{

#define SE_ID_DATA_IMAGE SonataEngine::SE_ID(0x3a137f07,0xe9c0471d)

class ImageReader;
class ImageWriter;

/**
	@brief Base class for image plugins.

	This class must be implemented to read/write images.
*/
class SE_GRAPHICS_EXPORT ImageDataPlugin : public DataPlugin
{
public:
	virtual SE_ID GetDataType() const { return SE_ID_DATA_IMAGE; }

	/** @name Operations. */
	//@{
	/**
		Creates an image reader for a given stream.
		@return An image reader if successful, NULL otherwise.
		@see CreateReader
	*/
	virtual ImageReader* CreateReader() = 0;

	/**
		Creates an image writer for a given stream.
		@return An image writer if successful, NULL otherwise.
		@see CreateWriter
	*/
	virtual ImageWriter* CreateWriter() = 0;

	/**
		Destroys an image reader.
		@param reader An image reader to destroy.
		@see CreateReader
	*/
	virtual void DestroyReader(ImageReader* reader) = 0;

	/**
		Creates an image writer for a given stream.
		@writer An image writer to destroy.
		@see CreateWriter
	*/
	virtual void DestroyWriter(ImageWriter* writer) = 0;
	//@}
};

}

#endif 
