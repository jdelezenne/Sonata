/*=============================================================================
PhysicsMaterial.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "PhysicsMaterial.h"

namespace SE_Software
{

PhysicsMaterial::PhysicsMaterial(PhysicsScene* scene) :
	Physics::IMaterial()
{
	_Scene = scene;
}

PhysicsMaterial::~PhysicsMaterial()
{
}

void PhysicsMaterial::Apply()
{
}

}
