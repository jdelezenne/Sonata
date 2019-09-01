/*=============================================================================
ParticleLocation.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_PARTICLELOCATION_H_
#define _SE_PARTICLELOCATION_H_

#include "Core/Object.h"
#include "Core/Range.h"
#include "Graphics/Particle/Particle.h"

namespace SonataEngine
{

/**
 * Base class for the particle locations.
 * A particle location determines the origin of the particles.
 */
class ParticleLocation : public RefObject
{
public:
	ParticleLocation();
	virtual ~ParticleLocation();

	const RangeVector3& GetPositionRange() const { return _PositionRange; }
	void SetPositionRange(const RangeVector3& positionRange) { _PositionRange = positionRange; }

	virtual void LocateParticle(Particle* particle) = 0;

protected:
	RangeVector3 _PositionRange;
};

}

#endif 
