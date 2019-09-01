/*=============================================================================
PointLocation.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "PointLocation.h"

namespace SonataEngine
{

PointLocation::PointLocation() :
	ParticleLocation()
{
}

PointLocation::~PointLocation()
{
}

void PointLocation::LocateParticle(Particle* particle)
{
	real32 position = Math::Random(_PositionRange.Min.X, _PositionRange.Max.X);
	particle->SetPosition(Vector3(position, position, position));
}

}
