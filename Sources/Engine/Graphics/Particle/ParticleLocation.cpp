/*=============================================================================
ParticleLocation.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ParticleLocation.h"

namespace SonataEngine
{

ParticleLocation::ParticleLocation() :
	RefObject(),
	_PositionRange(RangeVector3(Vector3::Zero, Vector3::Zero))
{
}


ParticleLocation::~ParticleLocation()
{
}

}
