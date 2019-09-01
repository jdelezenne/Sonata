/*=============================================================================
ParticleTemplate.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_PARTICLETEMPLATE_H_
#define _SE_PARTICLETEMPLATE_H_

#include "Core/Object.h"
#include "Core/Range.h"
#include "Core/Scale.h"
#include "Graphics/Particle/Particle.h"
#include "Graphics/Particle/ParticleEmitter.h"

namespace SonataEngine
{

/**
 * Particle Template.
 * Particle templates contains the settings of a type of particle.
 * The template is used by the emitters to spawn particles.
 * @remarks There are 3 kind of scales:
 *	- a constant value given by the emitter (from a range)
 *	- a scale of values
 *	- a scale of range of values
 */
class ParticleTemplate : public RefObject
{
protected:
	uint32 _MaxParticles;
	RangeVector3 _Size;
	RangeVector3 _Rotation;
	ScaleColor32 _Color;
	//ScaleReal32 _Alpha;
	Scale<real32> _Alpha;
	real32 _ColorScaleRepeats;

	real32 _FadeIn;
	real32 _FadeOut;
	RangeReal32 _FadeTime;

public:
	ParticleTemplate();
	virtual ~ParticleTemplate();

	const RangeVector3& GetSize() const { return _Size; }
	void SetSize(const RangeVector3& size) { _Size = size; }

	const RangeVector3& GetRotation() const { return _Rotation; }
	void SetRotation(const RangeVector3& rotation) { _Rotation = rotation; }

	const ScaleColor32& GetColor() const { return _Color; }
	void SetColor(const ScaleColor32& color) { _Color = color; }

	const ScaleReal32& GetAlpha() const { return _Alpha; }
	void SetAlpha(const ScaleReal32& alpha) { _Alpha = alpha; }

	real32 GetColorScaleRepeats() const { return _ColorScaleRepeats; }
	void SetColorScaleRepeats(real32 colorScaleRepeats) { _ColorScaleRepeats = Math::Max(0.0f, colorScaleRepeats); }

	real32 GetFadeIn() const { return _FadeIn; }
	void SetFadeIn(real32 fadeIn) { _FadeIn = fadeIn; }

	real32 GetFadeOut() const { return _FadeOut; }
	void SetFadeOut(real32 fadeOut) { _FadeOut = fadeOut; }

	const RangeReal32& GetFadeTime() const { return _FadeTime; }
	void SetFadeTime(const RangeReal32& fadeTime) { _FadeTime = fadeTime; }

	virtual void SetMaxParticles(uint32 maxParticles);

	virtual void InitParticle(ParticleEmitter* emitter, Particle* particle);

	virtual void UpdateParticle(Particle* particle, real64 elapsed);

	virtual void RenderParticles(ParticleEmitter* emitter, const ParticleEmitter::ParticleArray& particles, ShaderMaterial* shader) = 0;
};

}

#endif 
