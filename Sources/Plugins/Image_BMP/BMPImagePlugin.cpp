/*=============================================================================
BMPImagePlugin.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "BMPImagePlugin.h"
#include "BMPImageReader.h"
#include "BMPImageWriter.h"

#include <Core/Plugins/PluginManager.h>
#include <Core/Plugins/ManagerPlugin.h>

using namespace SonataEngine;

namespace SE_BMP
{

#ifdef SE_STATIC
	#define SE_EXPORT
#else
	#define SE_EXPORT __declspec(dllexport)
#endif

class BMPImagePlugin : public ImageDataPlugin
{
public:
	BMPImagePlugin();
	virtual ~BMPImagePlugin();

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

// Colors: 1, 4, 8, 16, 24, 32 bits
// Compression: Uncompressed, RLE
// Author: Microsoft Corporation

PluginDescription g_PluginDescription[1]=
{
	PluginDescription(
		SE_ID_DATAIMAGE_BMP,
		0x01,
		"BMP Image",
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
		case 0: return new BMPImagePlugin();
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

BMPImagePlugin::BMPImagePlugin() :
	ImageDataPlugin()
{
}

BMPImagePlugin::~BMPImagePlugin()
{
}

PluginDescription* BMPImagePlugin::GetPluginDescription() const
{
	return &g_PluginDescription[0];
}

void BMPImagePlugin::Load()
{
}

void BMPImagePlugin::Unload()
{
}

Array<String> BMPImagePlugin::GetExtensions() const
{
	Array<String> extensions;
	extensions.Add("bmp");
	return extensions;
}

bool BMPImagePlugin::CanRead() const
{
	return true;
}

bool BMPImagePlugin::CanWrite() const
{
	return true;
}

bool BMPImagePlugin::CanHandle(const Stream& stream) const
{
	return true;
}

ImageReader* BMPImagePlugin::CreateReader()
{
	return new BMPImageReader();
}

ImageWriter* BMPImagePlugin::CreateWriter()
{
	return new BMPImageWriter();
}

void BMPImagePlugin::DestroyReader(ImageReader* reader)
{
	if (reader != NULL)
	{
		delete reader;
	}
}

void BMPImagePlugin::DestroyWriter(ImageWriter* writer)
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

#if defined(_WIN32) && !defined(SE_STATIC)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ulReason, LPVOID lpReserved)
{
	switch (ulReason)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}

	return TRUE;
}

#endif
