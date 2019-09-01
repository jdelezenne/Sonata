/*=============================================================================
IMaterial.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "IMaterial.h"

namespace SonataEngine
{
	namespace Physics
	{
		IMaterial::IMaterial() :
			RefObject(),
			_Index(0),
			_DynamicFriction(0.0f),
			_StaticFriction(0.0f),
			_Restitution(0.0f),
			_FrictionEnabled(true)
		{
		}

		IMaterial::~IMaterial()
		{
		}
	}
}
