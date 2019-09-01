/*=============================================================================
SoftwarePhysicsSystemLib.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SOFTWAREPHYSICSSYSTEMLIB_H_
#define _SE_SOFTWAREPHYSICSSYSTEMLIB_H_

namespace SE_Software
{

#define SE_ID_PHYSICSSYSTEM_SOFTWARE SonataEngine::SE_ID(0xdf45a724,0x984a4c42)

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
