/*=============================================================================
NXPhysicsSystemLib.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "NXPhysicsSystemLib.h"
#include "NXPhysicsSystem.h"

#include <Core/Plugins/PluginManager.h>
#include <Core/Plugins/ManagerPlugin.h>

using namespace SonataEngine;

namespace SE_NX
{

#ifdef SE_STATIC
	#define SE_EXPORT
#else
	#define SE_EXPORT __declspec(dllexport)
#endif

PluginDescription g_PluginDescription[1]=
{
	PluginDescription(
		SE_ID_PHYSICSSYSTEM_NX,
		1,
		"Ageia PhysX Physics System",
		"Julien Delezenne")
};

class NXPhysicsSystemPlugin : public ManagerPlugin
{
public:
	NXPhysicsSystemPlugin::NXPhysicsSystemPlugin() :
		ManagerPlugin()
	{
	}

	virtual NXPhysicsSystemPlugin::~NXPhysicsSystemPlugin()
	{
	}

	virtual PluginDescription* NXPhysicsSystemPlugin::GetPluginDescription() const
	{
		return &g_PluginDescription[0];
	}

	virtual void NXPhysicsSystemPlugin::Load()
	{
	}

	virtual void NXPhysicsSystemPlugin::Unload()
	{
	}

	virtual SonataEngine::SE_ID NXPhysicsSystemPlugin::GetManagerType() const
	{
		return SE_ID_PHYSICSSYSTEM;
	}

	virtual Manager* NXPhysicsSystemPlugin::CreateManager()
	{
		return new NXPhysicsSystem();
	}

	virtual void NXPhysicsSystemPlugin::DestroyManager(Manager* manager)
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
		case 0: return new NXPhysicsSystemPlugin();
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
