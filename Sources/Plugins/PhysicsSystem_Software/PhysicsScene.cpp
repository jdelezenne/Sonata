/*=============================================================================
PhysicsScene.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "PhysicsScene.h"
#include "PhysicsBody.h"
#include "PhysicsMaterial.h"

namespace SE_Software
{

SpaceDivisionObject::SpaceDivisionObject() :
	Object()
{
}

SpaceDivisionObject::~SpaceDivisionObject()
{
}


SpaceDivision::SpaceDivision() :
	Object()
{
}

SpaceDivision::~SpaceDivision()
{
}


OctreeSpace::Node::Node()
{
}

OctreeSpace::OctreeSpace() :
	SpaceDivision()
{
}

OctreeSpace::~OctreeSpace()
{
}

void OctreeSpace::AddObject(SpaceDivisionObject* object)
{
}

void OctreeSpace::RemoveObject(SpaceDivisionObject* object)
{
}

void OctreeSpace::InternalAddNode(Node* node)
{
}

void OctreeSpace::InternalRemoveNode(Node* node)
{
}


PhysicsScene::PhysicsScene(PhysicsSystem* physicsSystem, const SceneDescription& desc) :
	Physics::IScene(),
	_PhysicsSystem(physicsSystem),
	_Description(desc)
{
	// Creates the default material
	IMaterial* material = CreateMaterial();
	material->SetDynamicFriction(0.5f);
	material->SetStaticFriction(0.5f);
	material->SetRestitution(0.5f);
	_DefaultMaterial = material->GetIndex();
}

PhysicsScene::~PhysicsScene()
{
}

void PhysicsScene::GenerateContactPairs()
{
	int i, j;

	int count = GetBodyCount();

	// Collision detection: broad phase
	//TODO: use an octree

	for (i=0; i<count; ++i)
	{
		IBody* bodyA = GetBody(i);

		if (!bodyA->GetEnabled())
			continue;

		for (j=0; j<i; ++j)
		{
			IBody* bodyB = GetBody(j);

			if (!bodyB->GetEnabled())
				continue;

			// If the bounding box overlaps, add a new contact pair
			// FIX: Planes have an empty AABB
			if (!_Description._BroadPhase ||
				(bodyA->GetWorldBoundingBox().IsEmpty() || bodyB->GetWorldBoundingBox().IsEmpty() ||
				bodyA->GetWorldBoundingBox().Intersects(bodyB->GetWorldBoundingBox())))
			{
				ContactPair pair;
				pair.SetBodyA(bodyA);
				pair.SetBodyB(bodyB);

				_ContactPairs.Add(pair);
			}
		}
	}
}

void PhysicsScene::GenerateContactInfos()
{
	int i, j;
	int count = _ContactPairs.Count();

	// Collision detection: narrow phase
	//TODO: Use the shape collision algorithms to calculate the contact points
	//Keep the pair with the largest separation value

	for (i=0; i<count; i++)
	{
		ContactPair pair = _ContactPairs[i];
		IBody* bodyA = pair.GetBodyA();
		IBody* bodyB = pair.GetBodyB();

		if (bodyA->Shapes().Count() == 0)
			continue;
		if (bodyB->Shapes().Count() == 0)
			continue;

		IShape* shapeA = bodyA->Shapes().GetItem(0);
		IShape* shapeB = bodyB->Shapes().GetItem(0);

		bool intersect = false;
		/*BaseArray<IntersectionContact> contacts;

		switch (shapeA->GetShapeType())
		{
		case ShapeType_Plane:
			switch (shapeB->GetShapeType())
			{
				case ShapeType_Plane:
					break;

				case ShapeType_Box:
					intersect = IntersectionBoxPlane::Contact(
						((IBoxShape*)shapeB)->Data, ((IPlaneShape*)shapeA)->Data, &contacts);
					break;

				case ShapeType_Sphere:
					intersect = IntersectionPlaneSphere::Contact(
						((IPlaneShape*)shapeA)->Data, ((ISphereShape*)shapeB)->Data, &contacts);
					break;
			}
			break;

		case ShapeType_Box:
			switch (shapeB->GetShapeType())
			{
				case ShapeType_Plane:
					intersect = IntersectionBoxPlane::Contact(
						((IBoxShape*)shapeA)->Data, ((IPlaneShape*)shapeB)->Data, &contacts);
					break;

				case ShapeType_Box:
					intersect = IntersectionBoxBox::Contact(
						((IBoxShape*)shapeA)->Data, ((IBoxShape*)shapeB)->Data, &contacts);
					break;

				case ShapeType_Sphere:
					intersect = IntersectionBoxSphere::Contact(
						((IBoxShape*)shapeA)->Data, ((ISphereShape*)shapeB)->Data, &contacts);
					break;
			}
			break;

		case ShapeType_Sphere:
			switch (shapeB->GetShapeType())
			{
				case ShapeType_Plane:
					intersect = IntersectionPlaneSphere::Contact(
						((IPlaneShape*)shapeB)->Data, ((ISphereShape*)shapeA)->Data, &contacts);
					break;

				case ShapeType_Box:
					intersect = IntersectionBoxSphere::Contact(
						((IBoxShape*)shapeB)->Data, ((ISphereShape*)shapeA)->Data, &contacts);
					break;

				case ShapeType_Sphere:
					intersect = IntersectionSphereSphere::Contact(
						((ISphereShape*)shapeA)->Data, ((ISphereShape*)shapeB)->Data, &contacts);
					break;
			}
			break;
		}

		if (intersect)
		{
			if (bodyA->GetBodyType() == BodyType_Dynamic)
				bodyA->SetActive(true);
			if (bodyB->GetBodyType() == BodyType_Dynamic)
				bodyB->SetActive(true);

			int contactCount = contacts.Count();
			for (j=0; j<contactCount; j++)
			{
				ContactInfo info;
				info.SetShapeA(shapeA);
				info.SetShapeB(shapeB);
				info.SetPosition(contacts[j]._Position);
				info.SetNormal(contacts[j]._Normal);
				info.SetSeparation(contacts[j]._Depth);

				IMaterial* materialA = shapeA->GetMaterial();
				IMaterial* materialB = shapeB->GetMaterial();
				info.SetMaterialA(materialA->GetIndex());
				info.SetMaterialB(materialB->GetIndex());
				info.SetRestitution((materialA->GetRestitution() + materialB->GetRestitution()) / 2.0);

				_ContactInfos.Add(info);
			}
		}*/
	}

	/*count = _ContactInfos.Count();
	if (count > 1)
	{
		Array<ContactInfo> contactInfos;
		for (j=0; j<count; j++)
		{
			ContactInfo info;
			info.SetShapeA(_ContactInfos[j].GetShapeA());
			info.SetShapeB(_ContactInfos[j].GetShapeB());

			info.SetMaterialA(_ContactInfos[j].GetMaterialA());
			info.SetMaterialB(_ContactInfos[j].GetMaterialB());

			Vector3 p = _ContactInfos[j].GetPosition();
			Vector3 n = _ContactInfos[j].GetNormal();
			real r = _ContactInfos[j].GetRestitution();
			real s = _ContactInfos[j].GetSeparation();
			int contacts = 1;
			for (i=j+1; i<count; i++)
			{
				if (info.GetShapeA() == _ContactInfos[i].GetShapeA())
				{
					p += _ContactInfos[i].GetPosition();
					n += _ContactInfos[i].GetNormal();
					r += _ContactInfos[i].GetRestitution();
					s += _ContactInfos[i].GetSeparation();
					contacts++;
				}
			}

			info.SetPosition(p / contacts);
			info.SetNormal(Vector3::Normalize(n / contacts));
			info.SetRestitution(r / contacts);
			info.SetSeparation(s / contacts);

			contactInfos.Add(info);
		}
		_ContactInfos = contactInfos;
	}*/
}

void PhysicsScene::ResolveCollisions(real64 elapsed)
{
	int i;
	int count = _ContactInfos.Count();

	//TODO: Calculate the forces: contact, friction

	for (i=0; i<count; i++)
	{
		ContactInfo info = _ContactInfos[i];

		//todo: Cancel
		if (!PreContact.Invoke(this, ContactEventArgs(info)))
			continue;

		PhysicsBody* bodyA = (PhysicsBody*)info.GetShapeA()->GetBody();
		PhysicsBody* bodyB = (PhysicsBody*)info.GetShapeB()->GetBody();

		IMaterial* materialA = GetMaterial(info.GetMaterialA());
		IMaterial* materialB = GetMaterial(info.GetMaterialB());

		Vector3 relPosA = info.GetPosition() - bodyA->GetPosition();
		Vector3 relPosB = info.GetPosition() - bodyB->GetPosition();

		Vector3 relVel = bodyA->GetVelocityAtLocalPosition(relPosA) -
			bodyB->GetVelocityAtLocalPosition(relPosB);
		real normalVel = Vector3::Dot(relVel, info.GetNormal());

		real impulseNumerator, impulseDenominator;
		if (!Math::Equals(normalVel, 0.0, 0.05))
		{
			//bodyA->SetActive(false);
			//bodyB->SetActive(false);

			if (elapsed > 0.0)
			{
				real Kerp = 0.4;
				real Kfps = 1.0 / elapsed;
				real Kcor = Kerp * Kfps;
				real positionalError = Kcor * -info.GetSeparation();
				real penetrationImpulse = positionalError * 1.0;
				normalVel += penetrationImpulse;
			}

			// Calculate the contact impulse
			impulseNumerator = -(1.0 + info.GetRestitution()) * normalVel;

			impulseDenominator = bodyA->GetInverseMass() +
				Vector3::Dot(Vector3::Cross(bodyA->GetInverseWorldInertiaTensor() *
				Vector3::Cross(relPosA, info.GetNormal()), relPosA), info.GetNormal());

			if (bodyB->GetBodyType() != BodyType_Static)
			{
				impulseDenominator +=  bodyB->GetInverseMass() +
					Vector3::Dot(Vector3::Cross(bodyB->GetInverseWorldInertiaTensor() *
					Vector3::Cross(relPosB, info.GetNormal()), relPosB), info.GetNormal());
			}

			if (impulseDenominator > Math::Epsilon)
			{
				Vector3 impulse = (impulseNumerator / impulseDenominator) * info.GetNormal();

				//Console::WriteLine(impulse.ToString());
				bodyA->AddImpulseAtLocalPosition(impulse, relPosA);

				if (bodyB->GetBodyType() != BodyType_Static)
				{
					bodyB->AddImpulseAtLocalPosition(-impulse, relPosB);
				}
			}
		}

		// Calculate the friction impulse

		// Get the new velocity
		relVel = bodyA->GetVelocityAtLocalPosition(relPosA) -
			bodyB->GetVelocityAtLocalPosition(relPosB);
		Vector3 tangentVel = relVel - Vector3::Dot(relVel, info.GetNormal()) * info.GetNormal();

		real tangentSpeed = tangentVel.Length();
		if (tangentSpeed < 0.05)
			continue;

		Vector3 tangent = -tangentVel / tangentSpeed;

		impulseDenominator = 0.0;
		if (bodyA->GetBodyType() != BodyType_Static)
		{
			impulseDenominator = bodyA->GetInverseMass() + 
				Vector3::Dot(tangent, Vector3::Cross(bodyA->GetInverseWorldInertiaTensor() *
				(Vector3::Cross(relPosA, tangent)), relPosA));
		}

		if (bodyB->GetBodyType() != BodyType_Static)
		{
			impulseDenominator += bodyB->GetInverseMass() + 
				Vector3::Dot(tangent, Vector3::Cross(bodyB->GetInverseWorldInertiaTensor() *
				(Vector3::Cross(relPosB, tangent)), relPosB));
		}

		if (impulseDenominator > Math::Epsilon)
		{
			real impulseToReverse = tangentSpeed / impulseDenominator;

			real impulseFromNormalImpulse = materialA->GetStaticFriction() * normalVel;
			real frictionImpulse;

			if (impulseToReverse < impulseFromNormalImpulse)
				frictionImpulse = impulseToReverse;
			else
				frictionImpulse = materialA->GetDynamicFriction() * normalVel;

			tangent *= frictionImpulse;

			bodyA->AddImpulseAtLocalPosition(tangent, relPosA);

			if (bodyB->GetBodyType() != BodyType_Static)
			{
				bodyB->AddImpulseAtLocalPosition(-tangent, relPosB);
			}
		}

		PostContact.Invoke(this, ContactEventArgs(info));
	}
}

SceneDescription PhysicsScene::GetDescription() const
{
	return _Description;
}

Vector3 PhysicsScene::GetGravity() const
{
	return _Description._Gravity;
}

void PhysicsScene::SetGravity(const Vector3& value)
{
	_Description._Gravity = value;
}

TimeStepType PhysicsScene::GetTimeStepType() const
{
	return _Description._TimeStepType;
}

void PhysicsScene::SetTimeStepType(TimeStepType value)
{
	_Description._TimeStepType = value;
}

real PhysicsScene::GetTimeStep() const
{
	return _Description._TimeStep;
}

void PhysicsScene::SetTimeStep(real value)
{
	_Description._TimeStep = value;
}

uint32 PhysicsScene::GetIterations() const
{
	return _Description._Iterations;
}

void PhysicsScene::SetIterations(uint32 value)
{
	_Description._Iterations = value;
}

IBody* PhysicsScene::CreateBody(const BodyDescription& desc)
{
	if (_Description._MaxBodies > 0 && GetBodyCount() == _Description._MaxBodies)
		return NULL;

	IBody* body = new PhysicsBody(this, desc);
	_Bodies.Add(body);
	return body;
}

void PhysicsScene::DestroyBody(IBody* body)
{
	if (_Bodies.Contains(body))
	{
		_Bodies.Remove(body);
	}
}

int PhysicsScene::GetBodyCount() const
{
	return _Bodies.Count();
}

IBody* PhysicsScene::GetBody(int index) const
{
	if (index < 0 || index >= _Bodies.Count())
		return NULL;

	return _Bodies[index];
}

IMaterial* PhysicsScene::CreateMaterial()
{
	IMaterial* material = new PhysicsMaterial(this);
	int count = _Materials.Keys().Count();
	material->SetIndex(count);
	_Materials[count] = material;
	return material;
}

void PhysicsScene::DestroyMaterial(MaterialIndex material)
{
	_Materials.Remove(material);
}

IMaterial* PhysicsScene::GetMaterial(MaterialIndex index) const
{
	return _Materials[index];
}

MaterialIndex PhysicsScene::GetDefaultMaterial() const
{
	return _DefaultMaterial;
}

void PhysicsScene::SetDefaultMaterial(MaterialIndex value)
{
	_DefaultMaterial = value;
}

IJoint* PhysicsScene::CreateJoint()
{
	if (_Description._MaxJoints > 0)// && GetJointCount() == _Description._MaxJoints)
		return NULL;

	IJoint* joint = NULL;//new PhysicsJoint(this);
	return joint;
}

void PhysicsScene::DestroyJoint(IJoint* joint)
{
	if (_Joints.Contains(joint))
	{
		_Joints.Remove(joint);
	}
}

void PhysicsScene::Update(real64 elapsed)
{
	uint i;

	real64 delta;
	static real64 lastTime = 0.0;
	if (_Description._TimeStepType == TimeStepType_Fixed)
	{
		lastTime += elapsed;
		if (lastTime > _Description._TimeStep)
			lastTime -= _Description._TimeStep;
		else
			return;
		delta = _Description._TimeStep;
	}
	else
	{
		delta = elapsed;
	}

	/*{
		Array<BodyPtr>::Iterator it = _Bodies.GetIterator();
		while (it.Next())
		{
			((PhysicsBody*)it.Current().Get())->UpdateVelocity(elapsed);
		}
	}*/

	// Two steps: collision detection and collision response
	if (_Description._CollisionDetection)
	{
		GenerateContactPairs();
		GenerateContactInfos();

		// Collision response
		_Description._Iterations = 1;
		real64 delta = elapsed;// / (real64)_Description._Iterations;
		for (i=0; i<_Description._Iterations; i++)
		{
			ResolveCollisions(delta);
		}
	}

	// Update the state of the bodies
	{
		Array<BodyPtr>::Iterator it = _Bodies.GetIterator();
		while (it.Next())
		{
			IBody* body = it.Current().Get();
			//body->AddForce(body->GetMass() * GetGravity());
			//((PhysicsBody*)body)->UpdateVelocity(elapsed);
			if (body->GetActive())
			{
				body->Update(elapsed);
			}

			/*if (body->GetBodyType() == BodyType_Dynamic)
			{
				if (body->GetLinearVelocity().Equals(Vector3::Zero, 0.05) &&
					body->GetAngularVelocity().Equals(Vector3::Zero, 0.05))
				{
					body->SetActive(false);
					body->SetLinearVelocity(Vector3::Zero);
					body->SetAngularVelocity(Vector3::Zero);
				}
				else if (!body->GetActive())
				{
					body->SetActive(true);
				}
			}*/
		}
	}

	_ContactPairs.Clear();
	_ContactInfos.Clear();

	//TODO: Determines the bodies that need to be deactived
}

}
