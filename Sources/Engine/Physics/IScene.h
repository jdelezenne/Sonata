/*=============================================================================
IScene.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_PHYSICS_ISCENE_H_
#define _SE_PHYSICS_ISCENE_H_

#include "Core/Core.h"
#include "Physics/Common.h"
#include "Physics/IBody.h"
#include "Physics/Collision.h"

namespace SonataEngine
{
	namespace Physics
	{
		class IMaterial;
		class IJoint;

		/** Provides data for the contact events. */
		class SE_PHYSICS_EXPORT ContactEventArgs : public EventArgs
		{
		public:
			ContactEventArgs(const ContactInfo& info) :
				EventArgs(),
				_Info(info)
			{
			}

			/** Gets the contact information. */
			ContactInfo Info() const { return _Info; }

		protected:
			ContactInfo _Info;
		};

		/** Scene description. */
		struct SE_PHYSICS_EXPORT SceneDescription
		{
			SceneDescription();

			/** Determines whether the broad phase algorithm is enabled. */
			bool _BroadPhase;

			/** Determines whether the collision detection is enabled. */
			bool _CollisionDetection;

			/** Gravity force. */
			Vector3 _Gravity;

			/** Time integration method. */
			TimeStepType _TimeStepType;

			/** Maximum time step value if the integration method is fixed. */
			real _TimeStep;

			/** Maximum number of substeps to take. */
			uint32 _Iterations;

			/** Maximum scene bounds. */
			AABB _Bounds;

			/** Maximum number of bodies. */
			uint32 _MaxBodies;

			/** Maximum number of joints. */
			uint32 _MaxJoints;
		};

		/**
			@brief Base class for physics scene implementations.
		*/
		class SE_PHYSICS_EXPORT IScene : public RefObject
		{
		public:
			virtual ~IScene();

			/** @name Properties. */
			//@{
			virtual SceneDescription GetDescription() const = 0;

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

			/**
				Retrieves the constant gravity for the entire scene.
				@return Value of the gravity force.
			*/
			virtual Vector3 GetGravity() const = 0;

			/**
				Sets a constant gravity for the entire scene.
				@param value Value of the gravity force.
			*/
			virtual void SetGravity(const Vector3& value) = 0;

			virtual TimeStepType GetTimeStepType() const = 0;
			virtual void SetTimeStepType(TimeStepType value) = 0;

			virtual real GetTimeStep() const = 0;
			virtual void SetTimeStep(real value) = 0;

			virtual uint32 GetIterations() const = 0;
			virtual void SetIterations(uint32 value) = 0;
			//@}

			/** @name Operations. */
			//@{
			/** Creates a physics body. */
			virtual IBody* CreateBody(const BodyDescription& desc) = 0;

			/** Destroys a physics body. */
			virtual void DestroyBody(IBody* body) = 0;

			virtual int GetBodyCount() const = 0;
			virtual IBody* GetBody(int index) const = 0;

			/** Creates a physics material. */
			virtual IMaterial* CreateMaterial() = 0;

			/** Destroys a physics material. */
			virtual void DestroyMaterial(MaterialIndex material) = 0;

			virtual IMaterial* GetMaterial(MaterialIndex index) const = 0;

			/** Retrieves the default material. */
			virtual MaterialIndex GetDefaultMaterial() const = 0;

			/** Sets the default material. */
			virtual void SetDefaultMaterial(MaterialIndex value) = 0;

			/** Creates a physics joint. */
			virtual IJoint* CreateJoint() = 0;

			/** Destroys a physics joint. */
			virtual void DestroyJoint(IJoint* joint) = 0;

			/** Updates the physics scene. */
			virtual void Update(real64 elapsed) = 0;
			//@}

			/** @name Events. */
			//@{
			/** Occurs when two bodies are in contact. */
			Event<ContactEventArgs> PreContact;

			/** Occurs after the resolution of two bodies in contact. */
			Event<ContactEventArgs> PostContact;

			/**
				Occurs when a breakable joint breaks.
				@todo EventArgs: bool Cancel, real BreakingForce, IJoint* BrokenJoint.
			*/
			DefaultEvent JointBreak;
			//@}

		protected:
			IScene();

		protected:
			String _name;
		};

		SEPointer(IScene);
	}
}

#endif
