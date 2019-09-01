/*=============================================================================
ManagerPlugin.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_MANAGERPLUGIN_H_
#define _SE_MANAGERPLUGIN_H_

#include "Core/Common.h"
#include "Core/Plugins/Plugin.h"
#include "Core/Manager.h"

namespace SonataEngine
{

#define SE_ID_MANAGER SE_ID(0x65705011,0xdfba4b70)

/** @brief Manager plugin.
	Base class for manager plugin implementations.
	A manager plugin is responsible for creating and destroying managers (subsystems).
*/
class SE_CORE_EXPORT ManagerPlugin : public Plugin
{
public:
	/** Destructor. */
	virtual ~ManagerPlugin();

	virtual SE_ID GetPluginType() const { return SE_ID_MANAGER; }

	/** @name Operations. */
	//@{
	/**
		Returns the SE_ID of the manager type.
		@return The SE_ID of the manager type.
	*/
	virtual SE_ID GetManagerType() const = 0;

	/** Creates the manager. */
	virtual Manager* CreateManager() = 0;

	/** Destroys the manager. */
	virtual void DestroyManager(Manager* manager) = 0;
	//@}

protected:
	/** Constructor. */
	ManagerPlugin();
};

}

#endif 
