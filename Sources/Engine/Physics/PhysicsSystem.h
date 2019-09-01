/*=============================================================================
PhysicsSystem.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_PHYSICSSYSTEM_H_
#define _SE_PHYSICSSYSTEM_H_

#include "Core/Core.h"
#include "Physics/Common.h"
#include "Physics/IScene.h"

namespace SonataEngine
{
	namespace Physics
	{
		/** Physics system description. */
		struct SE_PHYSICS_EXPORT PhysicsSystemDescription
		{
			PhysicsSystemDescription();
		};

		/**
			@brief Base class for physics system implementations.
		*/
		class SE_PHYSICS_EXPORT PhysicsSystem : public Manager, public Context<PhysicsSystem>
		{
		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			PhysicsSystem();

			/** Destructor. */
			virtual ~PhysicsSystem();
			//@}

			/** @name Properties. */
			//@{
			virtual PhysicsSystemDescription GetDescription() const = 0;
			//@}

			/** @name Creation / Destruction. */
			//@{
			/** Creates the physics system with the specified parameters. */
			virtual bool Create(const PhysicsSystemDescription& desc) = 0;

			/** Destroys the physics system. */
			virtual void Destroy() = 0;
			//@}

			/** @name Operations. */
			//@{
			/** Updates the physics system. */
			virtual void Update(real64 elapsed) = 0;

			/** Creates a physics scene. */
			virtual IScene* CreateScene(const SceneDescription& desc) = 0;

			/** Destroys a physics scene. */
			virtual void DestroyScene(IScene* scene) = 0;
			//@}

			/** @name Events. */
			//@{
			/** Occurs before a simulation update. */
			DefaultEvent PreStep;

			/** Occurs after a simulation update. */
			DefaultEvent PostStep;
			//@}

		public:
			/** Default gravity force: 0 -9.8 0. */
			static const Vector3 DefaultGravity;


		protected:
		};
	}
}

#endif 
