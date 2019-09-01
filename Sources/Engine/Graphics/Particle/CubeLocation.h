/*=============================================================================
CubeLocation.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_CUBELOCATION_H_
#define _SE_CUBELOCATION_H_

#include "Graphics/Particle/ParticleLocation.h"

namespace SonataEngine
{

/**
 * Cube Location.
 */
class CubeLocation : public ParticleLocation
{
public:
	CubeLocation();
	virtual ~CubeLocation();

	virtual void LocateParticle(Particle* particle);
};

}

#endif 
