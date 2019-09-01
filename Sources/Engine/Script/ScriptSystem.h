/*=============================================================================
ScriptSystem.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SCRIPTSYSTEM_H_
#define _SE_SCRIPTSYSTEM_H_

#include "Core/Core.h"
#include "Script/Common.h"
#include "Script/Script.h"

namespace SonataEngine
{
	namespace Script
	{
		/**
			@brief Base class for physics system implementations.
		*/
		class SE_SCRIPT_EXPORT ScriptSystem : public Manager, public Context<ScriptSystem>
		{
		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			ScriptSystem();

			/** Destructor. */
			virtual ~ScriptSystem();
			//@}

			/** @name Properties. */
			//@{
			//@}

			/** @name Creation / Destruction. */
			//@{
			/** Creates the script system. */
			virtual bool Create() = 0;

			/** Destroys the script system. */
			virtual void Destroy() = 0;
			//@}

			/** @name Operations. */
			//@{
			/** Creates a script. */
			virtual IScript* CreateScript() = 0;

			/** Destroys a script. */
			virtual void DestroyScene(Script* script) = 0;
			//@}

		protected:
		};
	}
}

#endif 
