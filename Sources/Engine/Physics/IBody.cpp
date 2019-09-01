/*=============================================================================
IBody.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "IBody.h"
#include "Physics/IShape.h"

namespace SonataEngine
{
	namespace Physics
	{
		real BodyDescription::GetDensity() const
		{
			if (_MassDensity < 0.0)
				return -_MassDensity;
			else
				return 0.0;
		}

		void BodyDescription::SetDensity(real value)
		{
			_MassDensity = -value;
		}

		real BodyDescription::GetMass() const
		{
			if (_MassDensity > 0.0)
				return _MassDensity;
			else
				return 0.0;
		}
		
		void BodyDescription::SetMass(real value)
		{
			_MassDensity = value;
		}

		IBody::IBody() :
			RefObject()
		{
		}

		IBody::~IBody()
		{
		}
	}
}
