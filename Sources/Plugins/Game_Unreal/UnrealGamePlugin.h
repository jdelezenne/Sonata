/*=============================================================================
UnrealGamePlugin.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UNREALGAMEPLUGIN_H_
#define _SE_UNREALGAMEPLUGIN_H_

namespace SE_Unreal
{

#define SE_ID_DATAMODEL_PSK SonataEngine::SE_ID(0x8910527f,0x9b4c41a2)

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
