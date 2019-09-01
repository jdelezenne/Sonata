/*=============================================================================
NullPhysicsSystem.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "NullPhysicsSystem.h"
#include "NullPhysicsScene.h"

namespace SE_Null
{

NullPhysicsSystem::NullPhysicsSystem() :
	PhysicsSystem()
{
}

NullPhysicsSystem::~NullPhysicsSystem()
{
	Destroy();
}

PhysicsSystemDescription NullPhysicsSystem::GetDescription() const
{
	return PhysicsSystemDescription();
}

bool NullPhysicsSystem::Create(const PhysicsSystemDescription& desc)
{
	return true;
}

void NullPhysicsSystem::Destroy()
{
}

void NullPhysicsSystem::Update(real64 elapsed)
{
	//PreStep.Invoke(this, UpdateEventArgs(elapsed));

	Array<ScenePtr>::Iterator it = _Scenes.GetIterator();
	while (it.Next())
	{
		it.Current()->Update(elapsed);
	}

	//PostStep.Invoke(this, UpdateEventArgs(elapsed));
}

IScene* NullPhysicsSystem::CreateScene(const SceneDescription& desc)
{
	IScene* scene = new NullPhysicsScene();
	_Scenes.Add(scene);
	return scene;
}

void NullPhysicsSystem::DestroyScene(IScene* scene)
{
	if (_Scenes.Contains(scene))
	{
		_Scenes.Remove(scene);
	}
}

}
