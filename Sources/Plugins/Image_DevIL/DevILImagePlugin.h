/*=============================================================================
DevILImagePlugin.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_DEVILIMAGEPLUGIN_H_
#define _SE_DEVILIMAGEPLUGIN_H_

namespace SE_DevIL
{

#define SE_ID_DATAIMAGE_DEVIL SonataEngine::SE_ID(0x125aaecb,0x13384c46)

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
