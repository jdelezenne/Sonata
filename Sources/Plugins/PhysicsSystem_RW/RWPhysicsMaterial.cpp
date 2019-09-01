/*=============================================================================
RWPhysicsMaterial.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "RWPhysicsMaterial.h"

namespace SE_RW
{

RWPhysicsMaterial::RWPhysicsMaterial(RWPhysicsScene* scene) :
	Physics::IMaterial()
{
	_RwpBody = NULL;
	_Scene = scene;
}

RWPhysicsMaterial::~RWPhysicsMaterial()
{
}

void RWPhysicsMaterial::Apply()
{
}

}
