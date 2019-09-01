/*=============================================================================
D3D8CgRenderSystemLib.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D8CGRENDERSYSTEMLIB_H_
#define _SE_D3D8CGRENDERSYSTEMLIB_H_

namespace SE_D3D8Cg
{

#define SE_ID_SHADERSYSTEM_D3D8CG SonataEngine::SE_ID(0xe6def879,0xb5d04c92)

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
