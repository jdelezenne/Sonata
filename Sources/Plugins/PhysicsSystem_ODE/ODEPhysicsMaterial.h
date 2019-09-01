/*=============================================================================
ODEPhysicsMaterial.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ODEPHYSICSMATERIAL_H_
#define _SE_ODEPHYSICSMATERIAL_H_

#include "ODEPhysicsSystem.h"
#include "ODEPhysicsScene.h"

namespace SE_ODE
{

/** ODE Material. */
class ODEPhysicsMaterial : public Physics::IMaterial
{
public:
	ODEPhysicsMaterial(ODEPhysicsScene* scene);
	virtual ~ODEPhysicsMaterial();

	virtual void Apply();

protected:
	ODEPhysicsScene* _Scene;
};

}

#endif 
