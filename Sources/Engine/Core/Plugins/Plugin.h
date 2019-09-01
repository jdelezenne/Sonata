/*=============================================================================
Plugin.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_PLUGIN_H_
#define _SE_PLUGIN_H_

#include "Core/Common.h"
#include "Core/SE_ID.h"

namespace SonataEngine
{

struct PluginDescription;

/** Base class for plugins. */
class SE_CORE_EXPORT Plugin
{
public:
	/** Destructor. */
	virtual ~Plugin();

	/** @name Properties. */
	//@{
	/** Returns whether the plugin is enabled. */
	bool GetEnabled() const { return _Enabled; }

	/** Emables or disables the plugin. */
	void SetEnabled(bool value) { _Enabled = value; }

	/** Returns the type of the plugin. */
	virtual SE_ID GetPluginType() const = 0;

	/** Returns the ID of the plugin. */
	SE_ID GetPluginID() const;
	//@}

	/** @name Operations. */
	//@{
	/**
		Returns the description of the plugin.
		@return The description of the plugin.
		@see PluginDescription
	*/
	virtual PluginDescription* GetPluginDescription() const = 0;

	/** Loads the plugin. */
	virtual void Load() = 0;

	/** Unloads the plugin. */
	virtual void Unload() = 0;
	//@}

protected:
	/** Constructor. */
	Plugin();

protected:
	bool _Enabled;
};

}

#endif 
