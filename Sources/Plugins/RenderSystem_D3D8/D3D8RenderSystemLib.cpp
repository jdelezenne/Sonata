/*=============================================================================
D3D8RenderSystemLib.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D8RenderSystemLib.h"
#include "D3D8RenderSystem.h"
#include "D3D8ShaderSystem.h"

#include <Core/Plugins/PluginManager.h>
#include <Core/Plugins/ManagerPlugin.h>

using namespace SonataEngine;

namespace SE_D3D8
{

#ifdef SE_STATIC
	#define SE_EXPORT
#else
	#define SE_EXPORT __declspec(dllexport)
#endif

PluginDescription g_PluginDescription[2]=
{
	PluginDescription(
		SE_ID_RENDERSYSTEM_D3D8,
		1,
		"Direct3D8 Render System",
		"Julien Delezenne"),

	PluginDescription(
		SE_ID_SHADERSYSTEM_D3D8,
		1,
		"Direct3D8 Shader System",
		"Julien Delezenne")
};

class D3D8RenderSystemPlugin : public ManagerPlugin
{
public:
	D3D8RenderSystemPlugin::D3D8RenderSystemPlugin() :
		ManagerPlugin()
	{
	}

	virtual D3D8RenderSystemPlugin::~D3D8RenderSystemPlugin()
	{
	}

	virtual PluginDescription* D3D8RenderSystemPlugin::GetPluginDescription() const
	{
		return &g_PluginDescription[0];
	}

	virtual void D3D8RenderSystemPlugin::Load()
	{
	}

	virtual void D3D8RenderSystemPlugin::Unload()
	{
	}

	virtual SonataEngine::SE_ID D3D8RenderSystemPlugin::GetManagerType() const
	{
		return SE_ID_RENDERSYSTEM;
	}

	virtual Manager* D3D8RenderSystemPlugin::CreateManager()
	{
		return new D3D8RenderSystem();
	}

	virtual void D3D8RenderSystemPlugin::DestroyManager(Manager* manager)
	{
		SE_DELETE(manager);
	}
};

class D3D8ShaderSystemPlugin : public ManagerPlugin
{
public:
	D3D8ShaderSystemPlugin::D3D8ShaderSystemPlugin() :
		ManagerPlugin()
	{
	}

	virtual D3D8ShaderSystemPlugin::~D3D8ShaderSystemPlugin()
	{
	}

	virtual PluginDescription* D3D8ShaderSystemPlugin::GetPluginDescription() const
	{
		return &g_PluginDescription[1];
	}

	virtual void D3D8ShaderSystemPlugin::Load()
	{
	}

	virtual void D3D8ShaderSystemPlugin::Unload()
	{
	}

	virtual SonataEngine::SE_ID D3D8ShaderSystemPlugin::GetManagerType() const
	{
		return SE_ID_SHADERSYSTEM;
	}

	virtual Manager* D3D8ShaderSystemPlugin::CreateManager()
	{
		D3D8RenderSystem* renderSystem = ((D3D8RenderSystem*)RenderSystem::Current());
		if (renderSystem == NULL)
			return NULL;

		IDirect3DDevice8* pD3DDevice = renderSystem->GetD3DDevice();
		if (pD3DDevice == NULL)
			return NULL;

		return new D3D8ShaderSystem(pD3DDevice);
	}

	virtual void D3D8ShaderSystemPlugin::DestroyManager(Manager* manager)
	{
		SE_DELETE(manager);
	}
};

#ifndef SE_STATIC
extern "C"
{
#endif

SE_EXPORT int GetPluginCount()
{
	return 2;
}

SE_EXPORT PluginDescription* GetPluginDescription(int index)
{
	return &g_PluginDescription[index];
}

SE_EXPORT Plugin* CreatePlugin(int index)
{
	switch (index)
	{
		case 0: return new D3D8RenderSystemPlugin();
		case 1: return new D3D8ShaderSystemPlugin();
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
	symbols.GetPluginCount = GetPluginCount;
	symbols.GetPluginDescription = GetPluginDescription;
	symbols.CreatePlugin = CreatePlugin;
	symbols.DestroyPlugin = DestroyPlugin;
	PluginManager::Instance()->RegisterPluginLibrary(symbols);
}
#endif

}
