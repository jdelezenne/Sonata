/*=============================================================================
WAVSoundPlugin.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "WAVSoundPlugin.h"
#include "WAVSoundReader.h"
#include "WAVSoundWriter.h"

#include <Core/Plugins/PluginManager.h>
#include <Core/Plugins/ManagerPlugin.h>

using namespace SonataEngine;

namespace SE_WAV
{

#ifdef SE_STATIC
	#define SE_EXPORT
#else
	#define SE_EXPORT __declspec(dllexport)
#endif

PluginDescription g_PluginDescription[1]=
{
	PluginDescription(
		SE_ID_DATASOUND_WAV,
		0x01,
		"WAV Sound",
		"Julien Delezenne")
};

class WAVSoundPlugin : public SoundDataPlugin
{
public:
	WAVSoundPlugin::WAVSoundPlugin() :
		SoundDataPlugin()
	{
	}

	virtual WAVSoundPlugin::~WAVSoundPlugin()
	{
	}

	virtual PluginDescription* WAVSoundPlugin::GetPluginDescription() const
	{
		return &g_PluginDescription[0];
	}

	virtual void WAVSoundPlugin::Load()
	{
	}

	virtual void WAVSoundPlugin::Unload()
	{
	}

	virtual Array<String> WAVSoundPlugin::GetExtensions() const
	{
		Array<String> extensions;
		extensions.Add("wav");
		return extensions;
	}

	virtual bool WAVSoundPlugin::CanRead() const
	{
		return true;
	}

	virtual bool WAVSoundPlugin::CanWrite() const
	{
		return true;
	}

	virtual bool WAVSoundPlugin::CanHandle(const Stream& stream) const
	{
		return true;
	}

	virtual SoundReader* WAVSoundPlugin::CreateReader()
	{
		return new WAVSoundReader();
	}

	virtual SoundWriter* WAVSoundPlugin::CreateWriter()
	{
		return new WAVSoundWriter();
	}

	virtual void WAVSoundPlugin::DestroyReader(SoundReader* reader)
	{
		if (reader != NULL)
			delete reader;
	}

	virtual void WAVSoundPlugin::DestroyWriter(SoundWriter* writer)
	{
		if (writer != NULL)
			delete writer;
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
		case 0: return new WAVSoundPlugin();
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
