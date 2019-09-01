/*=============================================================================
ODEPhysicsSystemLib.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ODEPhysicsSystemLib.h"
#include "ODEPhysicsSystem.h"

#include <Core/Plugins/PluginManager.h>
#include <Core/Plugins/ManagerPlugin.h>

using namespace SonataEngine;

namespace SE_ODE
{

#ifdef SE_STATIC
	#define SE_EXPORT
#else
	#define SE_EXPORT __declspec(dllexport)
#endif

PluginDescription g_PluginDescription[1]=
{
	PluginDescription(
		SE_ID_PHYSICSSYSTEM_ODE,
		1,
		"ODE Physics System",
		"Julien Delezenne")
};

class ODEPhysicsSystemPlugin : public ManagerPlugin
{
public:
	ODEPhysicsSystemPlugin::ODEPhysicsSystemPlugin() :
		ManagerPlugin()
	{
	}

	virtual ODEPhysicsSystemPlugin::~ODEPhysicsSystemPlugin()
	{
	}

	virtual PluginDescription* ODEPhysicsSystemPlugin::GetPluginDescription() const
	{
		return &g_PluginDescription[0];
	}

	virtual void ODEPhysicsSystemPlugin::Load()
	{
	}

	virtual void ODEPhysicsSystemPlugin::Unload()
	{
	}

	virtual SonataEngine::SE_ID ODEPhysicsSystemPlugin::GetManagerType() const
	{
		return SE_ID_PHYSICSSYSTEM;
	}

	virtual Manager* ODEPhysicsSystemPlugin::CreateManager()
	{
		return new ODEPhysicsSystem();
	}

	virtual void ODEPhysicsSystemPlugin::DestroyManager(Manager* manager)
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
		case 0: return new ODEPhysicsSystemPlugin();
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
