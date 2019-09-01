/*=============================================================================
PointParticle.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_POINTPARTICLE_H_
#define _SE_POINTPARTICLE_H_

#include "Graphics/Particle/ParticleTemplate.h"
#include "Graphics/Model/Mesh.h"

namespace SonataEngine
{

/**
 * Point particle.
 * This particle is represented by a colored point.
 */
class PointParticle : public ParticleTemplate
{
public:
	PointParticle();
	virtual ~PointParticle();

	virtual void SetMaxParticles(uint32 maxParticles);

	virtual void InitParticle(ParticleEmitter* emitter, Particle* particle);

	virtual void RenderParticles(ParticleEmitter* emitter, const ParticleEmitter::ParticleArray& particles, ShaderMaterial* shader);

protected:
	VertexLayoutPtr _vertexLayout;
	MeshPtr _mesh;
};

}

#endif 
