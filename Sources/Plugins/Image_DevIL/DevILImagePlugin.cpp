/*=============================================================================
DevILImagePlugin.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "DevILImagePlugin.h"
#include "DevILImageReader.h"
#include "DevILImageWriter.h"

#include <Core/Plugins/PluginManager.h>
#include <Core/Plugins/ManagerPlugin.h>

using namespace SonataEngine;

namespace SE_DevIL
{

#ifdef SE_STATIC
	#define SE_EXPORT
#else
	#define SE_EXPORT __declspec(dllexport)
#endif

PluginDescription g_PluginDescription[1]=
{
	PluginDescription(
		SE_ID_DATAIMAGE_DEVIL,
		0x01,
		"DevIL",
		"Julien Delezenne")
};

class DevILImagePlugin : public ImageDataPlugin
{
public:
	DevILImagePlugin::DevILImagePlugin() :
		ImageDataPlugin()
	{
	}

	virtual DevILImagePlugin::~DevILImagePlugin()
	{
	}

	virtual PluginDescription* DevILImagePlugin::GetPluginDescription() const
	{
		return &g_PluginDescription[0];
	}

	virtual void DevILImagePlugin::Load()
	{
	}

	virtual void DevILImagePlugin::Unload()
	{
	}

	virtual Array<String> DevILImagePlugin::GetExtensions() const
	{
		Array<String> extensions;
		extensions.Add("bmp");
		extensions.Add("cut");
		extensions.Add("dds");
		extensions.Add("gif");
		extensions.Add("ico");
		extensions.Add("jpg");
		extensions.Add("jpeg");
		extensions.Add("lif");
		extensions.Add("mng");
		extensions.Add("pcx");
		extensions.Add("pic");
		extensions.Add("png");
		extensions.Add("pnm");
		extensions.Add("psd");
		extensions.Add("psp");
		extensions.Add("sgi");
		extensions.Add("tga");
		extensions.Add("tif");
		extensions.Add("raw");
		return extensions;
	}

	virtual bool DevILImagePlugin::CanRead() const
	{
		return true;
	}

	virtual bool DevILImagePlugin::CanWrite() const
	{
		return true;
	}

	virtual bool DevILImagePlugin::CanHandle(const Stream& stream) const
	{
		return true;
	}

	virtual ImageReader* DevILImagePlugin::CreateReader()
	{
		return new DevILImageReader();
	}

	virtual ImageWriter* DevILImagePlugin::CreateWriter()
	{
		return new DevILImageWriter();
	}

	virtual void DevILImagePlugin::DestroyReader(ImageReader* reader)
	{
		if (reader != NULL)
		{
			delete reader;
		}
	}

	virtual void DevILImagePlugin::DestroyWriter(ImageWriter* writer)
	{
		if (writer != NULL)
		{
			delete writer;
		}
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
		case 0: return new DevILImagePlugin();
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
