/*=============================================================================
D3D8CgShaderSystemLib.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D8CgShaderSystemLib.h"
#include "D3D8CgShaderSystem.h"

#include <Core/Plugins/PluginManager.h>
#include <Core/Plugins/ManagerPlugin.h>
#include <RenderSystem_D3D8/D3D8RenderSystem.h>

using namespace SonataEngine;
using namespace SE_D3D8;

namespace SE_D3D8Cg
{

#ifdef SE_STATIC
	#define SE_EXPORT
#else
	#define SE_EXPORT __declspec(dllexport)
#endif

PluginDescription g_PluginDescription[1]=
{
	PluginDescription(
		SE_ID_SHADERSYSTEM_D3D8CG,
		1,
		"Direct3D8 Cg Shader System",
		"Julien Delezenne")
};

class D3D8CgShaderSystemPlugin : public ManagerPlugin
{
public:
	D3D8CgShaderSystemPlugin::D3D8CgShaderSystemPlugin() :
		ManagerPlugin()
	{
	}

	virtual D3D8CgShaderSystemPlugin::~D3D8CgShaderSystemPlugin()
	{
	}

	virtual PluginDescription* D3D8CgShaderSystemPlugin::GetPluginDescription() const
	{
		return &g_PluginDescription[0];
	}

	virtual void D3D8CgShaderSystemPlugin::Load()
	{
	}

	virtual void D3D8CgShaderSystemPlugin::Unload()
	{
	}

	virtual SonataEngine::SE_ID D3D8CgShaderSystemPlugin::GetManagerType() const
	{
		return SE_ID_SHADERSYSTEM;
	}

	virtual Manager* D3D8CgShaderSystemPlugin::CreateManager()
	{
		D3D8RenderSystem* renderSystem = ((D3D8RenderSystem*)RenderSystem::Current());
		if (renderSystem == NULL)
			return NULL;

		IDirect3DDevice8* pD3DDevice = renderSystem->GetD3DDevice();
		if (pD3DDevice == NULL)
			return NULL;

		return new D3D8CgShaderSystem(pD3DDevice);
	}

	virtual void D3D8CgShaderSystemPlugin::DestroyManager(Manager* manager)
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
		case 0: return new D3D8CgShaderSystemPlugin();
	}
	return NULL;
}

SE_EXPORT void DestroyPlugin(Plugin* plugin)
{
	if (plugin != NULL)
	{
		delete plugin;
	}
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
