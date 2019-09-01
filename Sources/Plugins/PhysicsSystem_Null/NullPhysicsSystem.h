/*=============================================================================
NullPhysicsSystem.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_NULLPHYSICSSYSTEM_H_
#define _SE_NULLPHYSICSSYSTEM_H_

#include <Core/Core.h>
#include <Physics/Physics.h>

using namespace SonataEngine;
using namespace SonataEngine::Physics;

namespace SE_Null
{

/** Null Physics System. */
class NullPhysicsSystem : public PhysicsSystem
{
public:
	NullPhysicsSystem();
	virtual ~NullPhysicsSystem();

	virtual PhysicsSystemDescription GetDescription() const;

	virtual bool Create(const PhysicsSystemDescription& desc);

	virtual void Destroy();

	virtual void Update(real64 elapsed);

	virtual IScene* CreateScene(const SceneDescription& desc);
	virtual void DestroyScene(IScene* scene);

protected:
	Array<ScenePtr> _Scenes;
};

}

#endif 
