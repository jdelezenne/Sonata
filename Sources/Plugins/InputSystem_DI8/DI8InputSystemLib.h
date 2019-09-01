/*=============================================================================
DI8InputSystemLib.h
Project: Sonata Engine
Copyright (c) 2005
Julien Delezenne
=============================================================================*/

#ifndef _SE_pDI8INPUTSYSTEMLIB_H_
#define _SE_pDI8INPUTSYSTEMLIB_H_

namespace SE_pDI8
{

#define SE_ID_INPUTSYSTEM_DI8 SonataEngine::SE_ID(0x3cf17ff4,0x4667694e)

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
