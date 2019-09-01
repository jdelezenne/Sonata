/*=============================================================================
ODEPhysicsScene.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ODEPhysicsScene.h"
#include "ODEPhysicsBody.h"
#include "ODEPhysicsMaterial.h"

namespace SE_ODE
{

ODEPhysicsScene::ODEPhysicsScene(ODEPhysicsSystem* physicsSystem, const SceneDescription& desc) :
	Physics::IScene(),
	_PhysicsSystem(physicsSystem),
	_Description(desc)
{

	_WorldID = dWorldCreate();
	if (_WorldID == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("ODEPhysicsScene"),
			_T("Failed to create the scene."));
	}

	SetGravity(PhysicsSystem::DefaultGravity);
	dWorldSetAutoDisableFlag(_WorldID, 1);

	// Create the collision space
	_SpaceID = dHashSpaceCreate(0);

	// Create the collision contact joint group
	_CollisionJointGroup = dJointGroupCreate(1000000);

	_DefaultMaterial = (new ODEPhysicsMaterial(this))->GetIndex();
}

ODEPhysicsScene::~ODEPhysicsScene()
{
	if (_WorldID != NULL)
	{
		dWorldDestroy(_WorldID);
		_WorldID = NULL;
	}

	if (_SpaceID != NULL)
	{
		dSpaceDestroy(_SpaceID);
		_SpaceID = NULL;
	}
}

SceneDescription ODEPhysicsScene::GetDescription() const
{
	return _Description;
}

Vector3 ODEPhysicsScene::GetGravity() const
{
	if (_WorldID == NULL)
		return Vector3::Zero;

	dVector3 result;
	dWorldGetGravity(_WorldID, result);
	return ODEHelper::MakeVector3(result);
}

TimeStepType ODEPhysicsScene::GetTimeStepType() const
{
	return _Description._TimeStepType;
}

void ODEPhysicsScene::SetTimeStepType(TimeStepType value)
{
	_Description._TimeStepType = value;
}

real ODEPhysicsScene::GetTimeStep() const
{
	return _Description._TimeStep;
}

void ODEPhysicsScene::SetTimeStep(real value)
{
	_Description._TimeStep = value;
}

uint32 ODEPhysicsScene::GetIterations() const
{
	return _Description._Iterations;
}

void ODEPhysicsScene::SetIterations(uint32 value)
{
	_Description._Iterations = value;
}

void ODEPhysicsScene::SetGravity(const Vector3& value)
{
	if (_WorldID == NULL)
		return;

	dWorldSetGravity(_WorldID, value.X, value.Y, value.Z);
}

IBody* ODEPhysicsScene::CreateBody(const BodyDescription& desc)
{
	IBody* body = new ODEPhysicsBody(this, desc);
	_Bodies.Add(body);
	return body;
}

void ODEPhysicsScene::DestroyBody(IBody* body)
{
	if (_Bodies.Contains(body))
	{
		_Bodies.Remove(body);
	}
}

int ODEPhysicsScene::GetBodyCount() const
{
	return _Bodies.Count();
}

IBody* ODEPhysicsScene::GetBody(int index) const
{
	if (index < 0 || index >= _Bodies.Count())
		return NULL;

	return _Bodies[index];
}

IMaterial* ODEPhysicsScene::CreateMaterial()
{
	IMaterial* material = new ODEPhysicsMaterial(this);
	return material;
}

void ODEPhysicsScene::DestroyMaterial(MaterialIndex material)
{
	_Materials.Remove(material);
}

IMaterial* ODEPhysicsScene::GetMaterial(MaterialIndex index) const
{
	return _Materials[index];
}

MaterialIndex ODEPhysicsScene::GetDefaultMaterial() const
{
	return _DefaultMaterial;
}

void ODEPhysicsScene::SetDefaultMaterial(MaterialIndex value)
{
	_DefaultMaterial = value;
}

IJoint* ODEPhysicsScene::CreateJoint()
{
	IJoint* joint = NULL;//new ODEPhysicsJoint(this);
	return joint;
}

void ODEPhysicsScene::DestroyJoint(IJoint* joint)
{
	if (_Joints.Contains(joint))
	{
		_Joints.Remove(joint);
	}
}

void ODEPhysicsScene::Update(real64 elapsed)
{
	if (_WorldID == NULL)
		return;

	dWorldStep(_WorldID, elapsed);
}

}
