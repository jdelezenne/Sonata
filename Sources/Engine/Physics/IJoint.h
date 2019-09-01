/*=============================================================================
IJoint.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_PHYSICS_IJOINT_H_
#define _SE_PHYSICS_IJOINT_H_

#include "Core/Core.h"
#include "Physics/Common.h"

namespace SonataEngine
{
	namespace Physics
	{
		/**
			@brief Base class for physics joint implementations.
		*/
		class SE_PHYSICS_EXPORT IJoint : public RefObject
		{
		public:
			virtual ~IJoint();

			/** @name Properties. */
			//@{
			/**
				Retrieves the name of the scene.
				@return Name of the scene.
			*/
			String GetName() const { return _name; }

			/**
				Sets the name of the scene.
				@param value Name of the scene.
			*/
			void SetName(const String& value) { _name = value; }
			//@}

			virtual void Update(real64 elapsed) = 0;

		protected:
			IJoint();

		protected:
			String _name;
		};

		typedef SmartPtr<IJoint> JointPtr;
	}
}

#endif
