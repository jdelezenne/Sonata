/*=============================================================================
SoftwarePhysicsSystem.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SOFTWAREPHYSICSSYSTEM_H_
#define _SE_SOFTWAREPHYSICSSYSTEM_H_

#include <Core/Core.h>
#include <Physics/Physics.h>

using namespace SonataEngine;
using namespace SonataEngine::Physics;

namespace SE_Software
{

#define Math_IsNaN(value) \
	(!(value == 0 || value > 0 || value < 0))

/** Software Physics System. */
class SoftwarePhysicsSystem : public PhysicsSystem
{
public:
	SoftwarePhysicsSystem();
	virtual ~SoftwarePhysicsSystem();

	virtual PhysicsSystemDescription GetDescription() const;

	virtual bool Create(const PhysicsSystemDescription& desc);

	virtual void Destroy();

	virtual void Update(real64 elapsed);

	virtual IScene* CreateScene(const SceneDescription& desc);
	virtual void DestroyScene(IScene* scene);

protected:
	PhysicsSystemDescription _Description;

	Array<ScenePtr> _Scenes;
};

}

#endif 
