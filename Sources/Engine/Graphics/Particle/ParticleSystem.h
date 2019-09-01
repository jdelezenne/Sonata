/*=============================================================================
ParticleSystem.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_PARTICLESYSTEM_H_
#define _SE_PARTICLESYSTEM_H_

#include "Graphics/Scene/SceneObject.h"
#include "Graphics/Particle/ParticleEmitter.h"
#include "Graphics/Particle/Particle.h"
#include "Core/Math/AABB.h"

namespace SonataEngine
{

/**
 * Particle System.
 * A particle system represents a particle effect.
 * It's a spatial object that can emits particles from its emitters.
 * It manages the creation and destruction of the particules.
 */
class ParticleSystem : public SceneObject
{
public:
	static const int MAXPARTICLES = 10;

	/// Particle Emitter list.
	typedef Array<ParticleEmitter*> ParticleEmitterList;

	ParticleSystem();
	virtual ~ParticleSystem();


	/** Particle Emitters. */
	//@{
	bool AddParticleEmitter(ParticleEmitter* emitter);
	bool InsertParticleEmitter(int index, ParticleEmitter* emitter);
	bool ReplaceParticleEmitter(ParticleEmitter* previous, ParticleEmitter* emitter);
	bool RemoveParticleEmitter(ParticleEmitter* emitter);
	void RemoveAllParticleEmitteres();
	int GetParticleEmitterCount() const;
	int GetParticleEmitterIndex(ParticleEmitter* emitter) const;
	ParticleEmitter* GetParticleEmitter(int index) const;
	bool SetParticleEmitter(int index, ParticleEmitter* emitter);
	//@}

	bool GetPaused() const { return _Paused; }
	void SetPaused(bool paused) { _Paused = paused; }

	real32 GetSpeed() const { return _Speed; }
	void SetSpeed(real32 speed) { _Speed = speed; }

	virtual void Update(const TimeValue& timeValue);

	virtual void Render();

	void Rewind();

protected:
	ParticleEmitterList _ParticleEmitters;
	bool _Paused;
	real32 _Speed;
};

}

#endif 
