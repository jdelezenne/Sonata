/*=============================================================================
D3D9RenderSystemLib.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D9RenderSystemLib.h"
#include "D3D9RenderSystem.h"
#include "D3D9HLSLShaderSystem.h"

#include <Core/Plugins/PluginManager.h>
#include <Core/Plugins/ManagerPlugin.h>

using namespace SonataEngine;

namespace SE_D3D9
{

#ifdef SE_STATIC
	#define SE_EXPORT
#else
	#define SE_EXPORT __declspec(dllexport)
#endif

PluginDescription g_PluginDescription[2]=
{
	PluginDescription(
		SE_ID_RENDERSYSTEM_D3D9,
		1,
		"Direct3D9 Render System",
		"Julien Delezenne"),

	PluginDescription(
		SE_ID_SHADERSYSTEM_HLSL9,
		1,
		"Direct3D9 HSLS Shader System",
		"Julien Delezenne")
};

class D3D9RenderSystemPlugin : public ManagerPlugin
{
public:
	D3D9RenderSystemPlugin::D3D9RenderSystemPlugin() :
		ManagerPlugin()
	{
	}

	virtual D3D9RenderSystemPlugin::~D3D9RenderSystemPlugin()
	{
	}

	virtual PluginDescription* D3D9RenderSystemPlugin::GetPluginDescription() const
	{
		return &g_PluginDescription[0];
	}

	virtual void D3D9RenderSystemPlugin::Load()
	{
	}

	virtual void D3D9RenderSystemPlugin::Unload()
	{
	}

	virtual SonataEngine::SE_ID D3D9RenderSystemPlugin::GetManagerType() const
	{
		return SE_ID_RENDERSYSTEM;
	}

	virtual Manager* D3D9RenderSystemPlugin::CreateManager()
	{
		return new D3D9RenderSystem();
	}

	virtual void D3D9RenderSystemPlugin::DestroyManager(Manager* manager)
	{
		SE_DELETE(manager);
	}
};

class D3D9HLSLShaderSystemPlugin : public ManagerPlugin
{
public:
	D3D9HLSLShaderSystemPlugin::D3D9HLSLShaderSystemPlugin() :
		ManagerPlugin()
	{
	}

	virtual D3D9HLSLShaderSystemPlugin::~D3D9HLSLShaderSystemPlugin()
	{
	}

	virtual PluginDescription* D3D9HLSLShaderSystemPlugin::GetPluginDescription() const
	{
		return &g_PluginDescription[1];
	}

	virtual void D3D9HLSLShaderSystemPlugin::Load()
	{
	}

	virtual void D3D9HLSLShaderSystemPlugin::Unload()
	{
	}

	virtual SonataEngine::SE_ID D3D9HLSLShaderSystemPlugin::GetManagerType() const
	{
		return SE_ID_SHADERSYSTEM;
	}

	virtual Manager* D3D9HLSLShaderSystemPlugin::CreateManager()
	{
		D3D9RenderSystem* renderSystem = ((D3D9RenderSystem*)RenderSystem::Current());
		if (renderSystem == NULL)
			return NULL;

		IDirect3DDevice9* pD3DDevice = renderSystem->GetD3DDevice();
		if (pD3DDevice == NULL)
			return NULL;

		return new D3D9HLSLShaderSystem(pD3DDevice);
	}

	virtual void D3D9HLSLShaderSystemPlugin::DestroyManager(Manager* manager)
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
		case 0: return new D3D9RenderSystemPlugin();
		case 1: return new D3D9HLSLShaderSystemPlugin();
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
