/*=============================================================================
XModelPlugin.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_XMODELPLUGIN_H_
#define _SE_XMODELPLUGIN_H_

namespace SE_X
{

#define SE_ID_DATAMODEL_X SonataEngine::SE_ID(0x83cb826b,0xd37041c1)
#define SE_ID_DATAMODEL_SDKMESH SonataEngine::SE_ID(0xa7bef061, 0x5e784100)
#define SE_ID_DATAMODEL_XATG SonataEngine::SE_ID(0xed605bd6,0x3ca648d2)

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
