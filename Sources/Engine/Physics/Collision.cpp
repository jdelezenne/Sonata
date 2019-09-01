/*=============================================================================
Collision.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Collision.h"

namespace SonataEngine
{
	namespace Physics
	{
		ContactPair::ContactPair() :
			Object(),
			_BodyA(NULL),
			_BodyB(NULL),
			_Time(0.0),
			_Forces(Vector3::Zero),
			_Torques(Vector3::Zero)
		{
		}

		ContactInfo::ContactInfo() :
			Object(),
			_ShapeA(NULL),
			_ShapeB(NULL),
			_Restitution(0.0),
			_Separation(0.0),
			_MaterialA(0),
			_MaterialB(0),
			_Position(Vector3::Zero),
			_Normal(Vector3::Zero)
		{
		}
	}
}
