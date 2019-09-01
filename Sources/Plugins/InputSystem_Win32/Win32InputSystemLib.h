/*=============================================================================
Win32InputSystemLib.h
Project: Sonata Engine
Copyright (c) 2005
Julien Delezenne
=============================================================================*/

#ifndef _SE_WIN32INPUTSYSTEMLIB_H_
#define _SE_WIN32INPUTSYSTEMLIB_H_

namespace SE_Win32
{

#define SE_ID_INPUTSYSTEM_WIN32 SonataEngine::SE_ID(0xec066c5a,0x155d4675)

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
