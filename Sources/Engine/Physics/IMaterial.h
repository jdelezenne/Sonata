/*=============================================================================
IMaterial.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_PHYSICS_IMATERIAL_H_
#define _SE_PHYSICS_IMATERIAL_H_

#include "Core/Core.h"
#include "Physics/Common.h"

namespace SonataEngine
{
	namespace Physics
	{
		/**
			@brief Base class for physics material implementations.
		*/
		class SE_PHYSICS_EXPORT IMaterial : public RefObject
		{
		public:
			virtual ~IMaterial();

			/** @name Properties. */
			//@{
			/** The material index. */
			MaterialIndex GetIndex() const { return _Index; }
			void SetIndex(MaterialIndex value) { _Index = value; }

			/**
				Retrieves the name of the body.
				@return Name of the body.
			*/
			String GetName() const { return _name; }

			/**
				Sets the name of the body.
				@param value Name of the body.
			*/
			void SetName(const String& value) { _name = value; }

			/**
				Retrieves the coefficient of dynamic friction.
				@return Coefficient of dynamic friction.
			*/
			real GetDynamicFriction() const { return _DynamicFriction; }

			/**
				Sets the coefficient of dynamic friction.
				@param value Coefficient of dynamic friction.
			*/
			void SetDynamicFriction(real value) { _DynamicFriction = value; }

			/**
				Retrieves the coefficient of static friction.
				@return Coefficient of static friction.
			*/
			real GetStaticFriction() const { return _StaticFriction; }

			/**
				Sets the coefficient of static friction.
				@param value Coefficient of static friction.
			*/
			void SetStaticFriction(real value) { _StaticFriction = value; }

			/**
				Retrieves the coefficient of restitution.
				@return Coefficient of restitution.
			*/
			real GetRestitution() const { return _Restitution; }

			/**
				Sets the coefficient of restitution.
				@param value Coefficient of restitution.
			*/
			void SetRestitution(real value) { _Restitution = value; }

			/**
				Retrieves whether the friction computations are enabled.
				@return true if the friction computations are enabled; otherwise, false.
			*/
			bool GetFrictionEnabled() const { return _FrictionEnabled; }

			/**
				Sets whether the friction computations are enabled.
				@param value true to enable the friction computations; otherwise, false.
			*/
			void SetFrictionEnabled(bool value) { _FrictionEnabled = value; }
			//@}

			virtual void Apply() = 0;

		protected:
			IMaterial();

		protected:
			MaterialIndex _Index;
			String _name;
			real _DynamicFriction;
			real _StaticFriction;
			real _Restitution;
			bool _FrictionEnabled;
		};

		typedef SmartPtr<IMaterial> MaterialPtr;
	}
}

#endif
