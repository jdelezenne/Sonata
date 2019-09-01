/*=============================================================================
RWPhysicsSystemLib.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_RWPHYSICSSYSTEMLIB_H_
#define _SE_RWPHYSICSSYSTEMLIB_H_

namespace SE_RW
{

#define SE_ID_PHYSICSSYSTEM_RW SonataEngine::SE_ID(0xb8d5fef2,0x238c4cfc)

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
