/*=============================================================================
DI8InputSystemLib.cpp
Project: Sonata Engine
Copyright (c) 2005
Julien Delezenne
=============================================================================*/

#include "DI8InputSystemLib.h"
#include "DI8InputSystem.h"

#include <Core/Plugins/PluginManager.h>
#include <Core/Plugins/ManagerPlugin.h>

using namespace SonataEngine;

namespace SE_pDI8
{

#ifdef SE_STATIC
	#define SE_EXPORT
#else
	#define SE_EXPORT __declspec(dllexport)
#endif

PluginDescription g_PluginDescription[1]=
{
	PluginDescription(
		SE_ID_INPUTSYSTEM_DI8,
		1,
		"DirectInput8 Input System",
		"Julien Delezenne")
};

class DI8InputSystemPlugin : public ManagerPlugin
{
public:
	DI8InputSystemPlugin::DI8InputSystemPlugin() :
		ManagerPlugin()
	{
	}

	virtual DI8InputSystemPlugin::~DI8InputSystemPlugin()
	{
	}

	virtual PluginDescription* DI8InputSystemPlugin::GetPluginDescription() const
	{
		return &g_PluginDescription[0];
	}

	virtual void DI8InputSystemPlugin::Load()
	{
	}

	virtual void DI8InputSystemPlugin::Unload()
	{
	}

	virtual SonataEngine::SE_ID DI8InputSystemPlugin::GetManagerType() const
	{
		return SE_ID_INPUTSYSTEM;
	}

	virtual Manager* DI8InputSystemPlugin::CreateManager()
	{
		return new DI8InputSystem();
	}

	virtual void DI8InputSystemPlugin::DestroyManager(Manager* manager)
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
		case 0: return new DI8InputSystemPlugin();
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
