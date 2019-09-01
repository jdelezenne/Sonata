/*=============================================================================
GLCgShaderSystemLib.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "GLCgShaderSystemLib.h"
#include "GLCgShaderSystem.h"

#include <Core/Plugins/PluginManager.h>
#include <Core/Plugins/ManagerPlugin.h>
#include <Graphics/Renderer/RenderSystem.h>

using namespace SonataEngine;

namespace SE_GLCg
{

#ifdef SE_STATIC
	#define SE_EXPORT
#else
	#define SE_EXPORT __declspec(dllexport)
#endif

PluginDescription g_PluginDescription[1]=
{
	PluginDescription(
		SE_ID_SHADERSYSTEM_GLCG,
		1,
		"OpenGL Cg Shader System",
		"Julien Delezenne")
};

class GLCgShaderSystemPlugin : public ManagerPlugin
{
public:
	GLCgShaderSystemPlugin::GLCgShaderSystemPlugin() :
		ManagerPlugin()
	{
	}

	virtual GLCgShaderSystemPlugin::~GLCgShaderSystemPlugin()
	{
	}

	virtual PluginDescription* GLCgShaderSystemPlugin::GetPluginDescription() const
	{
		return &g_PluginDescription[0];
	}

	virtual void GLCgShaderSystemPlugin::Load()
	{
	}

	virtual void GLCgShaderSystemPlugin::Unload()
	{
	}

	virtual SonataEngine::SE_ID GLCgShaderSystemPlugin::GetManagerType() const
	{
		return SE_ID_SHADERSYSTEM;
	}

	virtual Manager* GLCgShaderSystemPlugin::CreateManager()
	{
		RenderSystem* renderSystem = RenderSystem::Current();
		if (renderSystem == NULL)
			return NULL;

		return new GLCgShaderSystem();
	}

	virtual void GLCgShaderSystemPlugin::DestroyManager(Manager* manager)
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
		case 0: return new GLCgShaderSystemPlugin();
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
