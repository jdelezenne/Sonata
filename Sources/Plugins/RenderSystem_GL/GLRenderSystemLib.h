/*=============================================================================
GLRenderSystemLib.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_GLRENDERSYSTEMLIB_H_
#define _SE_GLRENDERSYSTEMLIB_H_

#include "Core/Plugins/ManagerPlugin.h"

namespace SE_GL
{

#define SE_ID_RENDERSYSTEM_GL SonataEngine::SE_ID(0xa8b7dfb,0x7a6a6432)
#define SE_ID_SHADERSYSTEM_GLSL SonataEngine::SE_ID(0xef803744,0x8bfb42fd)

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
