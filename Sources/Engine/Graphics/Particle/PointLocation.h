/*=============================================================================
PointLocation.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_POINTLOCATION_H_
#define _SE_POINTLOCATION_H_

#include "Graphics/Particle/ParticleLocation.h"

namespace SonataEngine
{

/**
 * Point Location.
 */
class PointLocation : public ParticleLocation
{
public:
	PointLocation();
	virtual ~PointLocation();

	virtual void LocateParticle(Particle* particle);
};

}

#endif 
