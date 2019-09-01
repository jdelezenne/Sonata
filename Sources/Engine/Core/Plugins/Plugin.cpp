/*=============================================================================
Plugin.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Plugin.h"
#include "Core/Plugins/PluginManager.h"

namespace SonataEngine
{

Plugin::Plugin() :
	_Enabled(false)
{
}

Plugin::~Plugin()
{
}

SE_ID Plugin::GetPluginID() const
{
	PluginDescription* desc = GetPluginDescription();
	if (desc == NULL)
		return SE_ID::Null;

	return desc->_Type;
}

}