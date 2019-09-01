/*=============================================================================
PluginManager.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_PLUGINMANAGER_H_
#define _SE_PLUGINMANAGER_H_

#include "Core/Common.h"
#include "Core/Singleton.h"
#include "Core/String.h"
#include "Core/SE_ID.h"
#include "Core/Containers/Array.h"
#include "Core/Plugins/Plugin.h"

namespace SonataEngine
{

#define SE_ID_SYSTEM SonataEngine::SE_ID(0x3d55128c,0x32220f0d)

/** Plugin functions. */
typedef int (*pfnGetPluginCount) ();
typedef PluginDescription* (*pfnGetPluginDescription) (int index);
typedef Plugin* (*pfnCreatePlugin) (int index);
typedef void (*pfnDestroyPlugin) (Plugin* plugin);

/** Plugin symbols. */
struct PluginSymbols
{
	PluginSymbols() :
		GetPluginCount(NULL),
		GetPluginDescription(NULL),
		CreatePlugin(NULL),
		DestroyPlugin(NULL)
	{
	}

	pfnGetPluginCount GetPluginCount;
	pfnGetPluginDescription GetPluginDescription;
	pfnCreatePlugin CreatePlugin;
	pfnDestroyPlugin DestroyPlugin;
};

/** Plugin library description. */
struct PluginLibraryDescription
{
	PluginLibraryDescription()
	{ 
		_PluginCount = 0;
	}

	String _fileName; // Library path.
	uint32 _PluginCount; // Number of plugins contained in this library.
	PluginSymbols _Symbols; // Library symbols.
};

/** Plugin description. */
struct PluginDescription
{
	PluginDescription()
	{
	}

	PluginDescription(SE_ID type, uint32 version, const String& description, const String& author) :
		_Type(type), _Version(version), _Description(description), _Author(author)
	{
	}

	PluginLibraryDescription* _PluginLibrary;
	int32 _Index;
	SE_ID _Type; // Type
	uint32 _Version; // Version
	String _Description; // Description
	String _Author; // Author name
};

/// Plugin entry.
struct PluginEntry
{
	PluginEntry()
	{
	}

	PluginDescription* _PluginDescription;
	PluginLibraryDescription* _PluginLibrary;
	int32 _Index;
};

/**
	@brief Plugin Manager.

	This class parses the plugins libraries and registers them.
	After being initialized, the plugins can be used.
*/
class SE_CORE_EXPORT PluginManager : public Singleton<PluginManager>
{
public:
	typedef Array<PluginLibraryDescription*> PluginLibraryList;
	typedef Array<PluginEntry*> PluginEntryList;
	typedef Array<Plugin*> PluginList;

	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	PluginManager();

	/** Destructor. */
	virtual ~PluginManager();
	//@}

	/** Parse a directory for plugins and returns the number of valid plugins registered.
		@param path Path to parse for plugins.
		@return Number of plugins registered.
	*/
	int32 ParsePlugins(const String& path);

	/** Registers a plugin library.
		@param fileName FileName of the plugin library to register.
		@return true if successful; otherwise, false.
	*/
	bool RegisterPluginLibrary(const String& fileName);

	/** Registers a static plugin library.
		@param symbols Plugin symbols.
		@return true if successful; otherwise, false.
	*/
	bool RegisterPluginLibrary(PluginSymbols symbols);

	/** Libraries. */
	//@{
	/// Loads a given library.
	bool LoadPluginLibrary(int index);

	/// Unloads a given library.
	bool UnloadPluginLibrary(int index);

	/** Returns the number of registered libraries.
		@return The number of registered libraries.
	*/
	int GetPluginLibraryCount() const;

	/** Returns the plugin library at a specific index.
		@param index The index of the plugin library to find.
		@return A pointer to the plugin library if successful, NULL otherwise.
	*/
	PluginLibraryDescription* GetPluginLibraryByIndex(int index) const;

	/** Returns the plugin library by name.
		@param name The name of the plugin library to find.
		@return A pointer to the plugin library if successful, NULL otherwise.
	*/
	PluginLibraryDescription* GetPluginLibraryByName(const String& name) const;

	PluginLibraryList::Iterator GetPluginLibraryIterator() const;
	//@}

	/** Plugins. */
	//@{
	/** Creates a plugin from its SE_ID.
		@param type The SE_ID of the plugins to create.
		@return A pointer to the plugin if successful, NULL otherwise.
	*/
	Plugin* CreatePlugin(const SE_ID& type);

	/** Creates a plugin from its SE_ID.
		@param plugin The plugin to destroy.
	*/
	void DestroyPlugin(Plugin* plugin);

	void CreateAllPlugins();
	void DestroyAllPlugins();

	int GetPluginEntryCount() const;

	PluginDescription* GetPluginEntryByIndex(int index) const;

	/** Search through every plugins for one with the specified SE_ID.
		@param type The SE_ID of the plugins to find.
		@return The plugin description if successful, NULL otherwise.
	*/
	PluginDescription* GetPluginEntryByID(const SE_ID& type);

	PluginEntryList::Iterator GetPluginEntryIterator() const;

	int GetPluginCount() const;

	Plugin* GetPluginByIndex(int index) const;

	Plugin* GetPluginByID(const SE_ID& type);

	PluginList::Iterator GetPluginIterator() const;
	//@}

protected:
	/// Internal. Adds a plugin.
	int AddPlugin(PluginEntry* value);

	/// Internal. Removes a plugin.
	void RemovePlugin(int index);

	PluginLibraryList _Libraries;
	PluginEntryList _PluginEntries;
	PluginList _Plugins;
};

}

#endif 
