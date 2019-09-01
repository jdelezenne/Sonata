/*=============================================================================
GLRenderSystemLib.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "GLRenderSystemLib.h"
#include "GLRenderSystem.h"
#include "GLSLShaderSystem.h"

#include <Core/Plugins/PluginManager.h>
#include <Core/Plugins/ManagerPlugin.h>

using namespace SonataEngine;

namespace SE_GL
{

#ifdef SE_STATIC
	#define SE_EXPORT
#else
	#define SE_EXPORT __declspec(dllexport)
#endif

PluginDescription g_PluginDescription[2]=
{
	PluginDescription(
		SE_ID_RENDERSYSTEM_GL,
		1,
		"OpenGL Render System",
		"Julien Delezenne"),

	PluginDescription(
		SE_ID_SHADERSYSTEM_GLSL,
		1,
		"OpenGL GLSL Shader System",
		"Julien Delezenne")
};

class GLRenderSystemPlugin : public ManagerPlugin
{
public:
	GLRenderSystemPlugin::GLRenderSystemPlugin() :
		ManagerPlugin()
	{
	}

	virtual GLRenderSystemPlugin::~GLRenderSystemPlugin()
	{
	}

	virtual PluginDescription* GLRenderSystemPlugin::GetPluginDescription() const
	{
		return &g_PluginDescription[0];
	}

	virtual void GLRenderSystemPlugin::Load()
	{
	}

	virtual void GLRenderSystemPlugin::Unload()
	{
	}

	virtual SonataEngine::SE_ID GLRenderSystemPlugin::GetManagerType() const
	{
		return SE_ID_RENDERSYSTEM;
	}

	virtual Manager* GLRenderSystemPlugin::CreateManager()
	{
		return new GLRenderSystem();
	}

	virtual void GLRenderSystemPlugin::DestroyManager(Manager* manager)
	{
		SE_DELETE(manager);
	}
};

class GLSLShaderSystemPlugin : public ManagerPlugin
{
public:
	GLSLShaderSystemPlugin::GLSLShaderSystemPlugin() :
		ManagerPlugin()
	{
	}

	virtual GLSLShaderSystemPlugin::~GLSLShaderSystemPlugin()
	{
	}

	virtual PluginDescription* GLSLShaderSystemPlugin::GetPluginDescription() const
	{
		return &g_PluginDescription[1];
	}

	virtual void GLSLShaderSystemPlugin::Load()
	{
	}

	virtual void GLSLShaderSystemPlugin::Unload()
	{
	}

	virtual SonataEngine::SE_ID GLSLShaderSystemPlugin::GetManagerType() const
	{
		return SE_ID_SHADERSYSTEM;
	}

	virtual Manager* GLSLShaderSystemPlugin::CreateManager()
	{
		return new GLSLShaderSystem();
	}

	virtual void GLSLShaderSystemPlugin::DestroyManager(Manager* manager)
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
		case 0: return new GLRenderSystemPlugin();
		case 1: return new GLSLShaderSystemPlugin();
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
