/*=============================================================================
ODEPhysicsMaterial.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ODEPhysicsMaterial.h"

namespace SE_ODE
{

ODEPhysicsMaterial::ODEPhysicsMaterial(ODEPhysicsScene* scene) :
	Physics::IMaterial()
{
	_Scene = scene;
}

ODEPhysicsMaterial::~ODEPhysicsMaterial()
{
}

void ODEPhysicsMaterial::Apply()
{
}

}
