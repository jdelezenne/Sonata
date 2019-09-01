/*=============================================================================
Particle.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Particle.h"

namespace SonataEngine
{

Particle::Particle() :
	_Age(0.0f),
	_TimeToLive(0.0f),
	_Size(Vector3(1.0, 1.0, 1.0)),
	_Color(Color32(1.0, 1.0, 1.0)),
	_Alpha(1.0f)
{
}

Particle::~Particle()
{
}

void Particle::Kill()
{
	_Age = 0.0f;
	_TimeToLive = 0.0f;
}

}
