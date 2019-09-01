/*=============================================================================
BMPImagePlugin.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_BMPIMAGEPLUGIN_H_
#define _SE_BMPIMAGEPLUGIN_H_

namespace SE_BMP
{

#define SE_ID_DATAIMAGE_BMP SonataEngine::SE_ID(0x642f2227,0x329b48cc)

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
