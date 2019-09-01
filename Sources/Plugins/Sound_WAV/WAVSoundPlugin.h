/*=============================================================================
WAVSoundPlugin.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_WAVSOUNDPLUGIN_H_
#define _SE_WAVSOUNDPLUGIN_H_

namespace SE_WAV
{

#define SE_ID_DATASOUND_WAV SonataEngine::SE_ID(0x9e143608,0xc3d1424d)

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
