/*=============================================================================
NullPhysicsMaterial.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_NULLPHYSICSMATERIAL_H_
#define _SE_NULLPHYSICSMATERIAL_H_

#include "NullPhysicsSystem.h"
#include "NullPhysicsScene.h"

namespace SE_Null
{

/** Null Material. */
class NullPhysicsMaterial : public Physics::IMaterial
{
public:
	NullPhysicsMaterial();
	virtual ~NullPhysicsMaterial();

	virtual void Apply();
};

}

#endif 
