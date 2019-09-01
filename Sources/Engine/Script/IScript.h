/*=============================================================================
IScript.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SCRIPT_ISCRIPT_H_
#define _SE_SCRIPT_ISCRIPT_H_

#include "Core/Core.h"
#include "Script/Common.h"

namespace SonataEngine
{
	namespace Script
	{
		/**
			@brief Base class for physics scene implementations.
		*/
		class SE_SCRIPT_EXPORT IScript : public RefObject
		{
		public:
			virtual ~IScript();

			/** @name Properties. */
			//@{
			/**
				Retrieves the name of the scene.
				@return Name of the scene.
			*/
			String GetName() const { return _Name; }

			/**
				Sets the name of the scene.
				@param value Name of the scene.
			*/
			void SetName(const String& value) { _Name = value; }
			//@}

		protected:
			IScript();

		protected:
			String _Name;
		};

		typedef SmartPtr<IScript> ScriptPtr;
	}
}

#endif
