/*=============================================================================
SoftwarePhysicsSystemLib.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "SoftwarePhysicsSystemLib.h"
#include "SoftwarePhysicsSystem.h"

#include <Core/Plugins/PluginManager.h>
#include <Core/Plugins/ManagerPlugin.h>

using namespace SonataEngine;

namespace SE_Software
{

#ifdef SE_STATIC
	#define SE_EXPORT
#else
	#define SE_EXPORT __declspec(dllexport)
#endif

PluginDescription g_PluginDescription[1]=
{
	PluginDescription(
		SE_ID_PHYSICSSYSTEM_SOFTWARE,
		1,
		"Software Physics System",
		"Julien Delezenne")
};

class SoftwarePhysicsSystemPlugin : public ManagerPlugin
{
public:
	SoftwarePhysicsSystemPlugin::SoftwarePhysicsSystemPlugin() :
		ManagerPlugin()
	{
	}

	virtual SoftwarePhysicsSystemPlugin::~SoftwarePhysicsSystemPlugin()
	{
	}

	virtual PluginDescription* SoftwarePhysicsSystemPlugin::GetPluginDescription() const
	{
		return &g_PluginDescription[0];
	}

	virtual void SoftwarePhysicsSystemPlugin::Load()
	{
	}

	virtual void SoftwarePhysicsSystemPlugin::Unload()
	{
	}

	virtual SonataEngine::SE_ID SoftwarePhysicsSystemPlugin::GetManagerType() const
	{
		return SE_ID_PHYSICSSYSTEM;
	}

	virtual Manager* SoftwarePhysicsSystemPlugin::CreateManager()
	{
		return new SoftwarePhysicsSystem();
	}

	virtual void SoftwarePhysicsSystemPlugin::DestroyManager(Manager* manager)
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
		case 0: return new SoftwarePhysicsSystemPlugin();
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
