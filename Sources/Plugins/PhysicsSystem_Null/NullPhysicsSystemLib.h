/*=============================================================================
NullPhysicsSystemLib.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_NULLPHYSICSSYSTEMLIB_H_
#define _SE_NULLPHYSICSSYSTEMLIB_H_

namespace SE_Null
{

#define SE_ID_PHYSICSSYSTEM_NULL SonataEngine::SE_ID(0x513264a7,0x57c5414e)

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
