/*=============================================================================
NXPhysicsMaterial.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_NXPHYSICSMATERIAL_H_
#define _SE_NXPHYSICSMATERIAL_H_

#include "NXPhysicsSystem.h"
#include "NXPhysicsScene.h"

namespace SE_NX
{

/** PhysX Material. */
class NXPhysicsMaterial : public Physics::IMaterial
{
public:
	NXPhysicsMaterial(NXPhysicsScene* scene);
	virtual ~NXPhysicsMaterial();

	virtual void Apply();

	NxMaterialIndex GetNxMaterialIndex() const { return _NxMaterialIndex; }

protected:
	NXPhysicsScene* _Scene;
	NxMaterialIndex _NxMaterialIndex;
};

}

#endif 
