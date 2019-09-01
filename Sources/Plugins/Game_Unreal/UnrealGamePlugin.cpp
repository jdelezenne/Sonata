/*=============================================================================
UnrealGamePlugin.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "UnrealGamePlugin.h"
#include "UnrealPSKReader.h"

#include <Core/Plugins/PluginManager.h>
#include <Core/Plugins/ManagerPlugin.h>

using namespace SonataEngine;

namespace SE_Unreal
{

#ifdef SE_STATIC
	#define SE_EXPORT
#else
	#define SE_EXPORT __declspec(dllexport)
#endif

PluginDescription g_PluginDescription[1] =
{
	PluginDescription(
		SE_ID_DATAMODEL_PSK,
		1,
		"Unreal PSK Models",
		"Julien Delezenne")
};

class UnrealPSKPlugin : public ModelDataPlugin
{
private:
	Array<String> _extensions;

public:
	UnrealPSKPlugin::UnrealPSKPlugin() :
		ModelDataPlugin()
	{
		_extensions.Add("psk");
	}

	virtual UnrealPSKPlugin::~UnrealPSKPlugin()
	{
	}

	virtual PluginDescription* UnrealPSKPlugin::GetPluginDescription() const
	{
		return &g_PluginDescription[0];
	}

	virtual void UnrealPSKPlugin::Load()
	{
	}

	virtual void UnrealPSKPlugin::Unload()
	{
	}

	virtual Array<String> UnrealPSKPlugin::GetExtensions() const
	{
		return _extensions;
	}

	virtual bool UnrealPSKPlugin::CanRead() const
	{
		return true;
	}

	virtual bool UnrealPSKPlugin::CanWrite() const
	{
		return false;
	}

	virtual bool UnrealPSKPlugin::CanHandle(const Stream& stream) const
	{
		return true;
	}

	virtual ModelReader* UnrealPSKPlugin::CreateReader()
	{
		return new UnrealPSKReader();
	}

	virtual ModelWriter* UnrealPSKPlugin::CreateWriter()
	{
		return NULL;
	}

	virtual void UnrealPSKPlugin::DestroyReader(ModelReader* reader)
	{
		if (reader != NULL)
			delete reader;
	}

	virtual void UnrealPSKPlugin::DestroyWriter(ModelWriter* writer)
	{
		if (writer != NULL)
			delete writer;
	}
};

#ifndef SE_STATIC
extern "C"
{
#endif

SE_EXPORT int GetPluginCount()
{
	return 1;
}

SE_EXPORT PluginDescription* GetPluginDescription(int index)
{
	return &g_PluginDescription[index];
}

SE_EXPORT Plugin* CreatePlugin(int index)
{
	switch (index)
	{
		case 0: return new UnrealPSKPlugin();
	}
	return NULL;
}

SE_EXPORT void DestroyPlugin(Plugin* plugin)
{
	SE_DELETE(plugin);
}

#ifndef SE_STATIC
}
#endif

#ifdef SE_STATIC
PluginModule::PluginModule()
{
	PluginSymbols symbols;
	symbols.GetPluginCount = GetPluginCount;
	symbols.GetPluginDescription = GetPluginDescription;
	symbols.CreatePlugin = CreatePlugin;
	symbols.DestroyPlugin = DestroyPlugin;
	PluginManager::Instance()->RegisterPluginLibrary(symbols);
}
#endif

}
