/*=============================================================================
PhysicsBody.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_PHYSICSBODY_H_
#define _SE_PHYSICSBODY_H_

#include "SoftwarePhysicsSystem.h"
#include "PhysicsScene.h"

namespace SE_Software
{

/**
	Body.

	@todo
		Use the center of mass.
*/
class PhysicsBody : public Physics::IBody
{
	SE_DECLARE_CLASS(PhysicsBody, Object);

	SE_BEGIN_REFLECTION(PhysicsBody);
		SE_Field(_name, String, Public);
		SE_Field(_Position, Vector3, Public);
		//SE_Field(_Orientation, Quaternion, Public);
		//SE_Field(_OrientationMatrix, Matrix3, Public);
		//SE_Field(_InverseOrientationMatrix, Matrix3, Public);

		SE_Field(_Enabled, bool, Public);
		SE_Field(_Active, bool, Public);
		SE_Field(_GravityEnabled, bool, Public);

		//SE_Field(_Group, uint32, Public);

		//SE_Field(_COMPosition, Vector3, Public);
		//SE_Field(_COMOrientation, Quaternion, Public);
		//SE_Field(_COMWorldPosition, Vector3, Public);
		//SE_Field(_COMWorldOrientation, Quaternion, Public);

		SE_Field(_Mass, real32, Public);
		//SE_Field(_InverseMass, real, Public);

		//SE_Field(_InertiaTensor, Matrix3, Public);
		//SE_Field(_InverseInertiaTensor, Matrix3, Public);
		//SE_Field(_WorldInertiaTensor, Matrix3, Public);
		//SE_Field(_InverseWorldInertiaTensor, Matrix3, Public);

		//SE_Field(_LinearDamping, real, Public);
		//SE_Field(_AngularDamping, real, Public);

		SE_Field(_LinearVelocity, Vector3, Public);
		SE_Field(_AngularVelocity, Vector3, Public);
		SE_Field(_MaxAngularVelocity, real, Public);

		SE_Field(_LinearMomentum, Vector3, Public);
		SE_Field(_AngularMomentum, Vector3, Public);

		SE_Field(_Forces, Vector3, Public);
		SE_Field(_Torques, Vector3, Public);
	SE_END_REFLECTION(PhysicsBody);

public:
	struct State
	{
		Vector3 position;
		Matrix3 orientation;
		Vector3 linearMomentum;
		Vector3 angularMomentum;
	};

	PhysicsBody();
	PhysicsBody(PhysicsScene* scene, const BodyDescription& desc);
	virtual ~PhysicsBody();

	virtual BodyType GetBodyType() const { return _BodyType; }
	virtual const Array<ShapePtr>& Shapes() const { return _Shapes; }
	virtual const AABB& GetWorldBoundingBox() const { return _WorldBoundingBox; }

	virtual IntegrationType GetIntegrationType() const;
	virtual void SetIntegrationType(IntegrationType value);

	virtual Vector3 GetPosition() const;
	virtual void SetPosition(const Vector3& value);

	virtual Quaternion GetOrientation() const;
	virtual Matrix3 GetOrientationMatrix() const;
	virtual void SetOrientation(const Quaternion& value);
	virtual void SetOrientationMatrix(const Matrix3& value);

	virtual bool GetEnabled() const { return _Enabled; }
	virtual void SetEnabled(bool value) { _Enabled = value; }
	virtual bool GetActive() const { return _Active; }
	virtual void SetActive(bool value) { _Active = value; }
	virtual bool GetGravityEnabled() const { return _GravityEnabled; }
	virtual void SetGravityEnabled(bool value) { _GravityEnabled = value; }

	virtual BodyGroup GetGroup() const { return _Group; }
	virtual void SetGroup(BodyGroup value) { _Group = value; }

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

	real GetInverseMass() const { return _InverseMass; }
	void UpdateVelocity(real64 elapsed);

private:
	void CreateFromShapes();
	void UpdateShapes();
	void UpdateInertiaTensor();
	void Integrate(real delta, PhysicsBody::State& next);

	friend void BodyDerivatives(real t, const real* state, real* deltaState, const void* userData);

protected:
	PhysicsBody::State current;

	PhysicsScene* _Scene;

	BodyType _BodyType;
	Array<ShapePtr> _Shapes;
	AABB _LocalBoundingBox;
	AABB _WorldBoundingBox;

	Vector3 _Position;
	Quaternion _Orientation;
	Matrix3 _OrientationMatrix;
	Matrix3 _InverseOrientationMatrix;

	bool _Enabled;
	bool _Active;
	bool _GravityEnabled;

	BodyGroup _Group;

	Vector3 _COMPosition;
	Quaternion _COMOrientation;
	Vector3 _COMWorldPosition;
	Quaternion _COMWorldOrientation;

	real _Mass;
	real _InverseMass;

	Matrix3 _InertiaTensor;
	Matrix3 _InverseInertiaTensor;
	Matrix3 _WorldInertiaTensor;
	Matrix3 _InverseWorldInertiaTensor;

	real _LinearDamping;
	real _AngularDamping;

	Vector3 _LinearVelocity;
	Vector3 _AngularVelocity;
	real _MaxAngularVelocity;

	Vector3 _LinearMomentum;
	Vector3 _AngularMomentum;

	Vector3 _Forces;
	Vector3 _Torques;

	IntegrationType _IntegrationType;
	ODESolver* _Solver;
};

}

#endif 
