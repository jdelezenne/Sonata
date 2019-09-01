/*=============================================================================
GLCgRenderSystemLib.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_GLCGRENDERSYSTEMLIB_H_
#define _SE_GLCGRENDERSYSTEMLIB_H_

namespace SE_GLCg
{

#define SE_ID_SHADERSYSTEM_GLCG SonataEngine::SE_ID(0xb97030f3,0x263b4916)

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
