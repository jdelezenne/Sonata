/*=============================================================================
RWPhysicsMaterial.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_RWPHYSICSMATERIAL_H_
#define _SE_RWPHYSICSMATERIAL_H_

#include "RWPhysicsSystem.h"
#include "RWPhysicsScene.h"

namespace SE_RW
{

/** RW Material. */
class RWPhysicsMaterial : public Physics::IMaterial
{
public:
	RWPhysicsMaterial(RWPhysicsScene* scene);
	virtual ~RWPhysicsMaterial();

	virtual void Apply();

	RwpBody* GetRwpBody() const { return _RwpBody; }

protected:
	RWPhysicsScene* _Scene;
	RwpBody* _RwpBody;
};

}

#endif 
