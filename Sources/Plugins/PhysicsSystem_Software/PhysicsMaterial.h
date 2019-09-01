/*=============================================================================
PhysicsMaterial.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_PHYSICSMATERIAL_H_
#define _SE_PHYSICSMATERIAL_H_

#include "SoftwarePhysicsSystem.h"
#include "PhysicsScene.h"

namespace SE_Software
{

/** Material. */
class PhysicsMaterial : public Physics::IMaterial
{
public:
	PhysicsMaterial(PhysicsScene* scene);
	virtual ~PhysicsMaterial();

	virtual void Apply();

protected:
	PhysicsScene* _Scene;
};

}

#endif 
