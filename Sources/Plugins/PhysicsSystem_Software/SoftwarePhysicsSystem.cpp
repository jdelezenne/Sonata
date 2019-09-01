/*=============================================================================
SoftwarePhysicsSystem.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "SoftwarePhysicsSystem.h"
#include "PhysicsScene.h"

namespace SE_Software
{

SoftwarePhysicsSystem::SoftwarePhysicsSystem() :
	PhysicsSystem()
{
}

SoftwarePhysicsSystem::~SoftwarePhysicsSystem()
{
	Destroy();
}

PhysicsSystemDescription SoftwarePhysicsSystem::GetDescription() const
{
	return _Description;
}

bool SoftwarePhysicsSystem::Create(const PhysicsSystemDescription& desc)
{
	_Description = desc;

	return true;
}

void SoftwarePhysicsSystem::Destroy()
{
}

void SoftwarePhysicsSystem::Update(real64 elapsed)
{
	//PreStep.Invoke(this, UpdateEventArgs(elapsed));

	Array<ScenePtr>::Iterator it = _Scenes.GetIterator();
	while (it.Next())
	{
		it.Current()->Update(elapsed);
	}

	//PostStep.Invoke(this, UpdateEventArgs(elapsed));
}

IScene* SoftwarePhysicsSystem::CreateScene(const SceneDescription& desc)
{
	IScene* scene = new PhysicsScene(this, desc);
	_Scenes.Add(scene);
	return scene;
}

void SoftwarePhysicsSystem::DestroyScene(IScene* scene)
{
	if (_Scenes.Contains(scene))
	{
		_Scenes.Remove(scene);
	}
}

}
