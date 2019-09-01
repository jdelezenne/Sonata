/*=============================================================================
NullPhysicsMaterial.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "NullPhysicsMaterial.h"

namespace SE_Null
{

NullPhysicsMaterial::NullPhysicsMaterial() :
	Physics::IMaterial()
{
}

NullPhysicsMaterial::~NullPhysicsMaterial()
{
}

void NullPhysicsMaterial::Apply()
{
}

}
