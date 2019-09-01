/*=============================================================================
Win32InputSystemLib.cpp
Project: Sonata Engine
Copyright (c) 2005
Julien Delezenne
=============================================================================*/

#include "Win32InputSystemLib.h"
#include "Win32InputSystem.h"

#include <Core/Plugins/PluginManager.h>
#include <Core/Plugins/ManagerPlugin.h>

using namespace SonataEngine;

namespace SE_Win32
{

#ifdef SE_STATIC
	#define SE_EXPORT
#else
	#define SE_EXPORT __declspec(dllexport)
#endif

PluginDescription g_PluginDescription[1]=
{
	PluginDescription(
		SE_ID_INPUTSYSTEM_WIN32,
		1,
		"Win32 Input System",
		"Julien Delezenne")
};

class Win32InputSystemPlugin : public ManagerPlugin
{
public:
	Win32InputSystemPlugin::Win32InputSystemPlugin() :
		ManagerPlugin()
	{
	}

	virtual Win32InputSystemPlugin::~Win32InputSystemPlugin()
	{
	}

	virtual PluginDescription* Win32InputSystemPlugin::GetPluginDescription() const
	{
		return &g_PluginDescription[0];
	}

	virtual void Win32InputSystemPlugin::Load()
	{
	}

	virtual void Win32InputSystemPlugin::Unload()
	{
	}

	virtual SonataEngine::SE_ID Win32InputSystemPlugin::GetManagerType() const
	{
		return SE_ID_INPUTSYSTEM;
	}

	virtual Manager* Win32InputSystemPlugin::CreateManager()
	{
		return new Win32InputSystem();
	}

	virtual void Win32InputSystemPlugin::DestroyManager(Manager* manager)
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
		case 0: return new Win32InputSystemPlugin();
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
