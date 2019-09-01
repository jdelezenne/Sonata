/*=============================================================================
NXPhysicsSystemLib.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_NXPHYSICSSYSTEMLIB_H_
#define _SE_NXPHYSICSSYSTEMLIB_H_

namespace SE_NX
{

#define SE_ID_PHYSICSSYSTEM_NX SonataEngine::SE_ID(0xd20af632,0x66754bdd)

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
