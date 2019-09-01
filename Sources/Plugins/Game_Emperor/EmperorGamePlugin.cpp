/*=============================================================================
EmperorGamePlugin.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "EmperorGamePlugin.h"
#include "EmperorModelReader.h"

#include <Core/Plugins/PluginManager.h>
#include <Core/Plugins/ManagerPlugin.h>

using namespace SonataEngine;

namespace SE_Emperor
{

#ifdef SE_STATIC
	#define SE_EXPORT
#else
	#define SE_EXPORT __declspec(dllexport)
#endif

class EmperorModelPlugin : public ModelDataPlugin
{
public:
	EmperorModelPlugin();
	virtual ~EmperorModelPlugin();

	virtual PluginDescription* GetPluginDescription() const;
	virtual void Load();
	virtual void Unload();

	virtual Array<String> GetExtensions() const;
	virtual bool CanRead() const;
	virtual bool CanWrite() const;
	virtual bool CanHandle(const Stream& stream) const;

	virtual ModelReader* CreateReader();
	virtual ModelWriter* CreateWriter();
	virtual void DestroyReader(ModelReader* reader);
	virtual void DestroyWriter(ModelWriter* writer);

private:
	Array<String> _extensions;
};

PluginDescription g_PluginDescription[1]=
{
	PluginDescription(
		SE_ID_DATAMODEL_EMPEROR,
		1,
		"Emperor Battle for Dune",
		"Julien Delezenne")
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
		case 0: return new EmperorModelPlugin();
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

EmperorModelPlugin::EmperorModelPlugin() :
	ModelDataPlugin()
{
	_extensions.Add("xaf");
	_extensions.Add("xbf");
}

EmperorModelPlugin::~EmperorModelPlugin()
{
}

PluginDescription* EmperorModelPlugin::GetPluginDescription() const
{
	return &g_PluginDescription[0];
}

void EmperorModelPlugin::Load()
{
}

void EmperorModelPlugin::Unload()
{
}

Array<String> EmperorModelPlugin::GetExtensions() const
{
	return _extensions;
}

bool EmperorModelPlugin::CanRead() const
{
	return true;
}

bool EmperorModelPlugin::CanWrite() const
{
	return false;
}

bool EmperorModelPlugin::CanHandle(const Stream& stream) const
{
	return true;
}

ModelReader* EmperorModelPlugin::CreateReader()
{
	return new EmperorModelReader();
}

ModelWriter* EmperorModelPlugin::CreateWriter()
{
	return NULL;
}

void EmperorModelPlugin::DestroyReader(ModelReader* reader)
{
	if (reader != NULL)
		delete reader;
}

void EmperorModelPlugin::DestroyWriter(ModelWriter* writer)
{
	if (writer != NULL)
		delete writer;
}

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
