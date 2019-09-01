/*=============================================================================
DS8AudioSystemLib.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "DS8AudioSystemLib.h"
#include "DS8AudioSystem.h"

#include <Core/Plugins/PluginManager.h>
#include <Core/Plugins/ManagerPlugin.h>

using namespace SonataEngine;

namespace SE_DS8
{

#ifdef SE_STATIC
	#define SE_EXPORT
#else
	#define SE_EXPORT __declspec(dllexport)
#endif

PluginDescription g_PluginDescription[1]=
{
	PluginDescription(
		SE_ID_AUDIOSYSTEM_DS8,
		1,
		"DirectSound8 Audio System",
		"Julien Delezenne")
};

class DS8AudioSystemPlugin : public ManagerPlugin
{
public:
	DS8AudioSystemPlugin::DS8AudioSystemPlugin() :
		ManagerPlugin()
	{
	}

	virtual DS8AudioSystemPlugin::~DS8AudioSystemPlugin()
	{
	}

	virtual PluginDescription* DS8AudioSystemPlugin::GetPluginDescription() const
	{
		return &g_PluginDescription[0];
	}

	virtual void DS8AudioSystemPlugin::Load()
	{
	}

	virtual void DS8AudioSystemPlugin::Unload()
	{
	}

	virtual SonataEngine::SE_ID DS8AudioSystemPlugin::GetManagerType() const
	{
		return SE_ID_AUDIOSYSTEM;
	}

	virtual Manager* DS8AudioSystemPlugin::CreateManager()
	{
		return new DS8AudioSystem();
	}

	virtual void DS8AudioSystemPlugin::DestroyManager(Manager* manager)
	{
		SE_DELETE(manager);
	}
};

#ifndef SE_STATIC
extern "C"
{
#endif

SE_EXPORT PluginDescription* GetPluginDescription(int index)
{
	return &g_PluginDescription[index];
}

SE_EXPORT Plugin* CreatePlugin(int index)
{
	switch (index)
	{
		case 0: return new DS8AudioSystemPlugin();
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
	symbols.GetPluginDescription = GetPluginDescription;
	symbols.CreatePlugin = CreatePlugin;
	symbols.DestroyPlugin = DestroyPlugin;
	PluginManager::Instance()->RegisterPluginLibrary(symbols);
}
#endif

}
