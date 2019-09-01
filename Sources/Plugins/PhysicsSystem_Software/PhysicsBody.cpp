/*=============================================================================
PhysicsBody.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "PhysicsBody.h"
#include "PhysicsMaterial.h"

namespace SE_Software
{

SE_IMPLEMENT_CLASS(PhysicsBody);
SE_IMPLEMENT_REFLECTION(PhysicsBody);

Matrix3 SkewSymmetric(const Vector3& value)
{
	return Matrix3(0.0f, -value.Z, value.Y, value.Z, 0.0f, -value.X, -value.Y, value.X, 0.0f);
}

void BodyDerivatives(real t, const real* state, real* deltaState, const void* userData)
{
	PhysicsBody* body = (PhysicsBody*)userData;
	PhysicsBody::State* s = (PhysicsBody::State*)state;

	struct BodyDerivatives
	{
		Vector3 linearVelocity;
		Matrix3 angularMatrix;
		Vector3 force;
		Vector3 torque;
	};
	
	BodyDerivatives* d = (BodyDerivatives*)deltaState;

	Vector3 angularVelocity;
	Matrix3 inverseWorldInertiaTensor;

	inverseWorldInertiaTensor = s->orientation * body->_InverseInertiaTensor * Matrix3::Transpose(s->orientation);
	angularVelocity = inverseWorldInertiaTensor * s->angularMomentum;

	// derivatives
	d->linearVelocity = body->_InverseMass * s->linearMomentum;
	d->angularMatrix = SkewSymmetric(angularVelocity) * s->orientation;
	d->force = - s->linearMomentum + body->_Forces;
	d->torque = - s->angularMomentum + body->_Torques;
}

PhysicsBody::PhysicsBody() :
	Physics::IBody()
{
}

PhysicsBody::PhysicsBody(PhysicsScene* scene, const BodyDescription& desc) :
	Physics::IBody(),
	_Position(Vector3::Zero),
	_Orientation(Quaternion::Identity),
	_OrientationMatrix(Matrix3::Identity),
	_InverseOrientationMatrix(Matrix3::Identity),
	_COMPosition(Vector3::Zero),
	_COMWorldPosition(Vector3::Zero),
	_COMOrientation(Quaternion::Identity),
	_COMWorldOrientation(Quaternion::Identity),
	_Mass(0.0),
	_InverseMass(0.0),
	_InertiaTensor(Matrix3::Identity),
	_InverseInertiaTensor(Matrix3::Identity),
	_WorldInertiaTensor(Matrix3::Identity),
	_InverseWorldInertiaTensor(Matrix3::Identity),
	_LinearDamping(0.0),
	_AngularDamping(0.05),
	_LinearVelocity(Vector3::Zero),
	_AngularVelocity(Vector3::Zero),
	_LinearMomentum(Vector3::Zero),
	_AngularMomentum(Vector3::Zero),
	_MaxAngularVelocity(0.0),
	_Forces(Vector3::Zero),
	_Torques(Vector3::Zero)
{
	_Scene = scene;
	_BodyType = desc._BodyType;
	_Shapes = desc._Shapes;

	_Enabled = desc._Enabled;
	_Active = desc._Active;
	_GravityEnabled = desc._GravityEnabled;

	_Group = 0;

	real mass = desc.GetMass();
	SetMass(mass);

	_IntegrationType = IntegrationType_Euler;
	_Solver = new ODESolverEuler(sizeof(PhysicsBody::State) / sizeof(real), BodyDerivatives, this);

	CreateFromShapes();
}

PhysicsBody::~PhysicsBody()
{
	SE_DELETE(_Solver);
}

void PhysicsBody::CreateFromShapes()
{
	if (_Shapes.IsEmpty())
		return;

	IShape* shape = _Shapes[0].Get();
	shape->SetBody(this);

	if (shape->GetMaterial() == NULL)
	{
		shape->SetMaterial(_Scene->GetMaterial(_Scene->GetDefaultMaterial()));
	}

	if (_BodyType == BodyType_Static)
		return;

	if (shape->GetMass() == 0.0 && shape->GetDensity() == 0.0)
	{
		Logger::Current()->Log(LogLevel::Error, _T("PhysicsBody.CreateFromShapes"),
			_T("Invalid shape: no mass and no density specified."));
		return;
	}

	real mass = shape->GetMass();

	SetInertiaTensor(shape->ComputeInertiaTensor());
	_LocalBoundingBox = shape->ComputeBoundingBox();

	switch (shape->GetShapeType())
	{
	case ShapeType_Plane:
		{
			IPlaneShape* plane = (IPlaneShape*)shape;
		}
		break;

	case ShapeType_Box:
		{
			IBoxShape* box = (IBoxShape*)shape;
			if (mass == 0.0) mass = IBoxShape::ComputeMass(box->Data, box->GetDensity());
		}
		break;

	case ShapeType_Sphere:
		{
			ISphereShape* sphere = (ISphereShape*)shape;
			if (mass == 0.0) mass = ISphereShape::ComputeMass(sphere->Data, sphere->GetDensity());
		}
		break;

	case ShapeType_Capsule:
		{
			ICapsuleShape* capsule = (ICapsuleShape*)shape;
			if (mass == 0.0) mass = ICapsuleShape::ComputeMass(capsule->Data, capsule->GetDensity());
		}
		break;

	case ShapeType_Mesh:
		{
			IMeshShape* mesh = (IMeshShape*)shape;
			OBB box(Vector3::Zero, _LocalBoundingBox.Max, Matrix3::Identity);
			if (mass == 0.0) mass = IBoxShape::ComputeMass(box, mesh->GetDensity());
		}
		break;

	default:
		return;
	}

	SetMass(mass);
}

void PhysicsBody::UpdateShapes()
{
	if (_Shapes.IsEmpty())
		return;

	IShape* shape = _Shapes[0].Get();

	switch (shape->GetShapeType())
	{
	case ShapeType_Plane:
		{
			IPlaneShape* plane = (IPlaneShape*)shape;
		}
		break;

	case ShapeType_Box:
		{
			IBoxShape* box = (IBoxShape*)shape;
			box->Data.Center = _Position;
			box->Data.Rotation = _OrientationMatrix;
		}
		break;

	case ShapeType_Sphere:
		{
			ISphereShape* sphere = (ISphereShape*)shape;
			sphere->Data.Center = _Position;
		}
		break;

	case ShapeType_Capsule:
		{
			ICapsuleShape* capsule = (ICapsuleShape*)shape;
		}
		break;

	case ShapeType_Mesh:
		{
			IMeshShape* mesh = (IMeshShape*)shape;
		}
		break;
	}
}

void PhysicsBody::UpdateInertiaTensor()
{
	_WorldInertiaTensor = _OrientationMatrix * _InertiaTensor * _InverseOrientationMatrix;
	_InverseWorldInertiaTensor = _OrientationMatrix * _InverseInertiaTensor * _InverseOrientationMatrix;
}

Vector3 PhysicsBody::GetPosition() const
{
	return _Position;
}

void PhysicsBody::SetPosition(const Vector3& value)
{
	_Position = value;
	UpdateShapes();
}

Quaternion PhysicsBody::GetOrientation() const
{
	return _Orientation;
}

Matrix3 PhysicsBody::GetOrientationMatrix() const
{
	return _OrientationMatrix;
}

void PhysicsBody::SetOrientation(const Quaternion& value)
{
	_Orientation = value;
	_Orientation.ToRotationMatrix(_OrientationMatrix);
	_InverseOrientationMatrix = Matrix3::Transpose(_OrientationMatrix);
	UpdateInertiaTensor();
	UpdateShapes();
}

void PhysicsBody::SetOrientationMatrix(const Matrix3& value)
{
	_OrientationMatrix = value;
	_Orientation = Quaternion::FromRotationMatrix(_OrientationMatrix);
	_InverseOrientationMatrix = Matrix3::Transpose(_OrientationMatrix);
	UpdateInertiaTensor();
	UpdateShapes();
}

Vector3 PhysicsBody::GetCOMLocalPosition() const
{
	if (_BodyType == BodyType_Static)
		return Vector3::Zero;

	return _COMPosition;
}

Quaternion PhysicsBody::GetCOMLocalOrientation() const
{
	if (_BodyType == BodyType_Static)
		return Quaternion::Identity;

	return _COMOrientation;
}

Vector3 PhysicsBody::GetCOMWorldPosition() const
{
	if (_BodyType == BodyType_Static)
		return Vector3::Zero;

	return _COMWorldPosition;
}

void PhysicsBody::SetCOMWorldPosition(const Vector3& value)
{
	if (_BodyType == BodyType_Static)
		return;

	_COMWorldPosition = value;
}

Quaternion PhysicsBody::GetCOMWorldOrientation() const
{
	if (_BodyType == BodyType_Static)
		return Quaternion::Identity;

	return _COMWorldOrientation;
}

void PhysicsBody::SetCOMWorldOrientation(const Quaternion& value)
{
	if (_BodyType == BodyType_Static)
		return;

	_COMWorldOrientation = value;
}

void PhysicsBody::SetCOMOffsetLocalPosition(const Vector3& value)
{
	if (_BodyType == BodyType_Static)
		return;

	//TODO
}

void PhysicsBody::SetCOMOffsetLocalOrientation(const Quaternion& value)
{
	if (_BodyType == BodyType_Static)
		return;

	//TODO
}

void PhysicsBody::SetCOMOffsetWorldPosition(const Vector3& value)
{
	if (_BodyType == BodyType_Static)
		return;

	//TODO
}

void PhysicsBody::SetCOMOffsetWorldOrientation(const Quaternion& value)
{
	if (_BodyType == BodyType_Static)
		return;

	//TODO
}

real PhysicsBody::GetMass() const
{
	if (_BodyType == BodyType_Static)
		return 0.0;

	return _Mass;
}

void PhysicsBody::SetMass(real value)
{
	if (_BodyType == BodyType_Static)
		return;

	_Mass = value;

	if (_Mass > Math::Epsilon)
	{
		_InverseMass = (1.0 / _Mass);
	}
	else
	{
		_InverseMass = 0.0;
	}
}

Vector3 PhysicsBody::GetInertiaTensor() const
{
	if (_BodyType == BodyType_Static)
		return Vector3::Zero;

	return Vector3(_InertiaTensor(0, 0), _InertiaTensor(1, 1), _InertiaTensor(2, 2));
}

void PhysicsBody::SetInertiaTensor(const Vector3& value)
{
	if (_BodyType == BodyType_Static)
		return;

	_InertiaTensor(0, 0) = value.X;
	_InertiaTensor(1, 1) = value.Y;
	_InertiaTensor(2, 2) = value.Z;

	_InverseInertiaTensor = Matrix3::Transpose(_InertiaTensor);
	UpdateInertiaTensor();
}

Matrix3 PhysicsBody::GetWorldInertiaTensor() const
{
	if (_BodyType == BodyType_Static)
		return Matrix3::Identity;

	return _WorldInertiaTensor;
}

Matrix3 PhysicsBody::GetInverseWorldInertiaTensor() const
{
	if (_BodyType == BodyType_Static)
		return Matrix3::Identity;

	return _InverseWorldInertiaTensor;
}

void PhysicsBody::SetMassFromShape(real density, real totalMass)
{
	if (_BodyType == BodyType_Static)
		return;

	//TODO
}

real PhysicsBody::GetLinearDamping() const
{
	if (_BodyType == BodyType_Static)
		return 0.0;

	return _LinearDamping;
}

void PhysicsBody::SetLinearDamping(real value)
{
	if (_BodyType == BodyType_Static)
		return;

	_LinearDamping =  value;
}

real PhysicsBody::GetAngularDamping() const
{
	if (_BodyType == BodyType_Static)
		return 0.0;

	return _AngularDamping;
}

void PhysicsBody::SetAngularDamping(real value)
{
	if (_BodyType == BodyType_Static)
		return;

	_AngularDamping = value;
}

Vector3 PhysicsBody::GetLinearVelocity() const
{
	if (_BodyType == BodyType_Static)
		return Vector3::Zero;

	return _LinearVelocity;
}

void PhysicsBody::SetLinearVelocity(const Vector3& value)
{
	if (_BodyType == BodyType_Static)
		return;

	_LinearVelocity = value;
}

Vector3 PhysicsBody::GetAngularVelocity() const
{
	if (_BodyType == BodyType_Static)
		return Vector3::Zero;

	return _AngularVelocity;
}

void PhysicsBody::SetAngularVelocity(const Vector3& value)
{
	if (_BodyType == BodyType_Static)
		return;

	_AngularVelocity = value;
}

real PhysicsBody::GetMaxAngularVelocity() const
{
	return _MaxAngularVelocity;
}

void PhysicsBody::SetMaxAngularVelocity(real value)
{
	_MaxAngularVelocity = value;
}

Vector3 PhysicsBody::GetVelocityAtWorldPosition(const Vector3& value) const
{
	if (_BodyType == BodyType_Static)
		return Vector3::Zero;

	return _LinearVelocity + Vector3::Cross(_AngularVelocity, value - _Position);
}

Vector3 PhysicsBody::GetVelocityAtLocalPosition(const Vector3& value) const
{
	if (_BodyType == BodyType_Static)
		return Vector3::Zero;

	return _LinearVelocity + Vector3::Cross(_AngularVelocity, _Orientation.Rotate(value));
}

Vector3 PhysicsBody::GetLinearMomentum() const
{
	if (_BodyType == BodyType_Static)
		return Vector3::Zero;

	return _LinearMomentum;
}

void PhysicsBody::SetLinearMomentum(const Vector3& value)
{
	if (_BodyType == BodyType_Static)
		return;

	_LinearMomentum = value;
}

Vector3 PhysicsBody::GetAngularMomentum() const
{
	if (_BodyType == BodyType_Static)
		return Vector3::Zero;

	return _AngularMomentum;
}

void PhysicsBody::SetAngularMomentum(const Vector3& value)
{
	if (_BodyType == BodyType_Static)
		return;

	_AngularMomentum = value;
}

void PhysicsBody::AddForceAtPosition(const Vector3& force, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;

	_Forces += force;
	_Torques += Vector3::Cross(position - _Position, force);
	SetActive(true);
}

void PhysicsBody::AddForceAtLocalPosition(const Vector3& force, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;

	_Forces += force;
	_Torques += Vector3::Cross(_Orientation.Rotate(position), force);
	SetActive(true);
}

void PhysicsBody::AddLocalForceAtPosition(const Vector3& force, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;

	_Forces += _Orientation.Rotate(force);
	_Torques += Vector3::Cross(position - _Position, force);
	SetActive(true);
}

void PhysicsBody::AddLocalForceAtLocalPosition(const Vector3& force, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;

	_Forces += _Orientation.Rotate(force);
	_Torques += Vector3::Cross(_Orientation.Rotate(position), force);
	SetActive(true);
}

void PhysicsBody::AddForce(const Vector3& force)
{
	if (_BodyType == BodyType_Static)
		return;

	_Forces += force;
	SetActive(true);
}

void PhysicsBody::AddLocalForce(const Vector3& force)
{
	if (_BodyType == BodyType_Static)
		return;

	_LinearVelocity += _Orientation.Rotate(force);
	SetActive(true);
}

void PhysicsBody::AddTorque(const Vector3& torque)
{
	if (_BodyType == BodyType_Static)
		return;

	_Torques += torque;
	SetActive(true);
}

void PhysicsBody::AddLocalTorque(const Vector3& torque)
{
	if (_BodyType == BodyType_Static)
		return;

	_Torques += _Orientation.Rotate(torque);
	SetActive(true);
}

void PhysicsBody::AddImpulseAtPosition(const Vector3& impulse, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;

	_LinearVelocity += impulse * _InverseMass;
	_AngularVelocity += _InverseWorldInertiaTensor * Vector3::Cross(position - _Position, impulse);
	SetActive(true);
}

void PhysicsBody::AddImpulseAtLocalPosition(const Vector3& impulse, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;

	_LinearVelocity += impulse * _InverseMass;
	_AngularVelocity += _InverseWorldInertiaTensor * Vector3::Cross(_Orientation.Rotate(position), impulse);
	SetActive(true);
}

void PhysicsBody::AddLocalImpulseAtPosition(const Vector3& impulse, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;

	_LinearVelocity += _Orientation.Rotate(impulse) * _InverseMass;
	_AngularVelocity += _InverseWorldInertiaTensor * Vector3::Cross(position - _Position, impulse);
	SetActive(true);
}

void PhysicsBody::AddLocalImpulseAtLocalPosition(const Vector3& impulse, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;

	_LinearVelocity += impulse * _InverseMass;
	_AngularVelocity += _InverseWorldInertiaTensor * Vector3::Cross(_Orientation.Rotate(position), impulse);
	SetActive(true);
}

void PhysicsBody::AddImpulse(const Vector3& impulse)
{
	if (_BodyType == BodyType_Static)
		return;

	_LinearVelocity += impulse * _InverseMass;
	SetActive(true);
}

void PhysicsBody::AddLocalImpulse(const Vector3& impulse)
{
	if (_BodyType == BodyType_Static)
		return;

	_LinearVelocity += _Orientation.Rotate(impulse) * _InverseMass;
	SetActive(true);
}

void PhysicsBody::AddTorqueImpulse(const Vector3& torque)
{
	if (_BodyType == BodyType_Static)
		return;

	_AngularVelocity += _InverseWorldInertiaTensor * torque;
	SetActive(true);
}

void PhysicsBody::AddLocalTorqueImpulse(const Vector3& torque)
{
	if (_BodyType == BodyType_Static)
		return;

	_AngularVelocity += _InverseWorldInertiaTensor * _Orientation.Rotate(torque);
	SetActive(true);
}

IntegrationType PhysicsBody::GetIntegrationType() const
{
	return _IntegrationType;
}

void PhysicsBody::SetIntegrationType(IntegrationType value)
{
	if (value == IntegrationType_Default)
		value = IntegrationType_Euler;

	if (_IntegrationType != value)
	{
		_IntegrationType = value;

		SE_DELETE(_Solver);
		switch (_IntegrationType)
		{
		case IntegrationType_Euler:
			_Solver = new ODESolverEuler(
				sizeof(PhysicsBody::State) / sizeof(real), BodyDerivatives, this);
			break;
		case IntegrationType_Midpoint:
			_Solver = new ODESolverMidpoint(
				sizeof(PhysicsBody::State) / sizeof(real), BodyDerivatives, this);
			break;
		case IntegrationType_RK4:
			_Solver = new ODESolverRungeKutta4(
				sizeof(PhysicsBody::State) / sizeof(real), BodyDerivatives, this);
			break;
		}
	}
}

void PhysicsBody::UpdateVelocity(real64 elapsed)
{
	if (_BodyType == BodyType_Dynamic)
	{
		// Integrate
		_LinearMomentum = (_Forces * elapsed);
		_AngularMomentum = (_Torques * elapsed);

		_LinearVelocity += _InverseMass * _LinearMomentum;
		_AngularVelocity += _InverseInertiaTensor * _AngularMomentum;
	}
}

void PhysicsBody::Integrate(real delta, PhysicsBody::State& next)
{
	_Solver->Update((real*)&current, (real*)&next, 0.0, delta);
}

void PhysicsBody::Update(real64 elapsed)
{
	PhysicsBody::State next;

	if (_BodyType == BodyType_Dynamic)
	{
		// Gravity
		// Add the gravity force: P = m * g.
		if (_GravityEnabled)
			_Forces += _Mass * _Scene->GetGravity();

		// Damping
		// Apply a force and torque in the opposite direction of motion scaled by the body's velocity 
		_Forces -= _LinearDamping * _LinearVelocity;
		_Torques -= _AngularDamping * _AngularVelocity;
	}

#if 0
	if (_BodyType == BodyType_Dynamic)
	{
		current.position = _Position;
		current.orientation = _OrientationMatrix;
		current.linearMomentum = _LinearMomentum;
		current.angularMomentum = _AngularMomentum;

		next = current;

		// calculate next position and orientation
		Integrate(elapsed, next);

		// set the new state
		current = next;

		_Position = current.position;
		_OrientationMatrix = current.orientation;
		_LinearMomentum = current.linearMomentum;
		_AngularMomentum = current.angularMomentum;

		_Orientation = Quaternion::FromRotationMatrix(_OrientationMatrix);
		_Orientation.Normalize();

		_LinearVelocity = _InverseMass * _LinearMomentum;
		_AngularVelocity = _InverseInertiaTensor * _AngularMomentum;
	}

#else
	UpdateVelocity(elapsed);

	// Update the position
	_Position += _LinearVelocity * elapsed;

	// Update the orientation
	_AngularVelocity.Normalize();
	_Orientation *= Quaternion::FromAxisAngle(_AngularVelocity, elapsed);
	_Orientation.Normalize();

	// Update the orientation matrix
	_Orientation.ToRotationMatrix(_OrientationMatrix);
#endif

	// Update the world inertia tensor and shapes
	UpdateInertiaTensor();
	UpdateShapes();

	_WorldBoundingBox = _LocalBoundingBox;
	_WorldBoundingBox.Transform(Matrix4(_OrientationMatrix, _Position));

	// Reset forces
	_Forces = Vector3::Zero;
	_Torques = Vector3::Zero;
}

}
