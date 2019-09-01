/*=============================================================================
SpriteParticle.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SPRITEPARTICLE_H_
#define _SE_SPRITEPARTICLE_H_

#include "Graphics/Particle/ParticleTemplate.h"
#include "Graphics/Model/Mesh.h"

namespace SonataEngine
{

/**
 * Sprite particle.
 * This particle is represented by a colored point.
 */
class SpriteParticle : public ParticleTemplate
{
public:
	SpriteParticle();
	virtual ~SpriteParticle();

	virtual void SetMaxParticles(uint32 maxParticles);

	virtual void RenderParticles(ParticleEmitter* emitter, const ParticleEmitter::ParticleArray& particles, ShaderMaterial* shader);

protected:
	VertexLayoutPtr _vertexLayout;
	MeshPtr _mesh;
};

}

#endif 
