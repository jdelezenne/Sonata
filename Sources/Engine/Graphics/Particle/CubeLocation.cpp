/*=============================================================================
CubeLocation.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "CubeLocation.h"
#include "Core/Math/Math.h"

namespace SonataEngine
{

CubeLocation::CubeLocation() :
	ParticleLocation()
{
}

CubeLocation::~CubeLocation()
{
}

void CubeLocation::LocateParticle(Particle* particle)
{
	particle->SetPosition(Math::Random(_PositionRange.Min, _PositionRange.Max));
}

}
