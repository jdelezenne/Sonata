/*=============================================================================
WestwoodGamePlugin.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_WESTWOODGAMEPLUGIN_H_
#define _SE_WESTWOODGAMEPLUGIN_H_

namespace SE_Westwood
{

#define SE_ID_DATAMODEL_W3D SonataEngine::SE_ID(0x8628129b,0x5e314bfa)

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
