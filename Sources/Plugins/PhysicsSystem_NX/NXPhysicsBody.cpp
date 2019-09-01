/*=============================================================================
NXPhysicsBody.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "NXPhysicsBody.h"
#include "NXPhysicsMaterial.h"
#include "NXPhysicsStream.h"

namespace SE_NX
{

NXPhysicsBody::NXPhysicsBody(NXPhysicsScene* scene, const BodyDescription& desc) :
	Physics::IBody()
{
	_Scene = scene;
	_NxActor = NULL;
	_BodyType = desc._BodyType;
	_Shapes = desc._Shapes;

	_Enabled = desc._Enabled;
	_Active = desc._Active;
	_GravityEnabled = desc._GravityEnabled;

	CreateFromShapes();
}

NXPhysicsBody::~NXPhysicsBody()
{
	if (_NxActor != NULL)
	{
		if (_Scene->GetNxScene())
			_Scene->GetNxScene()->releaseActor(*_NxActor);
		_NxActor = NULL;
	}
}

void NXPhysicsBody::CreateFromShapes()
{
	if (_Shapes.IsEmpty())
		return;

	IShape* shape = _Shapes[0].Get();

	NxActorDesc actorDesc;

	IMaterial* material = shape->GetMaterial();
	NxMaterialIndex materialIndex;
	if (material != NULL)
	{
		material->Apply();
		materialIndex = ((NXPhysicsMaterial*)material)->GetNxMaterialIndex();
	}
	else
	{
		materialIndex = 0;
	}

	switch (shape->GetShapeType())
	{
	case ShapeType_Plane:
		{
			IPlaneShape* plane = (IPlaneShape*)shape;
			
			NxPlaneShapeDesc desc;
			desc.materialIndex = materialIndex;
			desc.normal = NXHelper::MakeVector3(plane->Data.Normal);
			desc.d = plane->Data.D;
			actorDesc.shapes.pushBack(&desc);
		}
		break;

	case ShapeType_Box:
		{
			IBoxShape* box = (IBoxShape*)shape;
			
			NxBoxShapeDesc desc;
			desc.materialIndex = materialIndex;
			desc.dimensions = NXHelper::MakeVector3(box->Data.GetExtents());
			actorDesc.shapes.pushBack(&desc);
		}
		break;

	case ShapeType_Sphere:
		{
			ISphereShape* sphere = (ISphereShape*)shape;
			
			NxSphereShapeDesc desc;
			desc.materialIndex = materialIndex;
			desc.radius = sphere->Data.Radius;
			actorDesc.shapes.pushBack(&desc);
		}
		break;

	case ShapeType_Capsule:
		{
			ICapsuleShape* capsule = (ICapsuleShape*)shape;
			
			NxCapsuleShapeDesc desc;
			desc.materialIndex = materialIndex;
			desc.height = capsule->Data.GetHeight();
			desc.radius = capsule->Data.GetRadius();
			actorDesc.shapes.pushBack(&desc);
		}
		break;

	case ShapeType_Mesh:
		{
			IMeshShape* mesh = (IMeshShape*)shape;

			NxTriangleMeshDesc triMeshDesc;
			triMeshDesc.numVertices = mesh->Vertices.Count();
			triMeshDesc.points = &mesh->Vertices[0];
			triMeshDesc.pointStrideBytes = sizeof(Vector3);
			triMeshDesc.numTriangles = mesh->Triangles.Count();
			triMeshDesc.triangles = &mesh->Triangles[0];
			triMeshDesc.triangleStrideBytes = 3*sizeof(uint32);
			triMeshDesc.materialIndices = &mesh->MaterialIndices[0];
			triMeshDesc.materialIndexStride = sizeof(uint32);

			MemoryWriteBuffer buf;
			bool status = NxCookTriangleMesh(triMeshDesc, buf);
			MemoryReadBuffer readBuffer(buf.data);

			NxTriangleMeshShapeDesc desc;
			desc.meshData = _Scene->GetPhysicsSystem()->GetNxSDK()->createTriangleMesh(readBuffer);
			actorDesc.shapes.pushBack(&desc);
		}
		break;

	default:
		return;
	}

	if (_BodyType == BodyType_Dynamic)
	{
		NxBodyDesc bodyDesc;
		if (shape->GetDensity() > 0)
			actorDesc.density = shape->GetDensity();
		else if (shape->GetMass() > 0)
			bodyDesc.mass = shape->GetMass();
		actorDesc.body = &bodyDesc;
	}

	_NxActor = _Scene->GetNxScene()->createActor(actorDesc);
}

Vector3 NXPhysicsBody::GetPosition() const
{
	return NXHelper::MakeVector3(_NxActor->getGlobalPosition());
}

void NXPhysicsBody::SetPosition(const Vector3& value)
{
	_NxActor->setGlobalPosition(NXHelper::MakeVector3(value));
}

Quaternion NXPhysicsBody::GetOrientation() const
{
	return NXHelper::MakeQuaternion(_NxActor->getGlobalOrientationQuat());
}

Matrix3 NXPhysicsBody::GetOrientationMatrix() const
{
	return NXHelper::MakeMatrix33(_NxActor->getGlobalOrientation());
}

void NXPhysicsBody::SetOrientation(const Quaternion& value)
{
	_NxActor->setGlobalOrientationQuat(NXHelper::MakeQuaternion(value));
}

void NXPhysicsBody::SetOrientationMatrix(const Matrix3& value)
{
	_NxActor->setGlobalOrientation(NXHelper::MakeMatrix33(value));
}

BodyGroup NXPhysicsBody::GetGroup() const
{
	return (BodyGroup)_NxActor->getGroup();
}

void NXPhysicsBody::SetGroup(BodyGroup value)
{
	_NxActor->setGroup((NxActorGroup)value);
}

Vector3 NXPhysicsBody::GetCOMLocalPosition() const
{
	return NXHelper::MakeVector3(_NxActor->getCMassLocalPosition());
}

Quaternion NXPhysicsBody::GetCOMLocalOrientation() const
{
	return NXHelper::MakeQuaternion(_NxActor->getCMassLocalOrientation());
}

Vector3 NXPhysicsBody::GetCOMWorldPosition() const
{
	return NXHelper::MakeVector3(_NxActor->getCMassGlobalPosition());
}

void NXPhysicsBody::SetCOMWorldPosition(const Vector3& value)
{
	_NxActor->setCMassGlobalPosition(NXHelper::MakeVector3(value));
}

Quaternion NXPhysicsBody::GetCOMWorldOrientation() const
{
	return NXHelper::MakeQuaternion(_NxActor->getCMassGlobalOrientation());
}

void NXPhysicsBody::SetCOMWorldOrientation(const Quaternion& value)
{
	_NxActor->setCMassGlobalOrientation(NXHelper::MakeQuaternion(value));
}

void NXPhysicsBody::SetCOMOffsetLocalPosition(const Vector3& value)
{
	_NxActor->setCMassOffsetGlobalPosition(NXHelper::MakeVector3(value));
}

void NXPhysicsBody::SetCOMOffsetLocalOrientation(const Quaternion& value)
{
	_NxActor->setCMassOffsetLocalOrientation(NXHelper::MakeQuaternion(value));
}

void NXPhysicsBody::SetCOMOffsetWorldPosition(const Vector3& value)
{
	_NxActor->setCMassOffsetGlobalPosition(NXHelper::MakeVector3(value));
}

void NXPhysicsBody::SetCOMOffsetWorldOrientation(const Quaternion& value)
{
	_NxActor->setCMassOffsetGlobalOrientation(NXHelper::MakeQuaternion(value));
}

real NXPhysicsBody::GetMass() const
{
	return _NxActor->getMass();
}

void NXPhysicsBody::SetMass(real value)
{
	_NxActor->setMass(value);
}

Vector3 NXPhysicsBody::GetInertiaTensor() const
{
	return NXHelper::MakeVector3(_NxActor->getMassSpaceInertiaTensor());
}

void NXPhysicsBody::SetInertiaTensor(const Vector3& value)
{
	_NxActor->setMassSpaceInertiaTensor(NXHelper::MakeVector3(value));
}

Matrix3 NXPhysicsBody::GetWorldInertiaTensor() const
{
	return NXHelper::MakeMatrix33(_NxActor->getGlobalInertiaTensor());
}

Matrix3 NXPhysicsBody::GetInverseWorldInertiaTensor() const
{
	return NXHelper::MakeMatrix33(_NxActor->getGlobalInertiaTensorInverse());
}

void NXPhysicsBody::SetMassFromShape(real density, real totalMass)
{
	_NxActor->updateMassFromShapes(density, totalMass);
}

real NXPhysicsBody::GetLinearDamping() const
{
	return _NxActor->getLinearDamping();
}

void NXPhysicsBody::SetLinearDamping(real value)
{
	_NxActor->setLinearDamping(value);
}

real NXPhysicsBody::GetAngularDamping() const
{
	return _NxActor->getAngularDamping();
}

void NXPhysicsBody::SetAngularDamping(real value)
{
	_NxActor->setAngularDamping(value);
}

Vector3 NXPhysicsBody::GetLinearVelocity() const
{
	if (_BodyType != BodyType_Dynamic)
		return Vector3::Zero;

	return NXHelper::MakeVector3(_NxActor->getLinearVelocity());
}

void NXPhysicsBody::SetLinearVelocity(const Vector3& value)
{
	if (_BodyType != BodyType_Dynamic)
		return;

	_NxActor->setLinearVelocity(NXHelper::MakeVector3(value));
}

Vector3 NXPhysicsBody::GetAngularVelocity() const
{
	if (_BodyType != BodyType_Dynamic)
		return Vector3::Zero;

	return NXHelper::MakeVector3(_NxActor->getAngularVelocity());
}

void NXPhysicsBody::SetAngularVelocity(const Vector3& value)
{
	if (_BodyType != BodyType_Dynamic)
		return;

	_NxActor->setAngularVelocity(NXHelper::MakeVector3(value));
}

real NXPhysicsBody::GetMaxAngularVelocity() const
{
	if (_BodyType != BodyType_Dynamic)
		return 0.0;

	return _NxActor->getMaxAngularVelocity();
}

void NXPhysicsBody::SetMaxAngularVelocity(real value)
{
	if (_BodyType != BodyType_Dynamic)
		return;

	_NxActor->setMaxAngularVelocity(value);
}

Vector3 NXPhysicsBody::GetVelocityAtWorldPosition(const Vector3& value) const
{
	if (_BodyType != BodyType_Dynamic)
		return Vector3::Zero;

	return NXHelper::MakeVector3(_NxActor->getPointVelocity(NXHelper::MakeVector3(value)));
}

Vector3 NXPhysicsBody::GetVelocityAtLocalPosition(const Vector3& value) const
{
	if (_BodyType != BodyType_Dynamic)
		return Vector3::Zero;

	return NXHelper::MakeVector3(_NxActor->getLocalPointVelocity(NXHelper::MakeVector3(value)));
}

Vector3 NXPhysicsBody::GetLinearMomentum() const
{
	if (_BodyType != BodyType_Dynamic)
		return Vector3::Zero;

	return NXHelper::MakeVector3(_NxActor->getLinearMomentum());
}

void NXPhysicsBody::SetLinearMomentum(const Vector3& value)
{
	if (_BodyType != BodyType_Dynamic)
		return;

	_NxActor->setLinearMomentum(NXHelper::MakeVector3(value));
}

Vector3 NXPhysicsBody::GetAngularMomentum() const
{
	if (_BodyType != BodyType_Dynamic)
		return Vector3::Zero;

	return NXHelper::MakeVector3(_NxActor->getAngularMomentum());
}

void NXPhysicsBody::SetAngularMomentum(const Vector3& value)
{
	if (_BodyType != BodyType_Dynamic)
		return;

	_NxActor->setAngularMomentum(NXHelper::MakeVector3(value));
}

void NXPhysicsBody::AddForceAtPosition(const Vector3& force, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;

	_NxActor->addForceAtPos(NXHelper::MakeVector3(force), NXHelper::MakeVector3(position));
}

void NXPhysicsBody::AddForceAtLocalPosition(const Vector3& force, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;

	_NxActor->addForceAtLocalPos(NXHelper::MakeVector3(force), NXHelper::MakeVector3(position));
}

void NXPhysicsBody::AddLocalForceAtPosition(const Vector3& force, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;

	_NxActor->addLocalForceAtPos(NXHelper::MakeVector3(force), NXHelper::MakeVector3(position));
}

void NXPhysicsBody::AddLocalForceAtLocalPosition(const Vector3& force, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;

	_NxActor->addLocalForceAtLocalPos(NXHelper::MakeVector3(force), NXHelper::MakeVector3(position));
}

void NXPhysicsBody::AddForce(const Vector3& force)
{
	if (_BodyType == BodyType_Static)
		return;

	_NxActor->addForce(NXHelper::MakeVector3(force));
}

void NXPhysicsBody::AddLocalForce(const Vector3& force)
{
	if (_BodyType == BodyType_Static)
		return;

	_NxActor->addLocalForce(NXHelper::MakeVector3(force));
}

void NXPhysicsBody::AddTorque(const Vector3& torque)
{
	if (_BodyType == BodyType_Static)
		return;

	_NxActor->addTorque(NXHelper::MakeVector3(torque));
}

void NXPhysicsBody::AddLocalTorque(const Vector3& torque)
{
	if (_BodyType == BodyType_Static)
		return;

	_NxActor->addLocalTorque(NXHelper::MakeVector3(torque));
}

void NXPhysicsBody::AddImpulseAtPosition(const Vector3& impulse, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;

	_NxActor->addForceAtPos(NXHelper::MakeVector3(impulse), NXHelper::MakeVector3(position), NX_IMPULSE);
}

void NXPhysicsBody::AddImpulseAtLocalPosition(const Vector3& impulse, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;

	_NxActor->addForceAtLocalPos(NXHelper::MakeVector3(impulse), NXHelper::MakeVector3(position), NX_IMPULSE);
}

void NXPhysicsBody::AddLocalImpulseAtPosition(const Vector3& impulse, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;

	_NxActor->addLocalForceAtPos(NXHelper::MakeVector3(impulse), NXHelper::MakeVector3(position), NX_IMPULSE);
}

void NXPhysicsBody::AddLocalImpulseAtLocalPosition(const Vector3& impulse, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;

	_NxActor->addLocalForceAtLocalPos(NXHelper::MakeVector3(impulse), NXHelper::MakeVector3(position), NX_IMPULSE);
}

void NXPhysicsBody::AddImpulse(const Vector3& impulse)
{
	if (_BodyType == BodyType_Static)
		return;

	_NxActor->addForce(NXHelper::MakeVector3(impulse), NX_IMPULSE);
}

void NXPhysicsBody::AddLocalImpulse(const Vector3& impulse)
{
	if (_BodyType == BodyType_Static)
		return;

	_NxActor->addLocalForce(NXHelper::MakeVector3(impulse), NX_IMPULSE);
}

void NXPhysicsBody::AddTorqueImpulse(const Vector3& torque)
{
	if (_BodyType == BodyType_Static)
		return;

	_NxActor->addTorque(NXHelper::MakeVector3(torque), NX_IMPULSE);
}

void NXPhysicsBody::AddLocalTorqueImpulse(const Vector3& torque)
{
	if (_BodyType == BodyType_Static)
		return;

	_NxActor->addLocalTorque(NXHelper::MakeVector3(torque), NX_IMPULSE);
}

void NXPhysicsBody::Update(real64 elapsed)
{
	if (_NxActor == NULL)
		return;
}

}
