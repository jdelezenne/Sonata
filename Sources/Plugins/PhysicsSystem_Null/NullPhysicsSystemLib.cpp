/*=============================================================================
NullPhysicsSystemLib.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "NullPhysicsSystemLib.h"
#include "NullPhysicsSystem.h"

#include <Core/Plugins/PluginManager.h>
#include <Core/Plugins/ManagerPlugin.h>

using namespace SonataEngine;

namespace SE_Null
{

#ifdef SE_STATIC
	#define SE_EXPORT
#else
	#define SE_EXPORT __declspec(dllexport)
#endif

PluginDescription g_PluginDescription[1]=
{
	PluginDescription(
		SE_ID_PHYSICSSYSTEM_NULL,
		1,
		"Null Physics System",
		"Julien Delezenne")
};

class NullPhysicsSystemPlugin : public ManagerPlugin
{
public:
	NullPhysicsSystemPlugin::NullPhysicsSystemPlugin() :
		ManagerPlugin()
	{
	}

	virtual NullPhysicsSystemPlugin::~NullPhysicsSystemPlugin()
	{
	}

	virtual PluginDescription* NullPhysicsSystemPlugin::GetPluginDescription() const
	{
		return &g_PluginDescription[0];
	}

	virtual void NullPhysicsSystemPlugin::Load()
	{
	}

	virtual void NullPhysicsSystemPlugin::Unload()
	{
	}

	virtual SonataEngine::SE_ID NullPhysicsSystemPlugin::GetManagerType() const
	{
		return SE_ID_PHYSICSSYSTEM;
	}

	virtual Manager* NullPhysicsSystemPlugin::CreateManager()
	{
		return new NullPhysicsSystem();
	}

	virtual void NullPhysicsSystemPlugin::DestroyManager(Manager* manager)
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
		case 0: return new NullPhysicsSystemPlugin();
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
