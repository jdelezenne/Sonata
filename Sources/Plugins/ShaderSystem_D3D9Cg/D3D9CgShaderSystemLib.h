/*=============================================================================
D3D9CgRenderSystemLib.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D9CGRENDERSYSTEMLIB_H_
#define _SE_D3D9CGRENDERSYSTEMLIB_H_

namespace SE_D3D9Cg
{

#define SE_ID_SHADERSYSTEM_D3D9CG SonataEngine::SE_ID(0x1f85335a,0xee6e41a1)

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
