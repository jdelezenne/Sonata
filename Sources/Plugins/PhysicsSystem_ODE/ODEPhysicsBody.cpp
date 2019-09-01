/*=============================================================================
ODEPhysicsBody.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ODEPhysicsBody.h"
#include "ODEPhysicsMaterial.h"

namespace SE_ODE
{

ODEPhysicsBody::ODEPhysicsBody(ODEPhysicsScene* scene, const BodyDescription& desc) :
	Physics::IBody()
{
	_Scene = scene;
	_BodyID = NULL;
	_GeomID = NULL;

	_BodyType = desc._BodyType;
	_Shapes = desc._Shapes;

	_BodyID = dBodyCreate(_Scene->GetWorldID());

	SetEnabled(desc._Enabled);
	SetActive(desc._Active);
	SetGravityEnabled(desc._GravityEnabled);

	if (_BodyType == BodyType_Dynamic)
	{
	}

	CreateFromShapes();
}

ODEPhysicsBody::~ODEPhysicsBody()
{
	if (_GeomID != NULL)
	{
		dGeomDestroy(_GeomID);
		_GeomID = NULL;
	}

	if (_BodyID != NULL)
	{
		dBodyDestroy(_BodyID);
		_BodyID = NULL;
	}
}

void ODEPhysicsBody::CreateFromShapes()
{
	if (_Shapes.IsEmpty())
		return;

	IShape* shape = _Shapes[0].Get();

	real mass = shape->GetMass();

	switch (shape->GetShapeType())
	{
	case ShapeType_Plane:
		{
			IPlaneShape* plane = (IPlaneShape*)shape;

			_GeomID = dCreatePlane(_Scene->GetSpaceID(),
				plane->Data.Normal.X, plane->Data.Normal.Y, plane->Data.Normal.Z, plane->Data.D);
		}
		break;

	case ShapeType_Box:
		{
			IBoxShape* box = (IBoxShape*)shape;

			_GeomID = dCreateBox(_Scene->GetSpaceID(),
				box->Data.Extents.X, box->Data.Extents.Y, box->Data.Extents.Z);

			dGeomSetBody(_GeomID, _BodyID);

			dMass odeMass;
			dMassSetBox(&odeMass, 1, box->Data.Extents.X, box->Data.Extents.Y, box->Data.Extents.Z);
			if (mass == 0.0) mass = IBoxShape::ComputeMass(box->Data, box->GetDensity());
			dMassAdjust(&odeMass, mass);
			dBodySetMass(_BodyID, &odeMass);
		}
		break;

	case ShapeType_Sphere:
		{
			ISphereShape* sphere = (ISphereShape*)shape;

			_GeomID = dCreateSphere(_Scene->GetSpaceID(), sphere->Data.Radius);

			dGeomSetBody(_GeomID, _BodyID);

			dMass odeMass;
			dMassSetSphere(&odeMass, 1, sphere->Data.Radius);
			if (mass == 0.0) mass = ISphereShape::ComputeMass(sphere->Data, sphere->GetDensity());
			dMassAdjust(&odeMass, mass);
			dBodySetMass(_BodyID, &odeMass);
		}
		break;

	case ShapeType_Capsule:
		{
			ICapsuleShape* capsule = (ICapsuleShape*)shape;

			_GeomID = dCreateCCylinder(_Scene->GetSpaceID(),
				capsule->Data.GetRadius(), capsule->Data.GetHeight());

			dGeomSetBody(_GeomID, _BodyID);

			dMass odeMass;
			dMassSetCylinder(&odeMass, 1, 0, capsule->Data.GetRadius(), capsule->Data.GetHeight());
			if (mass == 0.0) mass = ICapsuleShape::ComputeMass(capsule->Data, capsule->GetDensity());
			dMassAdjust(&odeMass, mass);
			dBodySetMass(_BodyID, &odeMass);
		}
		break;

	case ShapeType_Mesh:
		{
			IMeshShape* mesh = (IMeshShape*)shape;
		}
		break;

	default:
		return;
	}
}

Vector3 ODEPhysicsBody::GetPosition() const
{
	return ODEHelper::MakeVector3(dBodyGetPosition(_BodyID));
}

void ODEPhysicsBody::SetPosition(const Vector3& value)
{
	dBodySetPosition(_BodyID, value.X, value.Y, value.Z);
}

Quaternion ODEPhysicsBody::GetOrientation() const
{
	return ODEHelper::MakeQuaternion(dBodyGetQuaternion(_BodyID));
}

Matrix3 ODEPhysicsBody::GetOrientationMatrix() const
{
	return ODEHelper::MakeMatrix3(dBodyGetRotation(_BodyID));
}

void ODEPhysicsBody::SetOrientation(const Quaternion& value)
{
	dBodySetQuaternion(_BodyID, ODEHelper::MakeQuaternion(value));
}

void ODEPhysicsBody::SetOrientationMatrix(const Matrix3& value)
{
	dBodySetRotation(_BodyID, ODEHelper::MakeMatrix3(value));
}

bool ODEPhysicsBody::GetEnabled() const
{
	if (_BodyID == NULL || _BodyType == BodyType_Static)
		return false;

	return dBodyIsEnabled(_BodyID) != 0;
}

void ODEPhysicsBody::SetEnabled(bool value)
{
	if (_BodyID == NULL || _BodyType == BodyType_Static)
		return;

	if (value)
		dBodyEnable(_BodyID);
	else
		dBodyDisable(_BodyID);
}

bool ODEPhysicsBody::GetActive() const
{
	if (_BodyID == NULL || _BodyType == BodyType_Static)
		return false;

	return true;
}

void ODEPhysicsBody::SetActive(bool value)
{
	if (_BodyID == NULL || _BodyType == BodyType_Static)
		return;
}

bool ODEPhysicsBody::GetGravityEnabled() const
{
	if (_BodyID == NULL || _BodyType == BodyType_Static)
		return false;

	return dBodyGetGravityMode(_BodyID) != 0;
}

void ODEPhysicsBody::SetGravityEnabled(bool value)
{
	if (_BodyID == NULL || _BodyType == BodyType_Static)
		return;

	dBodySetGravityMode(_BodyID, value ? 1 : 0);
}

BodyGroup ODEPhysicsBody::GetGroup() const
{
	return (BodyGroup)0;
}

void ODEPhysicsBody::SetGroup(BodyGroup value)
{
}

Vector3 ODEPhysicsBody::GetCOMLocalPosition() const
{
	return Vector3::Zero;
}

Quaternion ODEPhysicsBody::GetCOMLocalOrientation() const
{
	return Quaternion::Identity;
}

Vector3 ODEPhysicsBody::GetCOMWorldPosition() const
{
	return Vector3::Zero;
}

void ODEPhysicsBody::SetCOMWorldPosition(const Vector3& value)
{
}

Quaternion ODEPhysicsBody::GetCOMWorldOrientation() const
{
	return Quaternion::Identity;
}

void ODEPhysicsBody::SetCOMWorldOrientation(const Quaternion& value)
{
}

void ODEPhysicsBody::SetCOMOffsetLocalPosition(const Vector3& value)
{
}

void ODEPhysicsBody::SetCOMOffsetLocalOrientation(const Quaternion& value)
{
}

void ODEPhysicsBody::SetCOMOffsetWorldPosition(const Vector3& value)
{
}

void ODEPhysicsBody::SetCOMOffsetWorldOrientation(const Quaternion& value)
{
}

real ODEPhysicsBody::GetMass() const
{
	dMass mass;
	dBodyGetMass(_BodyID, &mass);
	return mass.mass;
}

void ODEPhysicsBody::SetMass(real value)
{
	dMass mass;
	dMassSetZero(&mass);
	dMassAdjust(&mass, value);
	dBodySetMass(_BodyID, &mass);
}

Vector3 ODEPhysicsBody::GetInertiaTensor() const
{
	dMass mass;
	dBodyGetMass(_BodyID, &mass);
	return Vector3(mass.I[0], mass.I[3], mass.I[6]);
}

void ODEPhysicsBody::SetInertiaTensor(const Vector3& value)
{
}

Matrix3 ODEPhysicsBody::GetWorldInertiaTensor() const
{
	return Matrix3::Identity;
}

Matrix3 ODEPhysicsBody::GetInverseWorldInertiaTensor() const
{
	return Matrix3::Identity;
}

void ODEPhysicsBody::SetMassFromShape(real density, real totalMass)
{
}

real ODEPhysicsBody::GetLinearDamping() const
{
	return 0.0;
}

void ODEPhysicsBody::SetLinearDamping(real value)
{
}

real ODEPhysicsBody::GetAngularDamping() const
{
	return 0.0;
}

void ODEPhysicsBody::SetAngularDamping(real value)
{
}

Vector3 ODEPhysicsBody::GetLinearVelocity() const
{
	return ODEHelper::MakeVector3(dBodyGetLinearVel(_BodyID));
}

void ODEPhysicsBody::SetLinearVelocity(const Vector3& value)
{
	dBodySetLinearVel(_BodyID, value.X, value.Y, value.Z);
}

Vector3 ODEPhysicsBody::GetAngularVelocity() const
{
	return ODEHelper::MakeVector3(dBodyGetAngularVel(_BodyID));
}

void ODEPhysicsBody::SetAngularVelocity(const Vector3& value)
{
	dBodySetAngularVel(_BodyID, value.X, value.Y, value.Z);
}

real ODEPhysicsBody::GetMaxAngularVelocity() const
{
	return 0.0;
}

void ODEPhysicsBody::SetMaxAngularVelocity(real value)
{
}

Vector3 ODEPhysicsBody::GetVelocityAtWorldPosition(const Vector3& value) const
{
	dVector3 result;
	dBodyGetPointVel(_BodyID, value.X, value.Y, value.Z, result);
	return ODEHelper::MakeVector3(result);
}

Vector3 ODEPhysicsBody::GetVelocityAtLocalPosition(const Vector3& value) const
{
	dVector3 result;
	dBodyGetRelPointVel(_BodyID, value.X, value.Y, value.Z, result);
	return ODEHelper::MakeVector3(result);
}

Vector3 ODEPhysicsBody::GetLinearMomentum() const
{
	return Vector3::Zero;
}

void ODEPhysicsBody::SetLinearMomentum(const Vector3& value)
{
}

Vector3 ODEPhysicsBody::GetAngularMomentum() const
{
	return Vector3::Zero;
}

void ODEPhysicsBody::SetAngularMomentum(const Vector3& value)
{
}

void ODEPhysicsBody::AddForceAtPosition(const Vector3& force, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;

	dBodyAddForceAtPos(_BodyID, force.X, force.Y, force.Z, position.X, position.Y, position.Z);
}

void ODEPhysicsBody::AddForceAtLocalPosition(const Vector3& force, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;

	dBodyAddForceAtRelPos(_BodyID, force.X, force.Y, force.Z, position.X, position.Y, position.Z);
}

void ODEPhysicsBody::AddLocalForceAtPosition(const Vector3& force, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;

	dBodyAddRelForceAtPos(_BodyID, force.X, force.Y, force.Z, position.X, position.Y, position.Z);
}

void ODEPhysicsBody::AddLocalForceAtLocalPosition(const Vector3& force, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;

	dBodyAddRelForceAtRelPos(_BodyID, force.X, force.Y, force.Z, position.X, position.Y, position.Z);
}

void ODEPhysicsBody::AddForce(const Vector3& force)
{
	if (_BodyType == BodyType_Static)
		return;

	dBodyAddForce(_BodyID, force.X, force.Y, force.Z);
}

void ODEPhysicsBody::AddLocalForce(const Vector3& force)
{
	if (_BodyType == BodyType_Static)
		return;

	dBodyAddRelForce(_BodyID, force.X, force.Y, force.Z);
}

void ODEPhysicsBody::AddTorque(const Vector3& torque)
{
	if (_BodyType == BodyType_Static)
		return;

	dBodyAddTorque(_BodyID, torque.X, torque.Y, torque.Z);
}

void ODEPhysicsBody::AddLocalTorque(const Vector3& torque)
{
	if (_BodyType == BodyType_Static)
		return;

	dBodyAddRelTorque(_BodyID, torque.X, torque.Y, torque.Z);
}

void ODEPhysicsBody::AddImpulseAtPosition(const Vector3& impulse, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;
}

void ODEPhysicsBody::AddImpulseAtLocalPosition(const Vector3& impulse, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;
}

void ODEPhysicsBody::AddLocalImpulseAtPosition(const Vector3& impulse, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;
}

void ODEPhysicsBody::AddLocalImpulseAtLocalPosition(const Vector3& impulse, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;
}

void ODEPhysicsBody::AddImpulse(const Vector3& impulse)
{
	if (_BodyType == BodyType_Static)
		return;
}

void ODEPhysicsBody::AddLocalImpulse(const Vector3& impulse)
{
	if (_BodyType == BodyType_Static)
		return;
}

void ODEPhysicsBody::AddTorqueImpulse(const Vector3& torque)
{
	if (_BodyType == BodyType_Static)
		return;
}

void ODEPhysicsBody::AddLocalTorqueImpulse(const Vector3& torque)
{
	if (_BodyType == BodyType_Static)
		return;
}

void ODEPhysicsBody::Update(real64 elapsed)
{
	if (_BodyID == NULL)
		return;
}

}
