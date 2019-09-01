/*=============================================================================
SDLImagePlugin.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SDLIMAGEPLUGIN_H_
#define _SDLIMAGEPLUGIN_H_

#include <Graphics/ImageDataPlugin.h>
using namespace SonataEngine;

namespace SE_SDLImage
{

#define SE_ID_DATAIMAGE_SDL SonataEngine::SE_ID(0x652a9ef5,0x63c34bae)

#ifdef SE_STATIC
class PluginModule
{
public:
	PluginModule();
};

#ifndef SE_LIB
PluginModule g_PluginModule;
#endif

#endif

}

#endif 
