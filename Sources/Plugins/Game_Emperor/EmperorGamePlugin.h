/*=============================================================================
EmperorGamePlugin.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_EMPERORGAMEPLUGIN_H_
#define _SE_EMPERORGAMEPLUGIN_H_

namespace SE_Emperor
{

#define SE_ID_DATAMODEL_EMPEROR SonataEngine::SE_ID(0xe4a91d96,0xbe624d3a)
//#define SE_ID_DATAMODEL_XAF SonataEngine::SE_ID(0xe4a91d96,0xbe624d3a)
//#define SE_ID_DATAMODEL_XBF SonataEngine::SE_ID(0xcda4169e,0x1ccf4eb8)

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
