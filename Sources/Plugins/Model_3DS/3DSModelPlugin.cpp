/*=============================================================================
3DSModelPlugin.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "3DSModelPlugin.h"
#include "3DSModelReader.h"
#include "3DSModelWriter.h"

#include <Core/Plugins/PluginManager.h>
#include <Core/Plugins/ManagerPlugin.h>

using namespace SonataEngine;

namespace SE_3DS
{

#ifdef SE_STATIC
	#define SE_EXPORT
#else
	#define SE_EXPORT __declspec(dllexport)
#endif

PluginDescription g_PluginDescription[1]=
{
	PluginDescription(
		SE_ID_DATAMODEL_3DS,
		0x01,
		"3D Studio MAX 3DS",
		"Julien Delezenne")
};

class M3DSModelPlugin : public ModelDataPlugin
{
public:
	M3DSModelPlugin::M3DSModelPlugin() :
		ModelDataPlugin()
	{
		_extensions.Add("obj");
	}

	virtual M3DSModelPlugin::~M3DSModelPlugin()
	{
	}

	virtual PluginDescription* M3DSModelPlugin::GetPluginDescription() const
	{
		return &g_PluginDescription[0];
	}

	virtual void M3DSModelPlugin::Load()
	{
	}

	virtual void M3DSModelPlugin::Unload()
	{
	}

	virtual Array<String> M3DSModelPlugin::GetExtensions() const
	{
		return _extensions;
	}

	virtual bool M3DSModelPlugin::CanRead() const
	{
		return true;
	}

	virtual bool M3DSModelPlugin::CanWrite() const
	{
		return true;
	}

	virtual bool M3DSModelPlugin::CanHandle(const Stream& stream) const
	{
		return true;
	}

	virtual ModelReader* M3DSModelPlugin::CreateReader()
	{
		return new M3DSModelReader();
	}

	virtual ModelWriter* M3DSModelPlugin::CreateWriter()
	{
		return NULL;
	}

	virtual void M3DSModelPlugin::DestroyReader(ModelReader* reader)
	{
		if (reader != NULL)
			delete reader;
	}

	virtual void M3DSModelPlugin::DestroyWriter(ModelWriter* writer)
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
		case 0: return new M3DSModelPlugin();
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
