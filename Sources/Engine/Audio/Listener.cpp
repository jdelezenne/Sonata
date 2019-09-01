/*=============================================================================
Listener.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Listener.h"

namespace SonataEngine
{
	namespace Audio
	{
		Listener::Listener() :
			_Position(Vector3::Zero),
			_Velocity(Vector3::Zero),
			_OrientationFront(Vector3::UnitZ),
			_OrientationTop(Vector3::UnitY),
			_DopplerFactor(1.0f),
			_DistanceFactor(1.0f),
			_RolloffFactor(1.0f)
		{
		}

		Listener::~Listener()
		{
		}
	}
}
