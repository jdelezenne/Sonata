/*=============================================================================
RWPhysicsBody.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "RWPhysicsBody.h"
#include "RWPhysicsMaterial.h"

namespace SE_RW
{

RWPhysicsBody::RWPhysicsBody(RWPhysicsScene* scene, const BodyDescription& desc) :
	Physics::IBody()
{
	_Scene = scene;
	_BodyType = desc._BodyType;
	_Shapes = desc._Shapes;

	_RwpBody = NULL;
	_RwpVolume = NULL;

	CreateFromShapes();
}

RWPhysicsBody::~RWPhysicsBody()
{
	if (_BodyType == BodyType_Static && _RwpVolume != NULL)
	{
		RwpContextRemoveFixedVolume(_Scene->GetRwpContext(), _RwpVolume);
		RwpVolumeDestroy(_RwpVolume);

		_RwpVolume = NULL;
	}
	else if (_BodyType == BodyType_Dynamic && _RwpBody != NULL)
	{
		RwpObj* obj = (RwpObj*)_RwpBody;
		RwpMgrRemoveObj(RwpContextGetMgr(_Scene->GetRwpContext()), obj);

		switch (RwpObjGetTypeId(obj))
		{
		case rwpOBJTYPEBODY:
			RwpBodyDestroy(RwpObjToBody(obj));
			break;
		case rwpOBJTYPERAGDOLL:
			RwpRagdollDestroy(RwpObjToRagdoll(obj));
			break;
		default:
			RwpGenericDestroy(RwpObjToGeneric(obj));
		}

		_RwpBody = NULL;
	}
}

void RWPhysicsBody::CreateFromShapes()
{
	if (_Shapes.IsEmpty())
		return;

	IShape* shape = _Shapes[0].Get();

	RwpBodyDef* bodyDef = NULL;

	switch (shape->GetShapeType())
	{
	case ShapeType_Plane:
		{
		IPlaneShape* plane = (IPlaneShape*)shape;

		_RwpVolume = RwpPlaneCreate();
		//RwMatrixRotate(&rotateTM, &XAxis, -90.0f, rwCOMBINEREPLACE);
		//RwpVolumeSetRelativeTransform(plane, &rotateTM);
		}
		break;

	case ShapeType_Box:
		{
		IBoxShape* box = (IBoxShape*)shape;

		bodyDef = RwpBodyDefCreate();
		RwpPartDef* partDef = RwpBodyDefGetPartDef(bodyDef);

		_RwpVolume = RwpPartDefGetVolume(partDef);

		RwV3d size;
		RWHelper::MakeVector3(&size, box->Data.GetExtents());
		RwpBoxInit(_RwpVolume, &size);

		//RwpVolumeSetRadius(vol, radius);
		RwpPartDefSetMassProperties(partDef, -1.0f, TRUE);
		}
		break;

	case ShapeType_Sphere:
		{
		ISphereShape* sphere = (ISphereShape*)shape;
		
		bodyDef = RwpBodyDefCreate();
		RwpPartDef* partDef = RwpBodyDefGetPartDef(bodyDef);

		_RwpVolume = RwpPartDefGetVolume(partDef);
		RwpSphereInit(_RwpVolume, sphere->Data.Radius);

		RwpPartDefSetMassProperties(partDef, -1.0f, TRUE);
		}
		break;

	case ShapeType_Capsule:
		{
		ICapsuleShape* capsule = (ICapsuleShape*)shape;
		
		bodyDef = RwpBodyDefCreate();
		RwpPartDef* partDef = RwpBodyDefGetPartDef(bodyDef);

		_RwpVolume = RwpPartDefGetVolume(partDef);
		RwpCapsuleInit(_RwpVolume, capsule->Data.GetRadius(), capsule->Data.GetHeight() / 2);

		RwpPartDefSetMassProperties(partDef, -1.0f, TRUE);
		}
		break;

/*
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
		_RwpBody->createShape(desc);
		}
		break;
*/
	default:
		return;
	}

	if (_BodyType == BodyType_Static)
	{
		RwpContextAddFixedVolume(_Scene->GetRwpContext(), _RwpVolume, NULL);
	}
	else if (_BodyType == BodyType_Dynamic && bodyDef != NULL)
	{
		_RwpBody = RwpBodyCreate(bodyDef);

		RwpMgrInsertBody(RwpContextGetMgr(_Scene->GetRwpContext()), _RwpBody, rwpMGRNEXTINDEX,
			rwpMGRINSERTMOVING | rwpMGRINSERTENABLED | rwpMGRINSERTACTIVE);
		
		RwpBodyDefDestroy(bodyDef);

		// Material
		RwpVolumeSetBounce(_RwpVolume, 1.0f);
	}
}

Vector3 RWPhysicsBody::GetPosition() const
{
	RwMatrix* tm;

	if (_BodyType == BodyType_Static)
	{
		tm = RwpVolumeGetRelativeTransform(_RwpVolume);
	}
	else
	{
		tm = RwpDynGetWorldCoMTransform(RwpBodyGetDyn(_RwpBody));
	}

	return RWHelper::MakeVector3(RwMatrixGetPos(tm));
}

void RWPhysicsBody::SetPosition(const Vector3& value)
{
	if (_BodyType == BodyType_Static)
	{
		RwMatrix* tm = RwpVolumeGetRelativeTransform(_RwpVolume);
		RwV3dSet(RwMatrixGetPos(tm), value.X, value.Y, value.Z);
		RwMatrixUpdate(tm);

		RwpVolumeSetRelativeTransform(_RwpVolume, tm);
	}
	else
	{
		RwMatrix* tm = RwpBodyGetTransformPtr(_RwpBody);
		RwV3dSet(RwMatrixGetPos(tm), value.X, value.Y, value.Z);
		RwMatrixUpdate(tm);

		RwpBodySyncTransform(_RwpBody);
		if (RwpBodyGetMgr(_RwpBody) != NULL)
		{
			RwpBodyUpdateBBox(_RwpBody);
			RwpBodySetMoving(_RwpBody, TRUE);
		}
	}
}

Quaternion RWPhysicsBody::GetOrientation() const
{
	RwMatrix* tm;

	if (_BodyType == BodyType_Static)
	{
		tm = RwpVolumeGetRelativeTransform(_RwpVolume);
	}
	else
	{
		tm = RwpBodyGetTransformPtr(_RwpBody);
	}

	//TODO
	return Quaternion::Identity;
}

Matrix3 RWPhysicsBody::GetOrientationMatrix() const
{
	Quaternion q = GetOrientation();
	Matrix3 m;
	q.ToRotationMatrix(m);
	return m;
}

void RWPhysicsBody::SetOrientation(const Quaternion& value)
{
	Vector3 axis;
	real angle;
	value.ToAxisAngle(axis, angle);

	RwV3d savepos, rwaxis;
	RwMatrix* tm;

	if (_BodyType == BodyType_Static)
	{
		tm = RwpVolumeGetRelativeTransform(_RwpVolume);
	}
	else
	{
		tm = RwpBodyGetTransformPtr(_RwpBody);
	}

	RwV3dSet(&rwaxis, axis.X, axis.Y, axis.Z);
	RwV3dNormalize(&rwaxis, &rwaxis);
	RwV3dAssign(&savepos, RwMatrixGetPos(tm));
	RwMatrixRotate(tm, &rwaxis, angle * 180.0f/rwPI, rwCOMBINEREPLACE);
	RwV3dAssign(RwMatrixGetPos(tm), &savepos);
	RwMatrixUpdate(tm);

	if (_BodyType == BodyType_Static)
	{
		RwpVolumeSetRelativeTransform(_RwpVolume, tm);
	}
	else
	{
		RwpBodySyncTransform(_RwpBody);
		if (RwpBodyGetMgr(_RwpBody) != NULL)
		{
			RwpBodyUpdateBBox(_RwpBody);
			RwpBodySetMoving(_RwpBody, TRUE);
		}
	}
}

void RWPhysicsBody::SetOrientationMatrix(const Matrix3& value)
{
	Quaternion q = Quaternion::FromRotationMatrix(value);
	SetOrientation(q);
}

BodyGroup RWPhysicsBody::GetGroup() const
{
	if (_RwpBody == NULL || _BodyType == BodyType_Static)
		return (BodyGroup)0;

	return (BodyGroup)RwpBodyGetGroup(RwpBodyGetDyn(_RwpBody));
}

void RWPhysicsBody::SetGroup(BodyGroup value)
{
	if (_RwpBody == NULL || _BodyType == BodyType_Static)
		return;

	RwpBodySetGroup(RwpBodyGetDyn(_RwpBody), (RwUInt16)value);
}

bool RWPhysicsBody::GetEnabled() const
{
	if (_RwpBody == NULL || _BodyType == BodyType_Static)
		return false;

	return RwpBodyIsEnabled(RwpBodyGetDyn(_RwpBody));
}

void RWPhysicsBody::SetEnabled(bool value)
{
	if (_RwpBody == NULL || _BodyType == BodyType_Static)
		return;

	RwpBodySetEnabled(RwpBodyGetDyn(_RwpBody), value);
}

bool RWPhysicsBody::GetActive() const
{
	if (_RwpBody == NULL || _BodyType == BodyType_Static)
		return false;

	return RwpBodyIsActive(RwpBodyGetDyn(_RwpBody));
}

void RWPhysicsBody::SetActive(bool value)
{
	if (_RwpBody == NULL || _BodyType == BodyType_Static)
		return;

	if (value)
		RwpBodyActivate(RwpBodyGetDyn(_RwpBody));
	else
		RwpBodyDeactivate(RwpBodyGetDyn(_RwpBody));
}

Vector3 RWPhysicsBody::GetCOMLocalPosition() const
{
	if (_BodyType == BodyType_Static)
		return Vector3::Zero;

	return RWHelper::MakeVector3(RwpDynGetRelativeCoM(RwpBodyGetDyn(_RwpBody)));
}

Quaternion RWPhysicsBody::GetCOMLocalOrientation() const
{
	if (_BodyType == BodyType_Static)
		return Quaternion::Identity;

	//TODO
	return Quaternion::Identity;
}

Vector3 RWPhysicsBody::GetCOMWorldPosition() const
{
	if (_BodyType == BodyType_Static)
		return Vector3::Zero;

	return RWHelper::MakeVector3(RwpDynGetCoMWorldPosition(RwpBodyGetDyn(_RwpBody)));
}

void RWPhysicsBody::SetCOMWorldPosition(const Vector3& value)
{
	if (_BodyType == BodyType_Static)
		return;

	//TODO
}

Quaternion RWPhysicsBody::GetCOMWorldOrientation() const
{
	if (_BodyType == BodyType_Static)
		return Quaternion::Identity;

	//TODO
	return Quaternion::Identity;
}

void RWPhysicsBody::SetCOMWorldOrientation(const Quaternion& value)
{
	if (_BodyType == BodyType_Static)
		return;

	//TODO
}

void RWPhysicsBody::SetCOMOffsetLocalPosition(const Vector3& value)
{
	if (_BodyType == BodyType_Static)
		return;

	//TODO
}

void RWPhysicsBody::SetCOMOffsetLocalOrientation(const Quaternion& value)
{
	if (_BodyType == BodyType_Static)
		return;

	//TODO
}

void RWPhysicsBody::SetCOMOffsetWorldPosition(const Vector3& value)
{
	if (_BodyType == BodyType_Static)
		return;

	//TODO
}

void RWPhysicsBody::SetCOMOffsetWorldOrientation(const Quaternion& value)
{
	if (_BodyType == BodyType_Static)
		return;

	//TODO
}

real RWPhysicsBody::GetMass() const
{
	if (_BodyType == BodyType_Static)
		return 0.0;

	return RwpDynGetMass(RwpBodyGetDyn(_RwpBody));
}

void RWPhysicsBody::SetMass(real value)
{
	if (_BodyType == BodyType_Static)
		return;

	RwpDynSetMass(RwpBodyGetDyn(_RwpBody), value);
}

Vector3 RWPhysicsBody::GetInertiaTensor() const
{
	if (_BodyType == BodyType_Static)
		return Vector3::Zero;

	RwpInertiaTensor tensor;
	RwpDynGetInertiaTensor(RwpBodyGetDyn(_RwpBody), &tensor);
	//TODO
	return Vector3::Zero;
}

void RWPhysicsBody::SetInertiaTensor(const Vector3& value)
{
	if (_BodyType == BodyType_Static)
		return;

	RwpInertiaTensor tensor;
	//TODO
	RwpDynSetNonSphericalInertiaTensor(RwpBodyGetDyn(_RwpBody), &tensor);
}

Matrix3 RWPhysicsBody::GetWorldInertiaTensor() const
{
	if (_BodyType == BodyType_Static)
		return Matrix3::Identity;

	//TODO
	return Matrix3::Identity;
}

Matrix3 RWPhysicsBody::GetInverseWorldInertiaTensor() const
{
	if (_BodyType == BodyType_Static)
		return Matrix3::Identity;

	//TODO
	return Matrix3::Identity;
}

void RWPhysicsBody::SetMassFromShape(real density, real totalMass)
{
	if (_BodyType == BodyType_Static)
		return;

	//TODO
}

real RWPhysicsBody::GetLinearDamping() const
{
	if (_BodyType == BodyType_Static)
		return 0.0;

	return RwpDynGetLinearDamping(RwpBodyGetDyn(_RwpBody));
}

void RWPhysicsBody::SetLinearDamping(real value)
{
	if (_BodyType == BodyType_Static)
		return;

	RwpDynSetLinearDamping(RwpBodyGetDyn(_RwpBody), value);
}

real RWPhysicsBody::GetAngularDamping() const
{
	if (_BodyType == BodyType_Static)
		return 0.0;

	return RwpDynGetAngularDamping(RwpBodyGetDyn(_RwpBody));
}

void RWPhysicsBody::SetAngularDamping(real value)
{
	if (_BodyType == BodyType_Static)
		return;

	RwpDynSetAngularDamping(RwpBodyGetDyn(_RwpBody), value);
}

Vector3 RWPhysicsBody::GetLinearVelocity() const
{
	if (_BodyType == BodyType_Static)
		return Vector3::Zero;

	return RWHelper::MakeVector3(RwpDynGetLinearVelocity(RwpBodyGetDyn(_RwpBody)));
}

void RWPhysicsBody::SetLinearVelocity(const Vector3& value)
{
	if (_BodyType == BodyType_Static)
		return;

	RwV3d vel;
	RWHelper::MakeVector3(&vel, value);
	RwpDynSetLinearVelocity(RwpBodyGetDyn(_RwpBody), &vel);
}

Vector3 RWPhysicsBody::GetAngularVelocity() const
{
	if (_BodyType == BodyType_Static)
		return Vector3::Zero;

	return RWHelper::MakeVector3(RwpDynGetAngularVelocity(RwpBodyGetDyn(_RwpBody)));
}

void RWPhysicsBody::SetAngularVelocity(const Vector3& value)
{
	if (_BodyType == BodyType_Static)
		return;

	RwV3d vel;
	RWHelper::MakeVector3(&vel, value);
	RwpDynSetAngularVelocity(RwpBodyGetDyn(_RwpBody), &vel);
}

real RWPhysicsBody::GetMaxAngularVelocity() const
{
	return 0.0;
}

void RWPhysicsBody::SetMaxAngularVelocity(real value)
{
}

Vector3 RWPhysicsBody::GetVelocityAtWorldPosition(const Vector3& value) const
{
	if (_BodyType == BodyType_Static)
		return Vector3::Zero;

	RwV3d pos;
	RWHelper::MakeVector3(&pos, value);
	RwV3d vel;
	return RWHelper::MakeVector3(RwpDynGetLinearVelocityAtPosition(
		RwpBodyGetDyn(_RwpBody), &pos, &vel));
}

Vector3 RWPhysicsBody::GetVelocityAtLocalPosition(const Vector3& value) const
{
	if (_BodyType == BodyType_Static)
		return Vector3::Zero;

	//TODO: transform
	RwV3d pos;
	RWHelper::MakeVector3(&pos, value);
	RwV3d vel;
	return RWHelper::MakeVector3(RwpDynGetLinearVelocityAtPosition(
		RwpBodyGetDyn(_RwpBody), &pos, &vel));
}

Vector3 RWPhysicsBody::GetLinearMomentum() const
{
	if (_BodyType == BodyType_Static)
		return Vector3::Zero;

	//TODO
	return Vector3::Zero;
}

void RWPhysicsBody::SetLinearMomentum(const Vector3& value)
{
	if (_BodyType == BodyType_Static)
		return;

	//TODO
}

Vector3 RWPhysicsBody::GetAngularMomentum() const
{
	if (_BodyType == BodyType_Static)
		return Vector3::Zero;

	//TODO
	return Vector3::Zero;
}

void RWPhysicsBody::SetAngularMomentum(const Vector3& value)
{
	if (_BodyType == BodyType_Static)
		return;

	//TODO
}

void RWPhysicsBody::AddForceAtPosition(const Vector3& force, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;

	RwV3d f;
	RWHelper::MakeVector3(&f, force);

	RwV3d pos;
	RWHelper::MakeVector3(&pos, position);

	RwpDynAddForceAtPosition(RwpBodyGetDyn(_RwpBody), &f, &pos);
}

void RWPhysicsBody::AddForceAtLocalPosition(const Vector3& force, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;

	RwV3d f;
	RWHelper::MakeVector3(&f, force);

	RwV3d pos;
	RWHelper::MakeVector3(&pos, position);

	//TODO: transform
	RwpDynAddForceAtPosition(RwpBodyGetDyn(_RwpBody), &f, &pos);
}

void RWPhysicsBody::AddLocalForceAtPosition(const Vector3& force, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;

	RwV3d f;
	RWHelper::MakeVector3(&f, force);

	RwV3d pos;
	RWHelper::MakeVector3(&pos, position);

	//TODO: transform
	RwpDynAddForceAtPosition(RwpBodyGetDyn(_RwpBody), &f, &pos);
}

void RWPhysicsBody::AddLocalForceAtLocalPosition(const Vector3& force, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;

	RwV3d f;
	RWHelper::MakeVector3(&f, force);

	RwV3d pos;
	RWHelper::MakeVector3(&pos, position);

	//TODO: transform
	RwpDynAddForceAtPosition(RwpBodyGetDyn(_RwpBody), &f, &pos);
}

void RWPhysicsBody::AddForce(const Vector3& force)
{
	if (_BodyType == BodyType_Static)
		return;

	Vector3 current = RWHelper::MakeVector3(RwpDynGetExternalForce(RwpBodyGetDyn(_RwpBody)));
	RwV3d f;
	RWHelper::MakeVector3(&f, current + force);
	RwpDynSetExternalForce(RwpBodyGetDyn(_RwpBody), &f);
}

void RWPhysicsBody::AddLocalForce(const Vector3& force)
{
	if (_BodyType == BodyType_Static)
		return;

	Vector3 current = RWHelper::MakeVector3(RwpDynGetExternalForce(RwpBodyGetDyn(_RwpBody)));
	//TODO: transform
	RwV3d f;
	RWHelper::MakeVector3(&f, current + force);
	RwpDynSetExternalForce(RwpBodyGetDyn(_RwpBody), &f);
}

void RWPhysicsBody::AddTorque(const Vector3& torque)
{
	if (_BodyType == BodyType_Static)
		return;

	Vector3 current = RWHelper::MakeVector3(RwpDynGetExternalTorque(RwpBodyGetDyn(_RwpBody)));
	RwV3d t;
	RWHelper::MakeVector3(&t, current + torque);
	RwpDynSetExternalTorque(RwpBodyGetDyn(_RwpBody), &t);
}

void RWPhysicsBody::AddLocalTorque(const Vector3& torque)
{
	if (_BodyType == BodyType_Static)
		return;

	Vector3 current = RWHelper::MakeVector3(RwpDynGetExternalTorque(RwpBodyGetDyn(_RwpBody)));
	//TODO: transform
	RwV3d t;
	RWHelper::MakeVector3(&t, current + torque);
	RwpDynSetExternalTorque(RwpBodyGetDyn(_RwpBody), &t);
}

void RWPhysicsBody::AddImpulseAtPosition(const Vector3& impulse, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;

	RwV3d f;
	RWHelper::MakeVector3(&f, impulse);

	RwV3d pos;
	RWHelper::MakeVector3(&pos, position);

	RwpDynAddForceAtPosition(RwpBodyGetDyn(_RwpBody), &f, &pos);
}

void RWPhysicsBody::AddImpulseAtLocalPosition(const Vector3& impulse, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;

	RwV3d f;
	RWHelper::MakeVector3(&f, impulse);

	RwV3d pos;
	RWHelper::MakeVector3(&pos, position);

	//TODO: transform
	RwpDynAddForceAtPosition(RwpBodyGetDyn(_RwpBody), &f, &pos);
}

void RWPhysicsBody::AddLocalImpulseAtPosition(const Vector3& impulse, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;

	RwV3d f;
	RWHelper::MakeVector3(&f, impulse);

	RwV3d pos;
	RWHelper::MakeVector3(&pos, position);

	//TODO: transform
	RwpDynAddForceAtPosition(RwpBodyGetDyn(_RwpBody), &f, &pos);
}

void RWPhysicsBody::AddLocalImpulseAtLocalPosition(const Vector3& impulse, const Vector3& position)
{
	if (_BodyType == BodyType_Static)
		return;

	RwV3d f;
	RWHelper::MakeVector3(&f, impulse);

	RwV3d pos;
	RWHelper::MakeVector3(&pos, position);

	//TODO: transform
	RwpDynAddForceAtPosition(RwpBodyGetDyn(_RwpBody), &f, &pos);
}

void RWPhysicsBody::AddImpulse(const Vector3& impulse)
{
	if (_BodyType == BodyType_Static)
		return;

	Vector3 current = RWHelper::MakeVector3(RwpDynGetExternalForce(RwpBodyGetDyn(_RwpBody)));
	RwV3d f;
	RWHelper::MakeVector3(&f, current + impulse);
	RwpDynSetExternalForce(RwpBodyGetDyn(_RwpBody), &f);
}

void RWPhysicsBody::AddLocalImpulse(const Vector3& impulse)
{
	if (_BodyType == BodyType_Static)
		return;

	Vector3 current = RWHelper::MakeVector3(RwpDynGetExternalForce(RwpBodyGetDyn(_RwpBody)));
	//TODO: transform
	RwV3d f;
	RWHelper::MakeVector3(&f, current + impulse);
	RwpDynSetExternalForce(RwpBodyGetDyn(_RwpBody), &f);
}

void RWPhysicsBody::AddTorqueImpulse(const Vector3& torque)
{
	if (_BodyType == BodyType_Static)
		return;

	Vector3 current = RWHelper::MakeVector3(RwpDynGetExternalTorque(RwpBodyGetDyn(_RwpBody)));
	RwV3d t;
	RWHelper::MakeVector3(&t, current + torque);
	RwpDynSetExternalTorque(RwpBodyGetDyn(_RwpBody), &t);
}

void RWPhysicsBody::AddLocalTorqueImpulse(const Vector3& torque)
{
	if (_BodyType == BodyType_Static)
		return;

	Vector3 current = RWHelper::MakeVector3(RwpDynGetExternalTorque(RwpBodyGetDyn(_RwpBody)));
	//TODO: transform
	RwV3d t;
	RWHelper::MakeVector3(&t, current + torque);
	RwpDynSetExternalTorque(RwpBodyGetDyn(_RwpBody), &t);
}

void RWPhysicsBody::Update(real64 elapsed)
{
	if (_RwpBody == NULL)
		return;
}

}
