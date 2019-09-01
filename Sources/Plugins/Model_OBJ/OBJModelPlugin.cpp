/*=============================================================================
OBJModelPlugin.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "OBJModelPlugin.h"
#include "OBJModelReader.h"
#include "OBJModelWriter.h"

#include <Core/Plugins/PluginManager.h>
#include <Core/Plugins/ManagerPlugin.h>

using namespace SonataEngine;

namespace SE_OBJ
{

#ifdef SE_STATIC
	#define SE_EXPORT
#else
	#define SE_EXPORT __declspec(dllexport)
#endif

PluginDescription g_PluginDescription[1]=
{
	PluginDescription(
		SE_ID_DATAMODEL_OBJ,
		0x01,
		"Wavefront OBJ",
		"Julien Delezenne")
};

class OBJModelPlugin : public ModelDataPlugin
{
public:
	OBJModelPlugin::OBJModelPlugin() :
		ModelDataPlugin()
	{
		_extensions.Add("obj");
	}

	virtual OBJModelPlugin::~OBJModelPlugin()
	{
	}

	virtual PluginDescription* OBJModelPlugin::GetPluginDescription() const
	{
		return &g_PluginDescription[0];
	}

	virtual void OBJModelPlugin::Load()
	{
	}

	virtual void OBJModelPlugin::Unload()
	{
	}

	virtual Array<String> OBJModelPlugin::GetExtensions() const
	{
		return _extensions;
	}

	virtual bool OBJModelPlugin::CanRead() const
	{
		return true;
	}

	virtual bool OBJModelPlugin::CanWrite() const
	{
		return true;
	}

	virtual bool OBJModelPlugin::CanHandle(const Stream& stream) const
	{
		return true;
	}

	virtual ModelReader* OBJModelPlugin::CreateReader()
	{
		return new OBJModelReader();
	}

	virtual ModelWriter* OBJModelPlugin::CreateWriter()
	{
		return NULL;
	}

	virtual void OBJModelPlugin::DestroyReader(ModelReader* reader)
	{
		if (reader != NULL)
			delete reader;
	}

	virtual void OBJModelPlugin::DestroyWriter(ModelWriter* writer)
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

SE_EXPORT PluginDescription* GetPluginDescription(int index)
{
	return &g_PluginDescription[index];
}

SE_EXPORT Plugin* CreatePlugin(int index)
{
	switch (index)
	{
		case 0: return new OBJModelPlugin();
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
