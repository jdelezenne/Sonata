/*=============================================================================
DataPlugin.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_DATAPLUGIN_H_
#define _SE_DATAPLUGIN_H_

#include "Core/Plugins/Plugin.h"
#include "Core/Plugins/PluginManager.h"

namespace SonataEngine
{

#define SE_ID_DATA SE_ID(0x1b4ce2a5,0xda234dbd)

/**
	@brief Data plugin.

	Base class for data plugin implementations.
	This class is used to read/write specific file formats. It must be
	implemented in the plugins to allow more file formats to be used by the engine.
*/
class SE_CORE_EXPORT DataPlugin : public Plugin
{
public:
	/** Destructor. */
	virtual ~DataPlugin();

	virtual SE_ID GetPluginType() const { return SE_ID_DATA; }

	/** @name Operations. */
	//@{
	/**
		Returns the SE_ID of the supported data type.
		@return The SE_ID of the supported data type.
	*/
	virtual SE_ID GetDataType() const = 0;

	/**
		Returns an array of supported file extensions.
		@return An array of supported file extensions.
	*/
	virtual Array<String> GetExtensions() const = 0;

	/**
		Returns whether the plugin can read data.
		@return True if the plugin can read data, False otherwise.
		@see CanWrite
	*/
	virtual bool CanRead() const = 0;

	/**
		Returns whether the plugin can write data.
		@return True if the plugin can write data, False otherwise.
		@see CanRead
	*/
	virtual bool CanWrite() const = 0;

	/**
		Returns whether the plugin can handle the given stream.
		@return True if the plugin can handle the given stream, False otherwise.
	*/
	virtual bool CanHandle(const Stream& stream) const = 0;
	//@}

protected:
	/** Constructor. */
	DataPlugin();
};

}

#endif 
