/*=============================================================================
SDLImagePlugin.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "SDLImagePlugin.h"
#include "SDLImageReader.h"
#include "SDLImageWriter.h"

#include <Core/Plugins/PluginManager.h>
#include <Core/Plugins/ManagerPlugin.h>

using namespace SonataEngine;

namespace SE_SDLImage
{

#ifdef SE_STATIC
	#define SE_EXPORT
#else
	#define SE_EXPORT __declspec(dllexport)
#endif

class SDLImagePlugin : public ImageDataPlugin
{
public:
	SDLImagePlugin();
	virtual ~SDLImagePlugin();

	virtual PluginDescription* GetPluginDescription() const;
	virtual void Load();
	virtual void Unload();

	virtual Array<String> GetExtensions() const;
	virtual bool CanRead() const;
	virtual bool CanWrite() const;
	virtual bool CanHandle(const Stream& stream) const;

	virtual ImageReader* CreateReader();
	virtual ImageWriter* CreateWriter();
	virtual void DestroyReader(ImageReader* reader);
	virtual void DestroyWriter(ImageWriter* writer);
};

PluginDescription g_PluginDescription[1]=
{
	PluginDescription(
		SE_ID_DATAIMAGE_SDL,
		0x01,
		"SDL Image",
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
		case 0: return new SDLImagePlugin();
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

SDLImagePlugin::SDLImagePlugin() :
	ImageDataPlugin()
{
}

SDLImagePlugin::~SDLImagePlugin()
{
}

PluginDescription* SDLImagePlugin::GetPluginDescription() const
{
	return &g_PluginDescription[0];
}

void SDLImagePlugin::Load()
{
}

void SDLImagePlugin::Unload()
{
}

Array<String> SDLImagePlugin::GetExtensions() const
{
	Array<String> extensions;
	extensions.Add("bmp");
	return extensions;
}

bool SDLImagePlugin::CanRead() const
{
	return true;
}

bool SDLImagePlugin::CanWrite() const
{
	return true;
}

bool SDLImagePlugin::CanHandle(const Stream& stream) const
{
	return true;
}

ImageReader* SDLImagePlugin::CreateReader()
{
	return new SDLImageReader();
}

ImageWriter* SDLImagePlugin::CreateWriter()
{
	return NULL;
}

void SDLImagePlugin::DestroyReader(ImageReader* reader)
{
	if (reader != NULL)
	{
		delete reader;
	}
}

void SDLImagePlugin::DestroyWriter(ImageWriter* writer)
{
	if (writer != NULL)
	{
		delete writer;
	}
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
