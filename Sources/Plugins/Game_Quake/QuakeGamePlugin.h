/*=============================================================================
QuakeGamePlugin.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_QUAKEGAMEPLUGIN_H_
#define _SE_QUAKEGAMEPLUGIN_H_

namespace SE_Quake
{

#define SE_ID_DATAMODEL_MDL SonataEngine::SE_ID(0x79e9f8f7,0xc8004c29)
#define SE_ID_DATAMODEL_MD2 SonataEngine::SE_ID(0x124b422e,0x33ca4533)
#define SE_ID_DATAMODEL_MD3 SonataEngine::SE_ID(0xf37080b3,0x4c7a4305)
#define SE_ID_DATASCENE_BSP SonataEngine::SE_ID(0x5a2bb369,0x6ad94ae8)

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
