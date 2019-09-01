/*=============================================================================
WestwoodGamePlugin.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "WestwoodGamePlugin.h"
#include "WestwoodW3DReader.h"

#include <Core/Plugins/PluginManager.h>
#include <Core/Plugins/ManagerPlugin.h>

using namespace SonataEngine;

namespace SE_Westwood
{

#ifdef SE_STATIC
	#define SE_EXPORT
#else
	#define SE_EXPORT __declspec(dllexport)
#endif

PluginDescription g_PluginDescription[2] =
{
	PluginDescription(
		SE_ID_DATAMODEL_W3D,
		1,
		"Westwood W3D",
		"Julien Delezenne")
};

class WestwoodW3DPlugin : public ModelDataPlugin
{
public:
	WestwoodW3DPlugin::WestwoodW3DPlugin() :
		ModelDataPlugin()
	{
		_extensions.Add("w3d");
	}

	virtual WestwoodW3DPlugin::~WestwoodW3DPlugin()
	{
	}

	virtual PluginDescription* WestwoodW3DPlugin::GetPluginDescription() const
	{
		return &g_PluginDescription[0];
	}

	virtual void WestwoodW3DPlugin::Load()
	{
	}

	virtual void WestwoodW3DPlugin::Unload()
	{
	}

	virtual Array<String> WestwoodW3DPlugin::GetExtensions() const
	{
		return _extensions;
	}

	virtual bool WestwoodW3DPlugin::CanRead() const
	{
		return true;
	}

	virtual bool WestwoodW3DPlugin::CanWrite() const
	{
		return false;
	}

	virtual bool WestwoodW3DPlugin::CanHandle(const Stream& stream) const
	{
		return true;
	}

	virtual ModelReader* WestwoodW3DPlugin::CreateReader()
	{
		return new WestwoodW3DReader();
	}

	virtual ModelWriter* WestwoodW3DPlugin::CreateWriter()
	{
		return NULL;
	}

	virtual void WestwoodW3DPlugin::DestroyReader(ModelReader* reader)
	{
		if (reader != NULL)
			delete reader;
	}

	virtual void WestwoodW3DPlugin::DestroyWriter(ModelWriter* writer)
	{
		if (writer != NULL)
			delete writer;
	}

private:
	Array<String> _extensions;
};

#ifndef SE_STATIC
extern "C"
{
#endif

SE_EXPORT int GetPluginCount()
{
	return 1;
}

SE_EXPORT PluginDescription* GetPluginDescription(int index)
{
	return &g_PluginDescription[index];
}

SE_EXPORT Plugin* CreatePlugin(int index)
{
	switch (index)
	{
		case 0: return new WestwoodW3DPlugin();
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
