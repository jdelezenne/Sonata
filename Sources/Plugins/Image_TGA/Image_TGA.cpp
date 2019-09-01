/*=============================================================================
Image_TGA.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "Image_TGA.h"
#include "TGAImageReader.h"
#include "TGAImageWriter.h"
#include <Core/Plugins/PluginManager.h>

namespace SE_ImagePlugin_TGA
{

// Colors: 8, 16, 24, 32 bits
// Compression: Uncompressed, RLE
// Author: Truevision, Inc.

PluginDescription g_PluginDescription[1]=
{
	PluginDescription(
		SE_ID_DATAIMAGE_TGA,
		SE_ID_DATA,
		0x01,
		"TGA Image",
		"Julien Delezenne")
};

SE_IMAGEPLUGIN_EXPORT PluginDescription* GetPluginDescription(int index)
{
	return &g_PluginDescription[index];
}

SE_IMAGEPLUGIN_EXPORT Plugin* CreatePlugin(int index)
{
	switch (index)
	{
		case 0: return new ImagePlugin_TGA();
	}
	return NULL;
}

SE_IMAGEPLUGIN_EXPORT void DestroyPlugin(Plugin* plugin)
{
	if (plugin != NULL)
	{
		delete plugin;
	}
}

#ifndef SE_IMAGEPLUGIN_DLL_EXPORT
PluginModule::PluginModule()
{
	PluginSymbols symbols;
	symbols.GetPluginDescription = GetPluginDescription;
	symbols.CreatePlugin = CreatePlugin;
	symbols.DestroyPlugin = DestroyPlugin;
	PluginManager::Instance()->RegisterPluginLibrary(symbols);
}
#endif

ImagePlugin_TGA::ImagePlugin_TGA() :
	ImageDataPlugin()
{
}

ImagePlugin_TGA::~ImagePlugin_TGA()
{
}

PluginDescription* ImagePlugin_TGA::GetPluginDescription()
{
	return &g_PluginDescription[0];
}

void ImagePlugin_TGA::Load()
{
}

void ImagePlugin_TGA::Unload()
{
}

Array<String> ImagePlugin_TGA::GetExtensions() const
{
	Array<String> extensions;
	extensions.Add("tga");
	return extensions;
}

bool ImagePlugin_TGA::CanRead() const
{
	return true;
}

bool ImagePlugin_TGA::CanWrite() const
{
	return true;
}

bool ImagePlugin_TGA::CanHandle(const Stream& stream) const
{
	if (stream.GetSourceName().Right(3).Compare(_T("tga"), true) == 0)
		return true;

	return false;
}

ImageReader* ImagePlugin_TGA::CreateReader(const Stream& stream)
{
	return new TGAImageReader();
}

ImageReader* ImagePlugin_TGA::CreateWriter(const Stream& stream)
{
	return NULL;
}

}
