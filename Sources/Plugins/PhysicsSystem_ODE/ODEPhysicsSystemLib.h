/*=============================================================================
ODEPhysicsSystemLib.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ODEPHYSICSSYSTEMLIB_H_
#define _SE_ODEPHYSICSSYSTEMLIB_H_

namespace SE_ODE
{

#define SE_ID_PHYSICSSYSTEM_ODE SonataEngine::SE_ID(0x44628672,0x8b404788)

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
