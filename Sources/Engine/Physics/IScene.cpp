/*=============================================================================
IScene.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "IScene.h"
#include "Physics/PhysicsSystem.h"

namespace SonataEngine
{
	namespace Physics
	{
		SceneDescription::SceneDescription() :
			_BroadPhase(true),
			_CollisionDetection(true),
			_Gravity(PhysicsSystem::DefaultGravity),
			_TimeStepType(TimeStepType_Fixed),
			_TimeStep(1.0f/60.0f),
			_Iterations(8),
			_Bounds(AABB::Empty),
			_MaxBodies(0),
			_MaxJoints(0)
		{
		}


		IScene::IScene() :
			RefObject()
		{
		}

		IScene::~IScene()
		{
		}
	}
}
