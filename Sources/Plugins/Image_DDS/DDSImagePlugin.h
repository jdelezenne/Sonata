/*=============================================================================
DDSImagePlugin.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_DDSIMAGEPLUGIN_H_
#define _SE_DDSIMAGEPLUGIN_H_

namespace SE_DDS
{

#define SE_ID_DATAIMAGE_DDS SonataEngine::SE_ID(0xfe915f7a,0xe91b46ca)

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
