/*=============================================================================
DS8AudioSystemLib.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_DS8AUDIOSYSTEMLIB_H_
#define _SE_DS8AUDIOSYSTEMLIB_H_

namespace SE_DS8
{

#define SE_ID_AUDIOSYSTEM_DS8 SonataEngine::SE_ID(0xc85e9ee1,0x36c84abe)

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
