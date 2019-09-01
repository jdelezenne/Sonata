/*=============================================================================
RWPhysicsSystemLib.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "RWPhysicsSystemLib.h"
#include "RWPhysicsSystem.h"

#include <Core/Plugins/PluginManager.h>
#include <Core/Plugins/ManagerPlugin.h>

using namespace SonataEngine;

namespace SE_RW
{

#ifdef SE_STATIC
	#define SE_EXPORT
#else
	#define SE_EXPORT __declspec(dllexport)
#endif

PluginDescription g_PluginDescription[1]=
{
	PluginDescription(
		SE_ID_PHYSICSSYSTEM_RW,
		1,
		"RenderWare Physics System",
		"Julien Delezenne")
};

class RWPhysicsSystemPlugin : public ManagerPlugin
{
public:
	RWPhysicsSystemPlugin::RWPhysicsSystemPlugin() :
		ManagerPlugin()
	{
	}

	virtual RWPhysicsSystemPlugin::~RWPhysicsSystemPlugin()
	{
	}

	virtual PluginDescription* RWPhysicsSystemPlugin::GetPluginDescription() const
	{
		return &g_PluginDescription[0];
	}

	virtual void RWPhysicsSystemPlugin::Load()
	{
	}

	virtual void RWPhysicsSystemPlugin::Unload()
	{
	}

	virtual SonataEngine::SE_ID RWPhysicsSystemPlugin::GetManagerType() const
	{
		return SE_ID_PHYSICSSYSTEM;
	}

	virtual Manager* RWPhysicsSystemPlugin::CreateManager()
	{
		return new RWPhysicsSystem();
	}

	virtual void RWPhysicsSystemPlugin::DestroyManager(Manager* manager)
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
		case 0: return new RWPhysicsSystemPlugin();
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
