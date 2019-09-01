/*=============================================================================
IShape.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_PHYSICS_ISHAPE_H_
#define _SE_PHYSICS_ISHAPE_H_

#include "Core/Core.h"
#include "Physics/Common.h"
#include "Physics/IMaterial.h"

namespace SonataEngine
{
	namespace Physics
	{
		class IBody;

		/**
			@brief Base class for physics shape implementations.

			Each shape needs to implemented the ComputeBoundingBox and ComputeInertiaTenso methods.
			Each shape defines its own ComputeMass and ComputeDensity methods.

			@remarks
				The intertia tensors ae computed using the equations from
				"Physics Modeling for Game Programmers", Premier Press, David Congers, 2003, page 238.

			@todo Add shapes: Cylinder, Cone, ConvexHull.
		*/
		class SE_PHYSICS_EXPORT IShape : public RefObject
		{
		public:
			virtual ~IShape();

			virtual ShapeType GetShapeType() const = 0;

			/** @name Properties. */
			//@{
			/**
				Retrieves the name of the shape.
				@return Name of the shape.
			*/
			String GetName() const { return _name; }

			/**
				Sets the name of the shape.
				@param value Name of the shape.
			*/
			void SetName(const String& value) { _name = value; }

			/** The group of the shape. */
			virtual ShapeGroup GetGroup() const { return _Group; }
			virtual void SetGroup(ShapeGroup value) { _Group = value; }

			/** Transformation of the shape. */
			Matrix4 GetTransform() const { return _Transform; }
			void SetTransform(Matrix4 value) { _Transform = value; }

			/** Density of the shape. */
			real GetDensity() const;
			void SetDensity(real value);

			/** Mass of the shape. */
			real GetMass() const;
			void SetMass(real value);

			MaterialPtr GetMaterial() const { return _Material; }
			void SetMaterial(MaterialPtr value) { _Material = value; }

			IBody* GetBody() const { return _Body; }
			void SetBody(IBody* value) { _Body = value; }
			//@}

			/** @name Operations. */
			//@{
			/** Computes the bounding box of the shape.*/
			virtual AABB ComputeBoundingBox() const = 0;

			/**
				Computes the intertia tensor of the shape.
				The inertia tensor is computed from the density or the mass.
			*/
			virtual Vector3 ComputeInertiaTensor() const = 0;
			//@}

		protected:
			IShape();

		protected:
			String _name;
			ShapeGroup _Group;
			Matrix4 _Transform;
			real _MassDensity;
			MaterialPtr _Material;
			IBody* _Body;
		};

		typedef SmartPtr<IShape> ShapePtr;

		/** Plane shape. */
		class SE_PHYSICS_EXPORT IPlaneShape : public IShape
		{
		public:
			IPlaneShape();

			virtual ~IPlaneShape();

			virtual ShapeType GetShapeType() const { return ShapeType_Plane; }

			virtual AABB ComputeBoundingBox() const;
			virtual Vector3 ComputeInertiaTensor() const;

		public:
			Plane Data;
		};

		/** Box shape. */
		class SE_PHYSICS_EXPORT IBoxShape : public IShape
		{
		public:
			IBoxShape();

			virtual ~IBoxShape();

			virtual ShapeType GetShapeType() const { return ShapeType_Box; }

			virtual AABB ComputeBoundingBox() const;

			/**
				Computes the intertia tensor of the box.

				@remarks
					I = mass * (width*width + height*height + depth*depth) / 12.0;
			*/
			virtual Vector3 ComputeInertiaTensor() const;

			/** Computes the mass from the density. */
			static real ComputeMass(const OBB& box, real density);

			/** Computes the density from the mass. */
			static real ComputeDensity(const OBB& box, real mass);

		public:
			OBB Data;
		};

		/** Sphere shape. */
		class SE_PHYSICS_EXPORT ISphereShape : public IShape
		{
		public:
			ISphereShape();

			virtual ~ISphereShape();

			virtual ShapeType GetShapeType() const { return ShapeType_Sphere; }

			virtual AABB ComputeBoundingBox() const;

			/**
				Computes the intertia tensor of the sphere.

				@remarks
					I = 2.0 * mass * (radius*radius) / 5.0;
			*/
			virtual Vector3 ComputeInertiaTensor() const;

			/** Computes the mass from the density. */
			static real ComputeMass(const BoundingSphere& sphere, real density);

			/** Computes the density from the mass. */
			static real ComputeDensity(const BoundingSphere& sphere, real mass);

		public:
			BoundingSphere Data;
		};

		/** Capsule shape. */
		class SE_PHYSICS_EXPORT ICapsuleShape : public IShape
		{
		public:
			ICapsuleShape();

			virtual ~ICapsuleShape();

			virtual ShapeType GetShapeType() const { return ShapeType_Capsule; }

			virtual AABB ComputeBoundingBox() const;

			/**
				Computes the intertia tensor of the capsule.

				@remarks
					I = I(sphere(radius)) + I(box(radius, height, radius));
			*/
			virtual Vector3 ComputeInertiaTensor() const;

			/** Computes the mass from the density. */
			static real ComputeMass(const Capsule& capsule, real density);

			/** Computes the density from the mass. */
			static real ComputeDensity(const Capsule& capsule, real mass);

		public:
			Capsule Data;
		};

		/** Mesh shape. */
		class SE_PHYSICS_EXPORT IMeshShape : public IShape
		{
		public:
			struct Triangle
			{
				uint32 v0, v1, v2;
			};

			IMeshShape();

			virtual ~IMeshShape();

			virtual ShapeType GetShapeType() const { return ShapeType_Mesh; }

			virtual AABB ComputeBoundingBox() const;
			virtual Vector3 ComputeInertiaTensor() const;
			Vector3 ComputeCenterOfMass() const;

			/** Computes the mass from the density. */
			static real ComputeMass(real density);

			/** Computes the density from the mass. */
			static real ComputeDensity(real mass);

		public:
			Array<Vector3> Vertices;
			BaseArray<Triangle> Triangles;
			Array<uint32> MaterialIndices;
		};
	}
}

#endif
