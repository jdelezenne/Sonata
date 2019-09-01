/*=============================================================================
OBJModelPlugin.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_OBJMODELPLUGIN_H_
#define _SE_OBJMODELPLUGIN_H_

namespace SE_OBJ
{

#define SE_ID_DATAMODEL_OBJ SonataEngine::SE_ID(0x6caebfc6,0x39c94530)

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
