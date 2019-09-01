/*=============================================================================
D3D8RenderSystemLib.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D8RENDERSYSTEMLIB_H_
#define _SE_D3D8RENDERSYSTEMLIB_H_

namespace SE_D3D8
{

#define SE_ID_RENDERSYSTEM_D3D8 SonataEngine::SE_ID(0x6DC4B2C2,0x8B1C40d2)
#define SE_ID_SHADERSYSTEM_D3D8 SonataEngine::SE_ID(0x9AE96625,0xC33D455a)

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
