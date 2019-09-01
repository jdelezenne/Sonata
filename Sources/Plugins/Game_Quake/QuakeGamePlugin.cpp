/*=============================================================================
QuakeGamePlugin.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "QuakeGamePlugin.h"
#include "QuakeMDLModelReader.h"
#include "QuakeMD2ModelReader.h"
#include "QuakeMD3ModelReader.h"
#include "QuakeBSPSceneReader.h"

#include <Core/Plugins/PluginManager.h>
#include <Core/Plugins/ManagerPlugin.h>

using namespace SonataEngine;

namespace SE_Quake
{

#ifdef SE_STATIC
	#define SE_EXPORT
#else
	#define SE_EXPORT __declspec(dllexport)
#endif

PluginDescription g_PluginDescription[]=
{
	PluginDescription(
		SE_ID_DATAMODEL_MDL,
		0x01,
		"Id Software Quake MDL",
		"Julien Delezenne"),

	PluginDescription(
		SE_ID_DATAMODEL_MD2,
		0x01,
		"Id Software Quake 2 MD2",
		"Julien Delezenne"),

	PluginDescription(
		SE_ID_DATAMODEL_MD3,
		0x01,
		"Id Software Quake 3 MD3",
		"Julien Delezenne"),

	PluginDescription(
		SE_ID_DATASCENE_BSP,
		0x01,
		"Id Software Quake 3 BSP",
		"Julien Delezenne")
};

class QuakeMDLModelPlugin : public ModelDataPlugin
{
private:
	Array<String> _extensions;

public:
	QuakeMDLModelPlugin() :
		ModelDataPlugin()
	{
		_extensions.Add("mdl");
	}

	virtual PluginDescription* GetPluginDescription() const
	{
		return &g_PluginDescription[0];
	}

	virtual void Load()
	{
	}

	virtual void Unload()
	{
	}

	virtual Array<String> GetExtensions() const
	{
		return _extensions;
	}

	virtual bool CanRead() const
	{
		return true;
	}

	virtual bool CanWrite() const
	{
		return true;
	}

	virtual bool CanHandle(const Stream& stream) const
	{
		return true;
	}

	virtual ModelReader* CreateReader()
	{
		return new QuakeMDLModelReader();
	}

	virtual ModelWriter* CreateWriter()
	{
		return NULL;
	}

	virtual void DestroyReader(ModelReader* reader)
	{
		SE_DELETE(reader);
	}

	virtual void DestroyWriter(ModelWriter* writer)
	{
		SE_DELETE(writer);
	}
};

class QuakeMD2ModelPlugin : public ModelDataPlugin
{
private:
	Array<String> _extensions;

public:
	QuakeMD2ModelPlugin() :
		ModelDataPlugin()
	{
		_extensions.Add("md2");
	}

	virtual PluginDescription* GetPluginDescription() const
	{
		return &g_PluginDescription[1];
	}

	virtual void Load()
	{
	}

	virtual void Unload()
	{
	}

	virtual Array<String> GetExtensions() const
	{
		return _extensions;
	}

	virtual bool CanRead() const
	{
		return true;
	}

	virtual bool CanWrite() const
	{
		return true;
	}

	virtual bool CanHandle(const Stream& stream) const
	{
		return true;
	}

	virtual ModelReader* CreateReader()
	{
		return new QuakeMD2ModelReader();
	}

	virtual ModelWriter* CreateWriter()
	{
		return NULL;
	}

	virtual void DestroyReader(ModelReader* reader)
	{
		SE_DELETE(reader);
	}

	virtual void DestroyWriter(ModelWriter* writer)
	{
		SE_DELETE(writer);
	}
};

class QuakeMD3ModelPlugin : public ModelDataPlugin
{
private:
	Array<String> _extensions;

public:
	QuakeMD3ModelPlugin() :
		ModelDataPlugin()
	{
		_extensions.Add("md3");
	}

	virtual PluginDescription* GetPluginDescription() const
	{
		return &g_PluginDescription[2];
	}

	virtual void Load()
	{
	}

	virtual void Unload()
	{
	}

	virtual Array<String> GetExtensions() const
	{
		return _extensions;
	}

	virtual bool CanRead() const
	{
		return true;
	}

	virtual bool CanWrite() const
	{
		return true;
	}

	virtual bool CanHandle(const Stream& stream) const
	{
		return true;
	}

	virtual ModelReader* CreateReader()
	{
		return new QuakeMD3ModelReader();
	}

	virtual ModelWriter* CreateWriter()
	{
		return NULL;
	}

	virtual void DestroyReader(ModelReader* reader)
	{
		SE_DELETE(reader);
	}

	virtual void DestroyWriter(ModelWriter* writer)
	{
		SE_DELETE(writer);
	}
};

class QuakeBSPScenePlugin : public SceneDataPlugin
{
private:
	Array<String> _extensions;

public:
	QuakeBSPScenePlugin() :
		SceneDataPlugin()
	{
		_extensions.Add("bsp");
	}

	virtual PluginDescription* GetPluginDescription() const
	{
		return &g_PluginDescription[3];
	}

	virtual void Load()
	{
	}

	virtual void Unload()
	{
	}

	virtual Array<String> GetExtensions() const
	{
		return _extensions;
	}

	virtual bool CanRead() const
	{
		return true;
	}

	virtual bool CanWrite() const
	{
		return true;
	}

	virtual bool CanHandle(const Stream& stream) const
	{
		return true;
	}

	virtual SceneReader* CreateReader()
	{
		return new QuakeBSPSceneReader();
	}

	virtual SceneWriter* CreateWriter()
	{
		return NULL;
	}

	virtual void DestroyReader(SceneReader* reader)
	{
		SE_DELETE(reader);
	}

	virtual void DestroyWriter(SceneWriter* writer)
	{
		SE_DELETE(writer);
	}
};

#ifndef SE_STATIC
extern "C"
{
#endif

SE_EXPORT int GetPluginCount()
{
	return 4;
}

SE_EXPORT PluginDescription* GetPluginDescription(int index)
{
	return &g_PluginDescription[index];
}

SE_EXPORT Plugin* CreatePlugin(int index)
{
	switch (index)
	{
		case 0: return new QuakeMDLModelPlugin();
		case 1: return new QuakeMD2ModelPlugin();
		case 2: return new QuakeMD3ModelPlugin();
		case 3: return new QuakeBSPScenePlugin();
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
	symbols.GetPluginCount = GetPluginCount;
	symbols.GetPluginDescription = GetPluginDescription;
	symbols.CreatePlugin = CreatePlugin;
	symbols.DestroyPlugin = DestroyPlugin;
	PluginManager::Instance()->RegisterPluginLibrary(symbols);
}
#endif

}
