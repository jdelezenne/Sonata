/*=============================================================================
PluginManager.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "PluginManager.h"
#include "Core/System/Environment.h"
#include "Core/System/Library.h"
#include "Core/IO/Path.h"
#include "Core/IO/Directory.h"
#include "Core/IO/File.h"
#include "Core/System/Environment.h"
#include "Core/Logging/Logger.h"
#include "Core/Exception/ArgumentException.h"
#include "Core/Exception/ArgumentNullException.h"
#include "Core/Exception/ArgumentOutOfRangeException.h"

namespace SonataEngine
{

PluginManager::PluginManager()
{
}

PluginManager::~PluginManager()	
{
	PluginLibraryList::Iterator itLib = _Libraries.GetIterator();
	while (itLib.Next())
	{
		delete itLib.Current();
	}

	PluginList::Iterator itPlg = _Plugins.GetIterator();
	while (itPlg.Next())
	{
		delete itPlg.Current();
	}
}

int PluginManager::ParsePlugins(const String& path)
{
	if (path.IsEmpty())
	{
		SEthrow(ArgumentException("path", "The specified path was empty."));
		return 0;
	}

	Directory directory(Path::Combine(path, _T("*.") + Environment::GetLibraryExtension()));

	Array<String> files = directory.GetFiles();

	int count = 0;

	Array<String>::Iterator it = files.GetIterator();
	while (it.Next())
	{
		String file = it.Current();
		String fullPath = Path::Combine(path, file);
		if (RegisterPluginLibrary(fullPath))
		{
			PluginLibraryDescription* pluginLibrary = GetPluginLibraryByName(fullPath);
			if (pluginLibrary != NULL)
			{
				count += pluginLibrary->_PluginCount;
			}
		}
	}

	return count;
}

bool PluginManager::RegisterPluginLibrary(const String& fileName)
{
	if (fileName.IsEmpty())
	{
		SEthrow(ArgumentException("fileName"));
		return false;
	}

	Library library(fileName);

	// The library is not a plugin or is invalid.
	if (!library.Load())
	{
		return false;
	}

	// Get the exported functions
	PluginSymbols symbols;
	symbols.GetPluginCount = (pfnGetPluginCount)library.GetSymbol("GetPluginCount");
	symbols.GetPluginDescription = (pfnGetPluginDescription)library.GetSymbol("GetPluginDescription");
	symbols.CreatePlugin = (pfnCreatePlugin)library.GetSymbol("CreatePlugin");
	symbols.DestroyPlugin = (pfnDestroyPlugin)library.GetSymbol("DestroyPlugin");

	int pluginCount;
	if (symbols.GetPluginCount == NULL)
	{
		pluginCount = 1;
	}
	else
	{
		pluginCount = (symbols.GetPluginCount)();
	}

	if (symbols.GetPluginDescription == NULL ||
		symbols.CreatePlugin == NULL ||
		symbols.CreatePlugin == NULL)
	{
		// The plugin is invalid, it has no exported function.
		return false;
	}

	PluginLibraryDescription* pluginLibrary = new PluginLibraryDescription();
	pluginLibrary->_Symbols = symbols;
	pluginLibrary->_fileName = fileName;
	pluginLibrary->_PluginCount = pluginCount;

	_Libraries.Add(pluginLibrary);

	for (int i = 0; i < pluginCount; i++)
	{
		PluginEntry* pluginEntry = new PluginEntry();
		PluginDescription* pluginDescription = (symbols.GetPluginDescription)(i);
		pluginEntry->_PluginDescription = pluginDescription;
		pluginEntry->_PluginLibrary = pluginLibrary;
		pluginEntry->_Index = i;
		AddPlugin(pluginEntry);
	}

	//library.Unload();

	Logger::Current()->Log(LogLevel::Information, _T("PluginManager.RegisterPluginLibrary"),
		_T("Added plugin library: ") + fileName);
	return true;
}

bool PluginManager::RegisterPluginLibrary(PluginSymbols symbols)
{
	if (symbols.GetPluginDescription == NULL || symbols.CreatePlugin == NULL || symbols.DestroyPlugin == NULL)
	{
		SEthrow(ArgumentNullException("symbols"));
		return false;
	}

	int pluginCount;
	if (symbols.GetPluginCount == NULL)
	{
		pluginCount = 1;
	}
	else
	{
		pluginCount = (symbols.GetPluginCount)();
	}

	PluginLibraryDescription* pluginLibrary = new PluginLibraryDescription();
	pluginLibrary->_Symbols = symbols;
	pluginLibrary->_PluginCount = pluginCount;

	_Libraries.Add(pluginLibrary);

	PluginEntry* pluginEntry;
	PluginDescription* pluginDescription;
	for (int i = 0; i < pluginCount; i++)
	{
		pluginEntry = new PluginEntry();
		pluginDescription = (symbols.GetPluginDescription)(i);
		pluginEntry->_PluginDescription = pluginDescription;
		pluginEntry->_PluginLibrary = pluginLibrary;
		pluginEntry->_Index = i;
		AddPlugin(pluginEntry);
	}

	return true;
}

bool PluginManager::LoadPluginLibrary(int index)
{
	return true;
}

bool PluginManager::UnloadPluginLibrary(int index)
{
	return true;
}

int PluginManager::GetPluginLibraryCount() const
{
	return _Libraries.Count();
}

PluginLibraryDescription* PluginManager::GetPluginLibraryByIndex(int index) const
{
	return _Libraries[index];
}

PluginLibraryDescription* PluginManager::GetPluginLibraryByName(const String& name) const
{
	PluginLibraryList::Iterator it = _Libraries.GetIterator();
	while (it.Next())
	{
		if (String::Compare(it.Current()->_fileName, name, true) == 0)
			return it.Current();
	}

	return NULL;
}

PluginManager::PluginLibraryList::Iterator PluginManager::GetPluginLibraryIterator() const
{
	return _Libraries.GetIterator();
}

Plugin* PluginManager::CreatePlugin(const SE_ID& type)
{
	PluginEntry* entry;
	PluginEntryList::Iterator it = _PluginEntries.GetIterator();
	while (it.Next())
	{
		entry = it.Current();
		if (entry->_PluginDescription->_Type == type)
		{
			return entry->_PluginLibrary->_Symbols.CreatePlugin(entry->_Index);
		}
	}

	return NULL;
}

void PluginManager::DestroyPlugin(Plugin* plugin)
{
}

void PluginManager::CreateAllPlugins()
{
	DestroyAllPlugins();

	PluginEntry* entry;
	Plugin* plugin;
	PluginEntryList::Iterator it = _PluginEntries.GetIterator();
	while (it.Next())
	{
		entry = it.Current();

		Library library(entry->_PluginLibrary->_fileName);
		if (library.Load())
		{
			plugin = entry->_PluginLibrary->_Symbols.CreatePlugin(entry->_Index);
			if (plugin != NULL)
			{
				_Plugins.Add(plugin);
			}
			library.Unload();
		}
	}
}

void PluginManager::DestroyAllPlugins()
{
	Plugin* plugin;
	PluginDescription* desc;
	PluginList::Iterator it = _Plugins.GetIterator();
	while (it.Next())
	{
		plugin = it.Current();
		desc = plugin->GetPluginDescription();

		Library library(desc->_PluginLibrary->_fileName);
		if (library.Load())
		{
			desc->_PluginLibrary->_Symbols.DestroyPlugin(plugin);
			library.Unload();
		}
	}

	_Plugins.Clear();
}

int PluginManager::GetPluginEntryCount() const
{
	return _PluginEntries.Count();
}

PluginDescription* PluginManager::GetPluginEntryByIndex(int index) const
{
	return _PluginEntries[index]->_PluginDescription;
}

PluginDescription* PluginManager::GetPluginEntryByID(const SE_ID& type)
{
	PluginEntryList::Iterator it = _PluginEntries.GetIterator();
	while (it.Next())
	{
		if (it.Current()->_PluginDescription->_Type == type)
		{
			return it.Current()->_PluginDescription;
		}
	}

	return NULL;
}

PluginManager::PluginEntryList::Iterator PluginManager::GetPluginEntryIterator() const
{
	return _PluginEntries.GetIterator();
}

int PluginManager::GetPluginCount() const
{
	return _Plugins.Count();
}

Plugin* PluginManager::GetPluginByIndex(int index) const
{
	return _Plugins[index];
}

Plugin* PluginManager::GetPluginByID(const SE_ID& type)
{
	PluginList::Iterator it = _Plugins.GetIterator();
	while (it.Next())
	{
		Plugin* plugin = it.Current();
		{
			if (plugin->GetPluginDescription()->_Type == type)
			{
				return plugin;
			}
		}
	}

	return NULL;
}

PluginManager::PluginList::Iterator PluginManager::GetPluginIterator() const
{
	return _Plugins.GetIterator();
}

int PluginManager::AddPlugin(PluginEntry* value)
{
	_PluginEntries.Add(value);

	return _PluginEntries.Count();
}

void PluginManager::RemovePlugin(int index)
{
	_PluginEntries.RemoveAt(index);
}

}
