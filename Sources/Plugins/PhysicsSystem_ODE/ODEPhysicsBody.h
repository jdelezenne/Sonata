/*=============================================================================
ODEPhysicsBody.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ODEPHYSICSBODY_H_
#define _SE_ODEPHYSICSBODY_H_

#include "ODEPhysicsSystem.h"
#include "ODEPhysicsScene.h"

namespace SE_ODE
{

/** ODE Body. */
class ODEPhysicsBody : public Physics::IBody
{
public:
	ODEPhysicsBody(ODEPhysicsScene* scene, const BodyDescription& desc);
	virtual ~ODEPhysicsBody();

	virtual BodyType GetBodyType() const { return _BodyType; }
	virtual const Array<ShapePtr>& Shapes() const { return _Shapes; }
	virtual const AABB& GetWorldBoundingBox() const { return _WorldBoundingBox; }

	virtual Vector3 GetPosition() const;
	virtual void SetPosition(const Vector3& value);

	virtual Quaternion GetOrientation() const;
	virtual Matrix3 GetOrientationMatrix() const;
	virtual void SetOrientation(const Quaternion& value);
	virtual void SetOrientationMatrix(const Matrix3& value);

	virtual bool GetEnabled() const;
	virtual void SetEnabled(bool value);
	virtual bool GetActive() const;
	virtual void SetActive(bool value);
	virtual bool GetGravityEnabled() const;
	virtual void SetGravityEnabled(bool value);

	virtual BodyGroup GetGroup() const;
	virtual void SetGroup(BodyGroup value);

	virtual Vector3 GetCOMLocalPosition() const;
	virtual Quaternion GetCOMLocalOrientation() const;
	virtual Vector3 GetCOMWorldPosition() const;
	virtual void SetCOMWorldPosition(const Vector3& value);
	virtual Quaternion GetCOMWorldOrientation() const;
	virtual void SetCOMWorldOrientation(const Quaternion& value);

	virtual void SetCOMOffsetLocalPosition(const Vector3& value);
	virtual void SetCOMOffsetLocalOrientation(const Quaternion& value);
	virtual void SetCOMOffsetWorldPosition(const Vector3& value);
	virtual void SetCOMOffsetWorldOrientation(const Quaternion& value);

	virtual real GetMass() const;
	virtual void SetMass(real value);

	virtual Vector3 GetInertiaTensor() const;
	virtual void SetInertiaTensor(const Vector3& value);
	virtual Matrix3 GetWorldInertiaTensor() const;
	virtual Matrix3 GetInverseWorldInertiaTensor() const;
	virtual void SetMassFromShape(real density, real totalMass);

	virtual real GetLinearDamping() const;
	virtual void SetLinearDamping(real value);
	virtual real GetAngularDamping() const;
	virtual void SetAngularDamping(real value);

	virtual Vector3 GetLinearVelocity() const;
	virtual void SetLinearVelocity(const Vector3& value);
	virtual Vector3 GetAngularVelocity() const;
	virtual void SetAngularVelocity(const Vector3& value);
	virtual real GetMaxAngularVelocity() const;
	virtual void SetMaxAngularVelocity(real value);

	virtual Vector3 GetVelocityAtWorldPosition(const Vector3& value) const;
	virtual Vector3 GetVelocityAtLocalPosition(const Vector3& value) const;

	virtual Vector3 GetLinearMomentum() const;
	virtual void SetLinearMomentum(const Vector3& value);
	virtual Vector3 GetAngularMomentum() const;
	virtual void SetAngularMomentum(const Vector3& value);

	virtual void AddForceAtPosition(const Vector3& force, const Vector3& position);
	virtual void AddForceAtLocalPosition(const Vector3& force, const Vector3& position);
	virtual void AddLocalForceAtPosition(const Vector3& force, const Vector3& position);
	virtual void AddLocalForceAtLocalPosition(const Vector3& force, const Vector3& position);
	virtual void AddForce(const Vector3& force);
	virtual void AddLocalForce(const Vector3& force);
	virtual void AddTorque(const Vector3& torque);
	virtual void AddLocalTorque(const Vector3& torque);

	virtual void AddImpulseAtPosition(const Vector3& impulse, const Vector3& position);
	virtual void AddImpulseAtLocalPosition(const Vector3& impulse, const Vector3& position);
	virtual void AddLocalImpulseAtPosition(const Vector3& impulse, const Vector3& position);
	virtual void AddLocalImpulseAtLocalPosition(const Vector3& impulse, const Vector3& position);
	virtual void AddImpulse(const Vector3& impulse);
	virtual void AddLocalImpulse(const Vector3& impulse);
	virtual void AddTorqueImpulse(const Vector3& torque);
	virtual void AddLocalTorqueImpulse(const Vector3& torque);

	virtual void Update(real64 elapsed);

private:
	void CreateFromShapes();

protected:
	ODEPhysicsScene* _Scene;
	dBodyID _BodyID;
	dGeomID _GeomID;

	BodyType _BodyType;
	Array<ShapePtr> _Shapes;
	AABB _WorldBoundingBox;
};

}

#endif 
