/*=============================================================================
Collision.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_PHYSICS_COLLISION_H_
#define _SE_PHYSICS_COLLISION_H_

#include "Core/Core.h"
#include "Physics/Common.h"
#include "Physics/IBody.h"

namespace SonataEngine
{
	namespace Physics
	{
		/**
			@brief Contact pair.
		*/
		class SE_PHYSICS_EXPORT ContactPair : public Object
		{
		public:
			/** Constructor. */
			ContactPair();

			/** @name Properties. */
			//@{
			BodyPtr GetBodyA() const { return _BodyA; }
			void SetBodyA(BodyPtr value) { _BodyA = value; }

			BodyPtr GetBodyB() const { return _BodyB; }
			void SetBodyB(BodyPtr value) { _BodyB = value; }

			real64 GetTime() const { return _Time; }
			void SetTime(real64 value) { _Time = value; }

			Vector3 GetForces() const { return _Forces; }
			void SetForces(Vector3 value) { _Forces = value; }

			Vector3 GetTorques() const { return _Torques; }
			void SetTorques(Vector3 value) { _Torques = value; }
			//@}

		protected:
			BodyPtr _BodyA;
			BodyPtr _BodyB;
			real64 _Time;
			Vector3 _Forces;
			Vector3 _Torques;
		};

		/**
			@brief Contact information.
		*/
		class SE_PHYSICS_EXPORT ContactInfo : public Object
		{
		public:
			/** Constructor. */
			ContactInfo();

			/** @name Properties. */
			//@{
			ShapePtr GetShapeA() const { return _ShapeA; }
			void SetShapeA(ShapePtr value) { _ShapeA = value; }

			ShapePtr GetShapeB() const { return _ShapeB; }
			void SetShapeB(ShapePtr value) { _ShapeB = value; }

			real GetRestitution() const { return _Restitution; }
			void SetRestitution(real value) { _Restitution = value; }

			MaterialIndex GetMaterialA() const { return _MaterialA; }
			void SetMaterialA(MaterialIndex value) { _MaterialA = value; }

			MaterialIndex GetMaterialB() const { return _MaterialB; }
			void SetMaterialB(MaterialIndex value) { _MaterialB = value; }

			Vector3 GetPosition() const { return _Position; }
			void SetPosition(Vector3 value) { _Position = value; }

			Vector3 GetNormal() const { return _Normal; }
			void SetNormal(Vector3 value) { _Normal = value; }

			real GetSeparation() const { return _Separation; }
			void SetSeparation(real value) { _Separation = value; }
			//@}

		protected:
			ShapePtr _ShapeA;
			ShapePtr _ShapeB;
			real _Restitution;
			real _Separation;
			MaterialIndex _MaterialA;
			MaterialIndex _MaterialB;
			Vector3 _Position;
			Vector3 _Normal;
		};
	}
}

#endif
