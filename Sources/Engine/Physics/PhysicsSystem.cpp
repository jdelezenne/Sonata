/*=============================================================================
PhysicsSystem.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "PhysicsSystem.h"

namespace SonataEngine
{
	namespace Physics
	{
		PhysicsSystemDescription::PhysicsSystemDescription()
		{
		}


		const Vector3 PhysicsSystem::DefaultGravity = Vector3(0.0, -9.8, 0.0);

		PhysicsSystem::PhysicsSystem() :
			Manager()
		{
		}

		PhysicsSystem::~PhysicsSystem()
		{
		}
	}
}
