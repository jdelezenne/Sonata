/*=============================================================================
NullPhysicsScene.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "NullPhysicsScene.h"
#include "NullPhysicsBody.h"
#include "NullPhysicsMaterial.h"

namespace SE_Null
{

NullPhysicsScene::NullPhysicsScene() :
	Physics::IScene()
{
	IMaterial* material = CreateMaterial();
	_DefaultMaterial = material->GetIndex();
}

NullPhysicsScene::~NullPhysicsScene()
{
}

SceneDescription NullPhysicsScene::GetDescription() const
{
	return SceneDescription();
}

Vector3 NullPhysicsScene::GetGravity() const
{
	return Vector3::Zero;
}

void NullPhysicsScene::SetGravity(const Vector3& value)
{
}

TimeStepType NullPhysicsScene::GetTimeStepType() const
{
	return TimeStepType_Fixed;
}

void NullPhysicsScene::SetTimeStepType(TimeStepType value)
{
}

real NullPhysicsScene::GetTimeStep() const
{
	return 0.0;
}

void NullPhysicsScene::SetTimeStep(real value)
{
}

uint32 NullPhysicsScene::GetIterations() const
{
	return 0;
}

void NullPhysicsScene::SetIterations(uint32 value)
{
}

IBody* NullPhysicsScene::CreateBody(const BodyDescription& desc)
{
	IBody* body = new NullPhysicsBody();
	_Bodies.Add(body);
	return body;
}

void NullPhysicsScene::DestroyBody(IBody* body)
{
	if (_Bodies.Contains(body))
	{
		_Bodies.Remove(body);
	}
}

int NullPhysicsScene::GetBodyCount() const
{
	return _Bodies.Count();
}

IBody* NullPhysicsScene::GetBody(int index) const
{
	if (index < 0 || index >= _Bodies.Count())
		return NULL;

	return _Bodies[index];
}

IMaterial* NullPhysicsScene::CreateMaterial()
{
	IMaterial* material = new NullPhysicsMaterial();
	int count = _Materials.Keys().Count();
	material->SetIndex(count);
	_Materials[count] = material;
	return material;
}

void NullPhysicsScene::DestroyMaterial(MaterialIndex material)
{
	_Materials.Remove(material);
}

IMaterial* NullPhysicsScene::GetMaterial(MaterialIndex index) const
{
	return _Materials[index];
}

MaterialIndex NullPhysicsScene::GetDefaultMaterial() const
{
	return _DefaultMaterial;
}

void NullPhysicsScene::SetDefaultMaterial(MaterialIndex value)
{
	_DefaultMaterial = value;
}

IJoint* NullPhysicsScene::CreateJoint()
{
	IJoint* joint = NULL;//new NullPhysicsJoint(this);
	return joint;
}

void NullPhysicsScene::DestroyJoint(IJoint* joint)
{
	if (_Joints.Contains(joint))
	{
		_Joints.Remove(joint);
	}
}

void NullPhysicsScene::Update(real64 elapsed)
{
	Array<BodyPtr>::Iterator it = _Bodies.GetIterator();
	while (it.Next())
	{
		IBody* body = it.Current().Get();
		body->Update(elapsed);
	}
}

}
