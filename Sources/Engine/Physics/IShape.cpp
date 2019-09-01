/*=============================================================================
IShape.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "IShape.h"

namespace SonataEngine
{
	namespace Physics
	{
		IShape::IShape() :
			RefObject(),
			_Group(0),
			_MassDensity(0.0),
			_Material(NULL)
		{
		}

		IShape::~IShape()
		{
		}

		real IShape::GetDensity() const
		{
			if (_MassDensity < 0.0)
				return -_MassDensity;
			else
				return 0.0;
		}

		void IShape::SetDensity(real value)
		{
			_MassDensity = -value;
		}

		real IShape::GetMass() const
		{
			if (_MassDensity > 0.0)
				return _MassDensity;
			else
				return 0.0;
		}
		
		void IShape::SetMass(real value)
		{
			_MassDensity = value;
		}


		IPlaneShape::IPlaneShape() :
			IShape()
		{
		}

		IPlaneShape::~IPlaneShape()
		{
		}

		AABB IPlaneShape::ComputeBoundingBox() const
		{
			return AABB::Empty;
		}

		Vector3 IPlaneShape::ComputeInertiaTensor() const
		{
			return Vector3::Zero;
		}


		IBoxShape::IBoxShape() :
			IShape()
		{
		}

		IBoxShape::~IBoxShape()
		{
		}

		AABB IBoxShape::ComputeBoundingBox() const
		{
			OBB box(Vector3::Zero, Data.GetExtents(), Matrix3::Identity);
			return AABB(box.GetMinimum(), box.GetMaximum());
		}

		Vector3 IBoxShape::ComputeInertiaTensor() const
		{
			real mass = (_MassDensity > 0.0) ? _MassDensity : ComputeMass(Data, -_MassDensity);
			Vector3 extents = Data.GetExtents();
			return Vector3(
				(1.0/12.0) * mass * (extents.Y * extents.Y + extents.Z * extents.Z),
				(1.0/12.0) * mass * (extents.X * extents.X + extents.Z * extents.Z),
				(1.0/12.0) * mass * (extents.X * extents.X + extents.Y * extents.Y));
		}

		real IBoxShape::ComputeMass(const OBB& box, real density)
		{
			if (density > 0.0 && box.GetVolume() > 0.0)
				return (density * box.GetVolume());
			else
				return 0.0;
		}

		real IBoxShape::ComputeDensity(const OBB& box, real mass)
		{
			if (mass > 0.0 && box.GetVolume() > 0.0)
				return (mass / box.GetVolume());
			else
				return 0.0;
		}


		ISphereShape::ISphereShape() :
			IShape()
		{
		}

		ISphereShape::~ISphereShape()
		{
		}

		AABB ISphereShape::ComputeBoundingBox() const
		{
			return AABB(Vector3(-Data.Radius), Vector3(Data.Radius));
		}

		Vector3 ISphereShape::ComputeInertiaTensor() const
		{
			real mass = (_MassDensity > 0.0) ? _MassDensity : ComputeMass(Data, -_MassDensity);
			real radius = Data.Radius;
			return Vector3((2.0/5.0) * mass * radius * radius);
		}

		real ISphereShape::ComputeMass(const BoundingSphere& sphere, real density)
		{
			if (density > 0.0 && sphere.GetVolume() > 0.0)
				return (density * sphere.GetVolume());
			else
				return 0.0;
		}

		real ISphereShape::ComputeDensity(const BoundingSphere& sphere, real mass)
		{
			if (mass > 0.0 && sphere.GetVolume() > 0.0)
				return (mass / sphere.GetVolume());
			else
				return 0.0;
		}


		ICapsuleShape::ICapsuleShape() :
			IShape()
		{
		}

		ICapsuleShape::~ICapsuleShape()
		{
		}

		AABB ICapsuleShape::ComputeBoundingBox() const
		{
			return AABB(
				Vector3(-Data.GetRadius(), -Data.GetHeight() / 2, -Data.GetRadius()),
				Vector3(Data.GetRadius(), Data.GetHeight() / 2, Data.GetRadius()));
		}

		Vector3 ICapsuleShape::ComputeInertiaTensor() const
		{
			real mass = (_MassDensity > 0.0) ? _MassDensity : ComputeMass(Data, -_MassDensity);
			real height = Data.GetHeight();
			real radius = Data.GetRadius();

			return Vector3(
				(1.0/12.0) * mass * (height * height + radius * radius),
				(1.0/12.0) * mass * (radius * radius + radius * radius),
				(1.0/12.0) * mass * (radius * radius + height * height)) +
				Vector3((2.0/5.0) * mass * radius * radius);
		}

		real ICapsuleShape::ComputeMass(const Capsule& capsule, real density)
		{
			if (density > 0.0 && capsule.GetVolume() > 0.0)
				return (density * capsule.GetVolume());
			else
				return 0.0;
		}

		real ICapsuleShape::ComputeDensity(const Capsule& capsule, real mass)
		{
			if (mass > 0.0 && capsule.GetVolume() > 0.0)
				return (mass / capsule.GetVolume());
			else
				return 0.0;
		}


		IMeshShape::IMeshShape() :
			IShape()
		{
		}

		IMeshShape::~IMeshShape()
		{
		}

		AABB IMeshShape::ComputeBoundingBox() const
		{
			//TODO
			return AABB::Empty;
		}

		Vector3 IMeshShape::ComputeInertiaTensor() const
		{
			//TODO
			return Vector3::Zero;
		}

		Vector3 IMeshShape::ComputeCenterOfMass() const
		{
			Vector3 com(Vector3::Zero);
			int count = Vertices.Count();
			for (int i=0; i<count; ++i)
			{
				com += Vertices[i];
			}

			if (count > 0)
			{
				com /= count;
			}

			return com;
		}

		real IMeshShape::ComputeMass(real density)
		{
			//TODO
			return 0.0;
		}

		real IMeshShape::ComputeDensity(real mass)
		{
			//TODO
			return 0.0;
		}
	}
}
