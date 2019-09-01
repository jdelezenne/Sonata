/*=============================================================================
ParticleManager.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_PARTICLEMANAGER_H_
#define _SE_PARTICLEMANAGER_H_

#include "Core/Singleton.h"
#include "Graphics/Particle/ParticleSystem.h"

namespace SonataEngine
{

/**
 * Particle System Manager.
 * Manages particle systems, emitter and controller factories.
 */
class ParticleManager : public Singleton<ParticleManager>
{
public:
	/// Particle System list.
	typedef Array<ParticleSystem*> ParticleSystemList;

	/** @name Constructors / Destructor. */
	//@{
	ParticleManager();
	virtual ~ParticleManager();
	//@}

	/** Particle Systems. */
	//@{
	bool AddParticleSystem(ParticleSystem* ps);
	bool InsertParticleSystem(int index, ParticleSystem* ps);
	bool ReplaceParticleSystem(ParticleSystem* previous, ParticleSystem* ps);
	bool RemoveParticleSystem(ParticleSystem* ps);
	void RemoveAllParticleSystems();
	int GetParticleSystemCount() const;
	int GetParticleSystemIndex(ParticleSystem* ps) const;
	ParticleSystem* GetParticleSystem(int index) const;
	bool SetParticleSystem(int index, ParticleSystem* ps);
	//@}

protected:
	ParticleSystemList _ParticleSystems;
};

}

#endif 
