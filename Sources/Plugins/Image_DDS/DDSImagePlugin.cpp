/*=============================================================================
DDSImagePlugin.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "DDSImagePlugin.h"
#include "DDSImageReader.h"
#include "DDSImageWriter.h"

#include <Core/Plugins/PluginManager.h>
#include <Core/Plugins/ManagerPlugin.h>

using namespace SonataEngine;

namespace SE_DDS
{

#ifdef SE_STATIC
	#define SE_EXPORT
#else
	#define SE_EXPORT __declspec(dllexport)
#endif

class DDSImagePlugin : public ImageDataPlugin
{
public:
	DDSImagePlugin();
	virtual ~DDSImagePlugin();

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

// Colors: 8, 16, 24, 32 bits
// Compression: Uncompressed, RLE
// Author: Truevision, Inc.

PluginDescription g_PluginDescription[1]=
{
	PluginDescription(
		SE_ID_DATAIMAGE_DDS,
		0x01,
		"DDS Image",
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
		case 0: return new DDSImagePlugin();
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

DDSImagePlugin::DDSImagePlugin() :
	ImageDataPlugin()
{
}

DDSImagePlugin::~DDSImagePlugin()
{
}

PluginDescription* DDSImagePlugin::GetPluginDescription() const
{
	return &g_PluginDescription[0];
}

void DDSImagePlugin::Load()
{
}

void DDSImagePlugin::Unload()
{
}

Array<String> DDSImagePlugin::GetExtensions() const
{
	Array<String> extensions;
	extensions.Add("dds");
	return extensions;
}

bool DDSImagePlugin::CanRead() const
{
	return true;
}

bool DDSImagePlugin::CanWrite() const
{
	return false;
}

bool DDSImagePlugin::CanHandle(const Stream& stream) const
{
	return true;
}

ImageReader* DDSImagePlugin::CreateReader()
{
	return new DDSImageReader();
}

ImageWriter* DDSImagePlugin::CreateWriter()
{
	return NULL;
}

void DDSImagePlugin::DestroyReader(ImageReader* reader)
{
	if (reader != NULL)
	{
		delete reader;
	}
}

void DDSImagePlugin::DestroyWriter(ImageWriter* writer)
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
