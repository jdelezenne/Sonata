/*=============================================================================
NullPhysicsBody.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "NullPhysicsBody.h"
#include "NullPhysicsMaterial.h"

namespace SE_Null
{

NullPhysicsBody::NullPhysicsBody() :
	Physics::IBody()
{
}

NullPhysicsBody::~NullPhysicsBody()
{
}

BodyType NullPhysicsBody::GetBodyType() const
{
	return BodyType_Dynamic;
}

const Array<ShapePtr>& NullPhysicsBody::Shapes() const
{
	return _Shapes;
}

const AABB& NullPhysicsBody::GetWorldBoundingBox() const
{
	return AABB::Empty;
}

bool NullPhysicsBody::GetEnabled() const
{
	return false;
}

void NullPhysicsBody::SetEnabled(bool value)
{
}

bool NullPhysicsBody::GetActive() const
{
	return false;
}

void NullPhysicsBody::SetActive(bool value)
{
}

bool NullPhysicsBody::GetGravityEnabled() const
{
	return false;
}

void NullPhysicsBody::SetGravityEnabled(bool value)
{
}

BodyGroup NullPhysicsBody::GetGroup() const
{
	return (BodyGroup)0;
}

void NullPhysicsBody::SetGroup(BodyGroup value)
{
}

Vector3 NullPhysicsBody::GetPosition() const
{
	return Vector3::Zero;
}

void NullPhysicsBody::SetPosition(const Vector3& value)
{
}

Quaternion NullPhysicsBody::GetOrientation() const
{
	return Quaternion::Identity;
}

Matrix3 NullPhysicsBody::GetOrientationMatrix() const
{
	return Matrix3::Identity;
}

void NullPhysicsBody::SetOrientation(const Quaternion& value)
{
}

void NullPhysicsBody::SetOrientationMatrix(const Matrix3& value)
{
}

Vector3 NullPhysicsBody::GetCOMLocalPosition() const
{
	return Vector3::Zero;
}

Quaternion NullPhysicsBody::GetCOMLocalOrientation() const
{
	return Quaternion::Identity;
}

Vector3 NullPhysicsBody::GetCOMWorldPosition() const
{
	return Vector3::Zero;
}

void NullPhysicsBody::SetCOMWorldPosition(const Vector3& value)
{
}

Quaternion NullPhysicsBody::GetCOMWorldOrientation() const
{
	return Quaternion::Identity;
}

void NullPhysicsBody::SetCOMWorldOrientation(const Quaternion& value)
{
}

void NullPhysicsBody::SetCOMOffsetLocalPosition(const Vector3& value)
{
}

void NullPhysicsBody::SetCOMOffsetLocalOrientation(const Quaternion& value)
{
}

void NullPhysicsBody::SetCOMOffsetWorldPosition(const Vector3& value)
{
}

void NullPhysicsBody::SetCOMOffsetWorldOrientation(const Quaternion& value)
{
}

real NullPhysicsBody::GetMass() const
{
	return 0.0;
}

void NullPhysicsBody::SetMass(real value)
{
}

Vector3 NullPhysicsBody::GetInertiaTensor() const
{
	return Vector3::Zero;
}

void NullPhysicsBody::SetInertiaTensor(const Vector3& value)
{
}

Matrix3 NullPhysicsBody::GetWorldInertiaTensor() const
{
	return Matrix3::Identity;
}

Matrix3 NullPhysicsBody::GetInverseWorldInertiaTensor() const
{
	return Matrix3::Identity;
}

void NullPhysicsBody::SetMassFromShape(real density, real totalMass)
{
}

real NullPhysicsBody::GetLinearDamping() const
{
	return 0.0;
}

void NullPhysicsBody::SetLinearDamping(real value)
{
}

real NullPhysicsBody::GetAngularDamping() const
{
	return 0.0;
}

void NullPhysicsBody::SetAngularDamping(real value)
{
}

Vector3 NullPhysicsBody::GetLinearVelocity() const
{
	return Vector3::Zero;
}

void NullPhysicsBody::SetLinearVelocity(const Vector3& value)
{
}

Vector3 NullPhysicsBody::GetAngularVelocity() const
{
	return Vector3::Zero;
}

void NullPhysicsBody::SetAngularVelocity(const Vector3& value)
{
}

real NullPhysicsBody::GetMaxAngularVelocity() const
{
	return 0.0;
}

void NullPhysicsBody::SetMaxAngularVelocity(real value)
{
}

Vector3 NullPhysicsBody::GetVelocityAtWorldPosition(const Vector3& value) const
{
	return Vector3::Zero;
}

Vector3 NullPhysicsBody::GetVelocityAtLocalPosition(const Vector3& value) const
{
	return Vector3::Zero;
}

Vector3 NullPhysicsBody::GetLinearMomentum() const
{
	return Vector3::Zero;
}

void NullPhysicsBody::SetLinearMomentum(const Vector3& value)
{
}

Vector3 NullPhysicsBody::GetAngularMomentum() const
{
	return Vector3::Zero;
}

void NullPhysicsBody::SetAngularMomentum(const Vector3& value)
{
}

void NullPhysicsBody::AddForceAtPosition(const Vector3& force, const Vector3& position)
{
}

void NullPhysicsBody::AddForceAtLocalPosition(const Vector3& force, const Vector3& position)
{
}

void NullPhysicsBody::AddLocalForceAtPosition(const Vector3& force, const Vector3& position)
{
}

void NullPhysicsBody::AddLocalForceAtLocalPosition(const Vector3& force, const Vector3& position)
{
}

void NullPhysicsBody::AddForce(const Vector3& force)
{
}

void NullPhysicsBody::AddLocalForce(const Vector3& force)
{
}

void NullPhysicsBody::AddTorque(const Vector3& torque)
{
}

void NullPhysicsBody::AddLocalTorque(const Vector3& torque)
{
}

void NullPhysicsBody::AddImpulseAtPosition(const Vector3& impulse, const Vector3& position)
{
}

void NullPhysicsBody::AddImpulseAtLocalPosition(const Vector3& impulse, const Vector3& position)
{
}

void NullPhysicsBody::AddLocalImpulseAtPosition(const Vector3& impulse, const Vector3& position)
{
}

void NullPhysicsBody::AddLocalImpulseAtLocalPosition(const Vector3& impulse, const Vector3& position)
{
}

void NullPhysicsBody::AddImpulse(const Vector3& impulse)
{
}

void NullPhysicsBody::AddLocalImpulse(const Vector3& impulse)
{
}

void NullPhysicsBody::AddTorqueImpulse(const Vector3& torque)
{
}

void NullPhysicsBody::AddLocalTorqueImpulse(const Vector3& torque)
{
}

void NullPhysicsBody::Update(real64 elapsed)
{
}

}
