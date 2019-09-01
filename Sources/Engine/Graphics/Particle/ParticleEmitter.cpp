/*=============================================================================
ParticleEmitter.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ParticleEmitter.h"
#include "Graphics/Particle/ParticleSystem.h"
#include "Graphics/Particle/ParticleTemplate.h"
#include "Graphics/Materials/DefaultMaterial.h"

namespace SonataEngine
{

ParticleEmitter::ParticleEmitter() :
	NamedObject(),
	_ParticleSystem(NULL),
	_ParticleCount(0),
	_AliveParticleCount(0),
	_DeadParticleCount(ParticleSystem::MAXPARTICLES),
	_Age(0.0f),
	_TimeToLive(0.0f),
	_EmissionRemainder(0.0f),
	_ParticleTemplate(NULL),
	_Enabled(true),
	_Looped(true),
	_CoordinateSystem(ParticleCoordinateSystem_Independent),
	_StartVelocity(RangeVector3(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f))),
	_Acceleration(Vector3(0.0f, 0.0f, 0.0f)),
	_MaxVelocity(Vector3(10000.0f, 10000.0f, 10000.0f)),
	_UseVelocityScale(false),
	_VelocityScaleRepeats(0.0f),
	_Location(NULL),
	_PositionOffset(Vector3(0.0f, 0.0f, 0.0f)),
	_Lifetime(RangeReal32(0.0f, 0.0f)),
	_EmissionRate(RangeReal32(0.0f, 0.0f))
{
	_shader = new DefaultMaterial();
	SetMaxParticles(ParticleSystem::MAXPARTICLES);
}

ParticleEmitter::~ParticleEmitter()
{
	SE_DELETE(_shader);
	SE_DELETE(_ParticleTemplate);
	SE_DELETE(_Location);

	ParticleArray::Iterator it = _Particles.GetIterator();
	while (it.Next())
	{
		delete it.Current();
	}
}

void ParticleEmitter::SetParticleTemplate(ParticleTemplate* particleTemplate)
{
	_ParticleTemplate = particleTemplate;

	if (_ParticleTemplate != NULL)
		_ParticleTemplate->SetMaxParticles(_MaxParticles);
}

void ParticleEmitter::SetMaxParticles(uint32 maxParticles)
{
	ParticleArray::Iterator it = _Particles.GetIterator();
	while (it.Next())
	{
		delete it.Current();
	}

	_MaxParticles = maxParticles;

	_ParticleCount = 0;
	_AliveParticleCount = 0;
	_DeadParticleCount = _MaxParticles;

	// Increase size
	_Particles.Resize(_MaxParticles);

	// Create new particles
	uint count = _Particles.Count();
	for (uint i = 0; i < count; i++)
	{
		Particle* particle = new Particle();
		_Particles.SetItem(i, particle);
	}

	if (_ParticleTemplate != NULL)
		_ParticleTemplate->SetMaxParticles(maxParticles);
}

void ParticleEmitter::Initialize()
{
	_Age = 0.0f;
	_TimeToLive = Math::Random(_Lifetime.Min, _Lifetime.Max);
}

void ParticleEmitter::CreateParticle()
{
	ParticleArray::Iterator it = _Particles.GetIterator();
	while (it.Next())
	{
		Particle* particle = it.Current();
		if (!particle->IsAlive())
		{
			InitParticle(particle);

			_AliveParticleCount++;
			_DeadParticleCount--;
			break;
		}
	}
}

void ParticleEmitter::DestroyParticle(Particle* particle)
{
	ParticleArray::Iterator it = _Particles.GetIterator();
	while (it.Next())
	{
		if (particle == it.Current())
		{
			particle->Kill();
			_AliveParticleCount--;
			_DeadParticleCount++;
			break;
		}
	}
}

void ParticleEmitter::InitParticle(Particle* particle)
{
	particle->SetAge(0.0f);
	particle->SetTimeToLive(Math::Random(_Lifetime.Min, _Lifetime.Max));

	particle->SetVelocity(Math::Random(_StartVelocity.Min, _StartVelocity.Max));

	if (_Location)
		_Location->LocateParticle(particle);

	if (_CoordinateSystem == ParticleCoordinateSystem_Independent)
		particle->SetPosition(particle->GetPosition() + _ParticleSystem->GetWorldPosition() + _PositionOffset);

	_ParticleTemplate->InitParticle(this, particle);
}

void ParticleEmitter::Update(const TimeValue& timeValue)
{
	if (!_Enabled)
		return;

	if (_Age >= _TimeToLive)
	{
		Initialize();
	}

	static TimeValue lastTime = timeValue;
	real64 elapsed = (real64)(timeValue - lastTime);

	if (_Looped || _ParticleCount < _MaxParticles)
	{
		_EmissionRemainder += Math::Random(_EmissionRate.Min, _EmissionRate.Max) * elapsed;
		uint16 iEmissionCount = (uint16) _EmissionRemainder;
		_EmissionRemainder -= iEmissionCount;

		while (iEmissionCount > 0 && _AliveParticleCount < _MaxParticles)
		{
			CreateParticle();
			iEmissionCount--;
			_ParticleCount++;
		}
	}

	ParticleArray::Iterator it = _Particles.GetIterator();
	while (it.Next())
	{
		Particle* particle = it.Current();
		if (!particle->IsAlive())
			continue;

		particle->SetAge(particle->GetAge() + elapsed);
		particle->SetTimeToLive(particle->GetTimeToLive() - elapsed);

		if (!_UseVelocityScale)
		{
			particle->SetVelocity(Math::Min(particle->GetVelocity() + _Acceleration, _MaxVelocity));
		}
		else
		{
			real32 step = particle->GetAge() / particle->GetLifetime();
			//particle->SetVelocity(InterpolateScale(step / (_VelocityScaleRepeats+1.0f) - (int)(step * (_VelocityScaleRepeats+1.0f)), _VelocityScale));
		}

		particle->SetPosition(particle->GetPosition() + particle->GetVelocity() * elapsed);

		_ParticleTemplate->UpdateParticle(particle, elapsed);

		if (!particle->IsAlive())
		{
			particle->Kill();
			_AliveParticleCount--;
			_DeadParticleCount++;
		}
	}

	//_Age += elapsed;
}

void ParticleEmitter::Render()
{
	/*if (_CoordinateSystem == ParticleCoordinateSystem_Relative)
		glMultMatrixf(_ParticleSystem->GetWorldTransform());*/

	FFPPass* pass = (FFPPass*)(((DefaultMaterial*)_shader)->GetTechnique())->GetPassByIndex(0);
	pass->RasterizerState.CullMode = CullMode_None;
	pass->DepthState.WriteEnable = false;

	if (_AliveParticleCount > 0)
	{
		_ParticleTemplate->RenderParticles(this, _Particles, _shader);
	}
}

}
