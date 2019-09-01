/*=============================================================================
IBody.h
Project: Sonata Engine
Copyright(c) by
Julien Delezenne
=============================================================================*/

#ifndef _SE_PHYSICS_IBODY_H_
#define _SE_PHYSICS_IBODY_H_

#include "Core/Core.h"
#include "Physics/Common.h"
#include "Physics/IShape.h"

namespace SonataEngine
{
	namespace Physics
	{
		class ContactEventArgs;

		enum IntegrationType
		{
			IntegrationType_Default,
			IntegrationType_Euler,
			IntegrationType_Midpoint,
			IntegrationType_RK4
		};

		/** Physics system description. */
		struct SE_PHYSICS_EXPORT BodyDescription
		{
			BodyDescription() :
				_BodyType(BodyType_Static),
				_Enabled(true),
				_MassDensity(0.0),
				_Active(true),
				_GravityEnabled(true)
			{
			}

			/** Density of the body. */
			real GetDensity() const;
			void SetDensity(real value);

			/** Mass of the body. */
			real GetMass() const;
			void SetMass(real value);

			/** Type of the body. */
			BodyType _BodyType;

			/** Shapes composing the body. */
			List<ShapePtr> _Shapes;

			/** Whether the body is enabled. */
			bool _Enabled;

			/** Whether the body is active. */
			bool _Active;

			/** Whether the body is affected by gravity. */
			bool _GravityEnabled;

		protected:
			real _MassDensity;
		};

		/**
			@brief Base class for physics body implementations.
		*/
		class SE_PHYSICS_EXPORT IBody : public RefObject, public IUserData
		{
		public:
			virtual ~IBody();

			/** @name Properties. */
			//@{
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

			/** Type of the body. */
			virtual BodyType GetBodyType() const = 0;

			/** Shapes composing the body. */
			virtual const List<ShapePtr>& Shapes() const = 0;

			/** World bounding box of the body. */
			virtual const AABB& GetWorldBoundingBox() const = 0;

			virtual IntegrationType GetIntegrationType() const { return IntegrationType_Default; }
			virtual void SetIntegrationType(IntegrationType value) {}
			//@}

			/** @name Frame. */
			//@{
			/** Retrieves the world space position of the body. */
			virtual Vector3 GetPosition() const = 0;

			/** Sets the world space position of the body. */
			virtual void SetPosition(const Vector3& value) = 0;

			/** Retrieves the world space orientation of the body. */
			virtual Quaternion GetOrientation() const = 0;
			virtual Matrix3 GetOrientationMatrix() const = 0;

			/** Sets the world space orientation of the body. */
			virtual void SetOrientation(const Quaternion& value) = 0;
			virtual void SetOrientationMatrix(const Matrix3& value) = 0;
			//@}

			/** @name State. */
			//@{
			/** Whether the body is enabled. */
			virtual bool GetEnabled() const = 0;
			virtual void SetEnabled(bool value) = 0;

			/** Whether the body is enabled. */
			virtual bool GetActive() const = 0;
			virtual void SetActive(bool value) = 0;

			/** Whether the body is affected by gravity. */
			virtual bool GetGravityEnabled() const = 0;
			virtual void SetGravityEnabled(bool value) = 0;
			//@}

			/** @name Group. */
			//@{
			/** The group of the body. */
			virtual BodyGroup GetGroup() const = 0;
			virtual void SetGroup(BodyGroup value) = 0;
			//@}

			/** @name Center of Mass. */
			//@{
			/** Retrieves the center of mass position relative to the body. */
			virtual Vector3 GetCOMLocalPosition() const = 0;

			/** Retrieves the center of mass orientation relative to the body. */
			virtual Quaternion GetCOMLocalOrientation() const = 0;

			/** Retrieves the center of mass position in world space. */
			virtual Vector3 GetCOMWorldPosition() const = 0;

			/** Moves the body by setting the position of the center of mass. */
			virtual void SetCOMWorldPosition(const Vector3& value) = 0;

			/** Retrieves the center of mass orientation in world space. */
			virtual Quaternion GetCOMWorldOrientation() const = 0;

			/** Moves the body by setting the orientation of the center of mass. */
			virtual void SetCOMWorldOrientation(const Quaternion& value) = 0;

			/** Sets the position of the center of mass relative to the body. */
			virtual void SetCOMOffsetLocalPosition(const Vector3& value) = 0;

			/** Sets the orientation of the center of mass relative to the body. */
			virtual void SetCOMOffsetLocalOrientation(const Quaternion& value) = 0;

			/** Sets the position of the center of mass relative to world space. */
			virtual void SetCOMOffsetWorldPosition(const Vector3& value) = 0;

			/** Sets the orientation of the center of mass relative to world space. */
			virtual void SetCOMOffsetWorldOrientation(const Quaternion& value) = 0;
			//@}

			/** @name Mass / Intertia. */
			//@{
			/** Retrieves the mass of the body. */
			virtual real GetMass() const = 0;

			/** Sets the mass of the body. */
			virtual void SetMass(real value) = 0;

			/** Retrieves the diagonal inertia tensor of the body relative
			to the mass coordinate frame. */
			virtual Vector3 GetInertiaTensor() const = 0;

			/** Sets the inertia tensor, using a parameter specified in
			mass space coordinates. */
			virtual void SetInertiaTensor(const Vector3& value) = 0;

			/** Retrieves the inertia tensor of the body relative to the
			world coordinate frame. */
			virtual Matrix3 GetWorldInertiaTensor() const = 0;

			/** Retrieves the inverse of the inertia tensor of the body
			relative to the world coordinate frame. */
			virtual Matrix3 GetInverseWorldInertiaTensor() const = 0;

			/** Recomputes the mass of a body from its shape. */
			virtual void SetMassFromShape(real density, real totalMass) = 0;
			//@}

			/** @name Damping. */
			//@{
			/** Retrieves the linear damping coefficient. */
			virtual real GetLinearDamping() const = 0;

			/** Sets the linear damping coefficient. */
			virtual void SetLinearDamping(real value) = 0;

			/** Retrieves the angular damping coefficient. */
			virtual real GetAngularDamping() const = 0;

			/** Sets the angular damping coefficient. */
			virtual void SetAngularDamping(real value) = 0;
			//@}

			/** @name Velocity. */
			//@{
			/** Retrieves the linear velocity of an body. */
			virtual Vector3 GetLinearVelocity() const = 0;

			/** Sets the linear velocity of the body. */
			virtual void SetLinearVelocity(const Vector3& value) = 0;

			/** Retrieves the angular velocity of the body. */
			virtual Vector3 GetAngularVelocity() const = 0;

			/** Sets the angular velocity of the body. */
			virtual void SetAngularVelocity(const Vector3& value) = 0;

			/** Retrieves the maximum angular velocity permitted for the body. */
			virtual real GetMaxAngularVelocity() const = 0;

			/** Sets the maximum angular velocity permitted for the body. */
			virtual void SetMaxAngularVelocity(real value) = 0;

			/** Computes the velocity of a point given in world coordinates
			if it were attached to the body and moving with it. */
			virtual Vector3 GetVelocityAtWorldPosition(const Vector3& value) const = 0;
			 
			/** Computes the velocity of a point given in body local
			coordinates if it were attached to the body and moving with it. */
			virtual Vector3 GetVelocityAtLocalPosition(const Vector3& value) const = 0;
			//@}
			 
			/** @name Momentum. */
			//@{
			/** Retrieves the linear momentum of an body. */
			virtual Vector3 GetLinearMomentum() const = 0;

			/** Sets the linear momentum of the body. */
			virtual void SetLinearMomentum(const Vector3& value) = 0;

			/** Retrieves the angular velocity of an body. */
			virtual Vector3 GetAngularMomentum() const = 0;

			/** Sets the angular momentum of the body. */
			virtual void SetAngularMomentum(const Vector3& value) = 0;
  			//@}

			/** @name Forces. */
			//@{
			/** Applies a force defined in the world coordinate frame,
			acting at a particular point in world coordinates, to the body. */
			virtual void AddForceAtPosition(const Vector3& force, const Vector3& position) = 0;

			/** Applies a force defined in the world coordinate frame,
			acting at a particular point in local coordinates, to the body. */
			virtual void AddForceAtLocalPosition(const Vector3& force, const Vector3& position) = 0;

			/** Applies a force defined in the body local coordinate frame,
			acting at a particular point in world coordinates, to the body. */
			virtual void AddLocalForceAtPosition(const Vector3& force, const Vector3& position) = 0;

			/** Applies a force defined in the body local coordinate frame,
			acting at a particular point in local coordinates, to the body. */
			virtual void AddLocalForceAtLocalPosition(const Vector3& force, const Vector3& position) = 0;

			/** Applies a force defined in the world coordinate frame to the body. */
			virtual void AddForce(const Vector3& force) = 0;

			/** Applies a force defined in the body local coordinate
			frame to the body. */
			virtual void AddLocalForce(const Vector3& force) = 0;

			/** Applies an impulsive torque defined in the world
			coordinate frame to the body. */
			virtual void AddTorque(const Vector3& torque) = 0;

			/** Applies an impulsive torque defined in the body local
			coordinate frame to the body. */
			virtual void AddLocalTorque(const Vector3& torque) = 0;

			virtual void AddImpulseAtPosition(const Vector3& impulse, const Vector3& position) = 0;
			virtual void AddImpulseAtLocalPosition(const Vector3& impulse, const Vector3& position) = 0;
			virtual void AddLocalImpulseAtPosition(const Vector3& impulse, const Vector3& position) = 0;
			virtual void AddLocalImpulseAtLocalPosition(const Vector3& impulse, const Vector3& position) = 0;
			virtual void AddImpulse(const Vector3& impulse) = 0;
			virtual void AddLocalImpulse(const Vector3& impulse) = 0;
			virtual void AddTorqueImpulse(const Vector3& torque) = 0;
			virtual void AddLocalTorqueImpulse(const Vector3& torque) = 0;
  			//@}

			virtual void Update(real64 elapsed) = 0;

			/** @name Events. */
			//@{
			/** Occurs when two bodies are in contact. */
			Event<ContactEventArgs> Contact;
			//@}

		protected:
			IBody();

		protected:
			String _name;
		};

		typedef SmartPtr<IBody> BodyPtr;
	}
}

#endif
