/*=============================================================================
RWPhysicsScene.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "RWPhysicsScene.h"
#include "RWPhysicsBody.h"
#include "RWPhysicsMaterial.h"

namespace SE_RW
{

RWPhysicsScene::RWPhysicsScene(RWPhysicsSystem* physicsSystem, const SceneDescription& desc) :
	Physics::IScene(),
	_PhysicsSystem(physicsSystem)
{
	RwpBBox box = { {1000.0f, 1000.0f, 1000.0f}, 0, {-1000.0f, -1000.0f, -1000.0f} };
	RwpSizes sizes;

	RwpSizesInit(&sizes);

	RwpSizesAddBodySpace(&sizes, 200, 100);
	RwpSizesAddRagdollSpace(&sizes, 100, 50);
	RwpSizesSetExtJointSpace(&sizes, 20);

	_RwpContext = RwpContextCreate(&sizes, 1.0f, &box);

	SetGravity(PhysicsSystem::DefaultGravity);

	_DefaultMaterial = (new RWPhysicsMaterial(this))->GetIndex();
}

RWPhysicsScene::~RWPhysicsScene()
{
	if (_RwpContext != NULL)
	{
		RwpContextDestroy(_RwpContext);
		_RwpContext = NULL;
	}
}

SceneDescription RWPhysicsScene::GetDescription() const
{
	return _Description;
}

Vector3 RWPhysicsScene::GetGravity() const
{
	if (_RwpContext == NULL)
		return Vector3::Zero;

	return _Description._Gravity;
}

void RWPhysicsScene::SetGravity(const Vector3& value)
{
	if (_RwpContext == NULL)
		return;

	_Description._Gravity = value;

	RwV3d gravity;
	RWHelper::MakeVector3(&gravity, value);
	RwpPipelineSetGravity(RwpContextGetPipeline(_RwpContext), &gravity);
}

TimeStepType RWPhysicsScene::GetTimeStepType() const
{
	return _Description._TimeStepType;
}

void RWPhysicsScene::SetTimeStepType(TimeStepType value)
{
	_Description._TimeStepType = value;
}

real RWPhysicsScene::GetTimeStep() const
{
	return _Description._TimeStep;
}

void RWPhysicsScene::SetTimeStep(real value)
{
	_Description._TimeStep = value;
}

uint32 RWPhysicsScene::GetIterations() const
{
	return _Description._Iterations;
}

void RWPhysicsScene::SetIterations(uint32 value)
{
	_Description._Iterations = value;
}

IBody* RWPhysicsScene::CreateBody(const BodyDescription& desc)
{
	IBody* body = new RWPhysicsBody(this, desc);
	_Bodies.Add(body);
	return body;
}

void RWPhysicsScene::DestroyBody(IBody* body)
{
	if (_Bodies.Contains(body))
	{
		_Bodies.Remove(body);
	}
}

int RWPhysicsScene::GetBodyCount() const
{
	return _Bodies.Count();
}

IBody* RWPhysicsScene::GetBody(int index) const
{
	if (index < 0 || index >= _Bodies.Count())
		return NULL;

	return _Bodies[index];
}

IMaterial* RWPhysicsScene::CreateMaterial()
{
	IMaterial* material = new RWPhysicsMaterial(this);
	return material;
}

void RWPhysicsScene::DestroyMaterial(MaterialIndex material)
{
	_Materials.Remove(material);
}

IMaterial* RWPhysicsScene::GetMaterial(MaterialIndex index) const
{
	return _Materials[index];
}

MaterialIndex RWPhysicsScene::GetDefaultMaterial() const
{
	return _DefaultMaterial;
}

void RWPhysicsScene::SetDefaultMaterial(MaterialIndex value)
{
	_DefaultMaterial = value;
}

IJoint* RWPhysicsScene::CreateJoint()
{
	IJoint* joint = NULL;//new RWPhysicsJoint(this);
	return joint;
}

void RWPhysicsScene::DestroyJoint(IJoint* joint)
{
	if (_Joints.Contains(joint))
	{
		_Joints.Remove(joint);
	}
}

void RWPhysicsScene::Update(real64 elapsed)
{
	if (_RwpContext == NULL)
		return;

	RwpPipeline* p;

#ifdef PS2
	/* We must ensure that the scratchpad is free for use during the 
		* pipeline step. RWDMA_SPR_FREE() will free it from any use by 
		* RenderWare Graphics.
		*/
	RWDMA_SPR_FREE();
#endif

	RwReal timeStep = 1.0f / 60.0f;
	p = RwpPipelineStep(RwpContextGetPipeline(_RwpContext), timeStep);

	if (p == NULL)
		return;
}

}
