/*=============================================================================
ParticleTemplate.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ParticleTemplate.h"

namespace SonataEngine
{

ParticleTemplate::ParticleTemplate() :
	RefObject(),
	_MaxParticles(0),
	_Size(RangeVector3(Vector3(1.0, 1.0, 1.0), Vector3(1.0, 1.0, 1.0))),
	_ColorScaleRepeats(0.0f)
{
}

ParticleTemplate::~ParticleTemplate()
{
}

void ParticleTemplate::SetMaxParticles(uint32 maxParticles)
{
	_MaxParticles = maxParticles;
}

void ParticleTemplate::InitParticle(ParticleEmitter* emitter, Particle* particle)
{
	particle->SetRotation(Math::Random(_Rotation.Min, _Rotation.Max));
	particle->SetSize(Math::Random(_Size.Min, _Size.Max));
}

void ParticleTemplate::UpdateParticle(Particle* particle, real64 elapsed)
{
	real32 step = Math::Clamp(particle->GetAge() / particle->GetLifetime(), 0.0f, 1.0f);

	//particle->SetRotation(InterpolateRange(step, _Rotation));
	//particle->SetSize(InterpolateRange(step, _Size));
	//particle->SetColor(InterpolateScale(
	//	step * (_ColorScaleRepeats+1.0f) - (int)(step * (_ColorScaleRepeats+1.0f)), _Color));
	//particle->SetAlpha(InterpolateScale(step, _Alpha));
}

}
