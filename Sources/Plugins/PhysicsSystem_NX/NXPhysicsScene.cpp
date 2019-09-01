/*=============================================================================
NXPhysicsScene.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "NXPhysicsScene.h"
#include "NXPhysicsBody.h"
#include "NXPhysicsMaterial.h"

namespace SE_NX
{

class ContactReport : public NxUserContactReport
{
public:
	NXPhysicsScene* Scene;

    virtual void onContactNotify(NxContactPair& pair, NxU32 events)
	{
		// Iterate through contact points
		NxContactStreamIterator i(pair.stream);
		//user can call getNumPairs() here
		while (i.goNextPair())
		{
			//user can also call getShape() and getNumPatches() here
			while (i.goNextPatch())
			{
				while (i.goNextPoint())
				{
					ContactInfo info;
					info.SetPosition(NXHelper::MakeVector3(i.getPoint()));
					info.SetNormal(NXHelper::MakeVector3(i.getPatchNormal()));
					info.SetSeparation(i.getSeparation());

					Scene->PreContact.Invoke(Scene, ContactEventArgs(info));
				}
			}
		}
	}
};

NXPhysicsScene::NXPhysicsScene(NXPhysicsSystem* physicsSystem, const SceneDescription& desc) :
	Physics::IScene(),
	_PhysicsSystem(physicsSystem),
	_Description(desc)
{
	ContactReport* _ContactReport = new ContactReport();
	_ContactReport->Scene = this;

	NxSceneDesc sceneDesc;
	sceneDesc.gravity = NXHelper::MakeVector3(PhysicsSystem::DefaultGravity);
	sceneDesc.broadPhase = NX_BROADPHASE_COHERENT;
	sceneDesc.collisionDetection = desc._CollisionDetection;
	sceneDesc.userTriggerReport = NULL;
	sceneDesc.userNotify = NULL;
	sceneDesc.groundPlane = false;
	sceneDesc.userContactReport = _ContactReport;
	sceneDesc.timeStepMethod = NX_TIMESTEP_FIXED;

	_NxScene = _PhysicsSystem->GetNxSDK()->createScene(sceneDesc);
	if (_NxScene == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("NXPhysicsScene"),
			_T("Failed to create the scene."));
	}

	_DefaultMaterial = (new NXPhysicsMaterial(this))->GetIndex();
	NxMaterial* defaultMaterial = _NxScene->getMaterialFromIndex(0); 
	defaultMaterial->setRestitution(0.5);
	defaultMaterial->setStaticFriction(0.5);
	defaultMaterial->setDynamicFriction(0.5);

	_NxScene->setActorGroupPairFlags(0, 0,  NX_NOTIFY_ON_START_TOUCH);
}

NXPhysicsScene::~NXPhysicsScene()
{
	if (_NxScene != NULL)
	{
		_PhysicsSystem->GetNxSDK()->releaseScene(*_NxScene);
		_NxScene = NULL;
	}
}

SceneDescription NXPhysicsScene::GetDescription() const
{
	return _Description;
}

Vector3 NXPhysicsScene::GetGravity() const
{
	if (_NxScene == NULL)
		return Vector3::Zero;

	NxVec3 value;
	_NxScene->getGravity(value);
	return NXHelper::MakeVector3(value);
}

void NXPhysicsScene::SetGravity(const Vector3& value)
{
	if (_NxScene == NULL)
		return;

	_NxScene->setGravity(NXHelper::MakeVector3(value));
}

TimeStepType NXPhysicsScene::GetTimeStepType() const
{
	return _Description._TimeStepType;
}

void NXPhysicsScene::SetTimeStepType(TimeStepType value)
{
	_Description._TimeStepType = value;
}

real NXPhysicsScene::GetTimeStep() const
{
	return _Description._TimeStep;
}

void NXPhysicsScene::SetTimeStep(real value)
{
	_Description._TimeStep = value;
}

uint32 NXPhysicsScene::GetIterations() const
{
	return _Description._Iterations;
}

void NXPhysicsScene::SetIterations(uint32 value)
{
	_Description._Iterations = value;
}

IBody* NXPhysicsScene::CreateBody(const BodyDescription& desc)
{
	IBody* body = new NXPhysicsBody(this, desc);
	_Bodies.Add(body);
	return body;
}

void NXPhysicsScene::DestroyBody(IBody* body)
{
	if (_Bodies.Contains(body))
	{
		_Bodies.Remove(body);
	}
}

int NXPhysicsScene::GetBodyCount() const
{
	return _Bodies.Count();
}

IBody* NXPhysicsScene::GetBody(int index) const
{
	if (index < 0 || index >= _Bodies.Count())
		return NULL;

	return _Bodies[index];
}

IMaterial* NXPhysicsScene::CreateMaterial()
{
	IMaterial* material = new NXPhysicsMaterial(this);
	return material;
}

void NXPhysicsScene::DestroyMaterial(MaterialIndex material)
{
	_Materials.Remove(material);
}

IMaterial* NXPhysicsScene::GetMaterial(MaterialIndex index) const
{
	return _Materials[index];
}

MaterialIndex NXPhysicsScene::GetDefaultMaterial() const
{
	return _DefaultMaterial;
}

void NXPhysicsScene::SetDefaultMaterial(MaterialIndex value)
{
	_DefaultMaterial = value;
}

IJoint* NXPhysicsScene::CreateJoint()
{
	IJoint* joint = NULL;//new NXPhysicsJoint(this);
	return joint;
}

void NXPhysicsScene::DestroyJoint(IJoint* joint)
{
	if (_Joints.Contains(joint))
	{
		_Joints.Remove(joint);
	}
}

void NXPhysicsScene::Update(real64 elapsed)
{
	if (_NxScene == NULL)
		return;

	_NxScene->simulate(elapsed);
	_NxScene->flushStream();
	_NxScene->fetchResults(NX_RIGID_BODY_FINISHED, true);
}

}
