/*=============================================================================
Particle.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_PARTICLE_H_
#define _SE_PARTICLE_H_

#include "Core/Core.h"
#include "Core/Range.h"
#include "Core/Scale.h"

namespace SonataEngine
{

/**
 * Base class for the particles.
 * Particles are a point in space but their representation can be different.
 * Particles are emitted by an emitter and owned by a particle system.
 * Different affectors can modify an instance of a particule.
 */
class Particle
{
public:
	Particle();
	~Particle();

	real32 GetAge() const { return _Age; }

	real32 GetTimeToLive() const { return _TimeToLive; }

	real32 GetLifetime() const { return _Age + _TimeToLive; }

	bool IsAlive() const { return _TimeToLive > 0; }

	const Vector3& GetPosition() const { return _Position; }
	void SetPosition(const Vector3& position) { _Position = position; }

	const Vector3& GetVelocity() const { return _Velocity; }
	void SetVelocity(const Vector3& velocity) { _Velocity = velocity; }

	const Vector3& GetRotation() const { return _Rotation; }
	void SetRotation(const Vector3& rotation) { _Rotation = rotation; }

	const Vector3& GetSize() const { return _Size; }
	void SetSize(const Vector3& size) { _Size = size; }

	const Color32& GetColor() const { return _Color; }
	void SetColor(const Color32& color) { _Color = color; }

	real32 GetAlpha() const { return _Alpha; }
	void SetAlpha(real32 alpha) { _Alpha = alpha; }

protected:
	friend class ParticleEmitter;
	void SetAge(real32 age) { _Age = age; }
	void SetTimeToLive(real32 timeToLive) { _TimeToLive = timeToLive; }
	void Kill();

	real32 _Age;
	real32 _TimeToLive;

	Vector3 _Position;
	Vector3 _Velocity;
	Vector3 _Rotation;
	Vector3 _Size;
	Color32 _Color;
	real32 _Alpha;
};

}

#endif 
