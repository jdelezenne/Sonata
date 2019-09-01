/*=============================================================================
Image_TGA.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _IMAGE_TGA_H_
#define _IMAGE_TGA_H_

#include <Core/Plugins/ImageDataPlugin.h>
using namespace SonataEngine;

namespace SE_ImagePlugin_TGA
{

#define SE_ID_DATAIMAGE_TGA     SonataEngine::SE_ID(0x514c6b30,0xe0f74db1)

#ifdef SE_IMAGEPLUGIN_DLL_EXPORT
	// DLL library.
	#define SE_IMAGEPLUGIN_EXPORT __declspec(dllexport)
#else
	// Client of the DLL library.
	#ifdef SE_IMAGEPLUGIN_DLL_IMPORT
		#define SE_IMAGEPLUGIN_EXPORT __declspec(dllimport)
	#else
		// Static library.
		#define SE_IMAGEPLUGIN_EXPORT
	#endif
#endif 

#ifndef SE_IMAGEPLUGIN_DLL_EXPORT
class PluginModule
{
public:
	PluginModule();
};
#endif

#ifdef SE_STATIC
PluginModule g_PluginModule;
#endif

class ImagePlugin_TGA : public ImageDataPlugin
{
public:
	ImagePlugin_TGA();
	virtual ~ImagePlugin_TGA();

	virtual PluginDescription* GetPluginDescription();
	virtual void Load();
	virtual void Unload();

	virtual Array<String> GetExtensions() const;
	virtual bool CanRead() const;
	virtual bool CanWrite() const;
	virtual bool CanHandle(const Stream& stream) const;

	virtual ImageReader* CreateReader(const Stream& stream);
	virtual ImageReader* CreateWriter(const Stream& stream);
};

}

#endif 
