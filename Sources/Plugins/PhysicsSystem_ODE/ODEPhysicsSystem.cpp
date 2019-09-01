/*=============================================================================
ODEPhysicsSystem.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ODEPhysicsSystem.h"
#include "ODEPhysicsScene.h"

namespace SE_ODE
{

ODEPhysicsSystem::ODEPhysicsSystem() :
	PhysicsSystem()
{
}

ODEPhysicsSystem::~ODEPhysicsSystem()
{
	Destroy();
}

PhysicsSystemDescription ODEPhysicsSystem::GetDescription() const
{
	return _Description;
}

bool ODEPhysicsSystem::Create(const PhysicsSystemDescription& desc)
{
	_Description = desc;

	return true;
}

void ODEPhysicsSystem::Destroy()
{
	dCloseODE();
}

void ODEPhysicsSystem::Update(real64 elapsed)
{
	Array<ScenePtr>::Iterator it = _Scenes.GetIterator();
	while (it.Next())
	{
		it.Current()->Update(elapsed);
	}
}

IScene* ODEPhysicsSystem::CreateScene(const SceneDescription& desc)
{
	IScene* scene = new ODEPhysicsScene(this, desc);
	_Scenes.Add(scene);
	return scene;
}

void ODEPhysicsSystem::DestroyScene(IScene* scene)
{
	if (_Scenes.Contains(scene))
	{
		_Scenes.Remove(scene);
	}
}

}
