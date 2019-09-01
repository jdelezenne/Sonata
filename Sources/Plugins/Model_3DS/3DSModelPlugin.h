/*=============================================================================
3DSModelPlugin.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_3DSMODELPLUGIN_H_
#define _SE_3DSMODELPLUGIN_H_

namespace SE_3DS
{

#define SE_ID_DATAMODEL_3DS SonataEngine::SE_ID(0x99fb4c44,0xa2d8422a)

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
