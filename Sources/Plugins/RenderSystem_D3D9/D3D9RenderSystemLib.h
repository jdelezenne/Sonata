/*=============================================================================
D3D9RenderSystemLib.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D9RENDERSYSTEMLIB_H_
#define _SE_D3D9RENDERSYSTEMLIB_H_

namespace SE_D3D9
{

#define SE_ID_RENDERSYSTEM_D3D9 SonataEngine::SE_ID(0x37e123b3,0x60805177)
#define SE_ID_SHADERSYSTEM_HLSL9 SonataEngine::SE_ID(0x9c4b3cec,0x6b884d97)

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
