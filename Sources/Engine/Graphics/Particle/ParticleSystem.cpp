/*=============================================================================
ParticleSystem.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ParticleSystem.h"

namespace SonataEngine
{

ParticleSystem::ParticleSystem() :
	SceneObject(),
	_Paused(false),
	_Speed(1.0f)
{
}

ParticleSystem::~ParticleSystem()
{
	ParticleEmitterList::Iterator it = _ParticleEmitters.GetIterator();
	while (it.Next())
	{
		ParticleEmitter* emitter = it.Current();
		emitter->SetParticleSystem(NULL);
		delete emitter;
	}
}

void ParticleSystem::Update(const TimeValue& timeValue)
{
	if (_Paused)
		return;

	static TimeValue lastTime = timeValue;
	real64 elapsed = (real64)(timeValue - lastTime);

	real32 delta = _Speed * elapsed;

	ParticleEmitterList::Iterator it = _ParticleEmitters.GetIterator();
	while (it.Next())
	{
		it.Current()->Update(timeValue * TimeValue(delta));
	}
}

void ParticleSystem::Render()
{
	ParticleEmitterList::Iterator it = _ParticleEmitters.GetIterator();
	while (it.Next())
	{
		it.Current()->Render();
	}
}

bool ParticleSystem::AddParticleEmitter(ParticleEmitter* value)
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return false;
	}
	else
	{
		value->SetParticleSystem(this);
		_ParticleEmitters.Add(value);
		return true;
	}
}

bool ParticleSystem::InsertParticleEmitter(int index, ParticleEmitter* value)
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return false;
	}
	else
	{
		value->SetParticleSystem(this);
		_ParticleEmitters.Insert(index, value);
		return true;
	}
}

bool ParticleSystem::ReplaceParticleEmitter(ParticleEmitter* previous, ParticleEmitter* emitter)
{
	if (previous == NULL || emitter == NULL)
	{
		SEthrow(ArgumentNullException());
		return false;
	}
	else
	{
		int index = _ParticleEmitters.IndexOf(previous);
		if (index == -1)
		{
			throw "ArgumentException";
			return false;
		}
		else
		{
			emitter->SetParticleSystem(this);
			_ParticleEmitters.SetItem(index, emitter);
			return true;
		}
	}
}

bool ParticleSystem::RemoveParticleEmitter(ParticleEmitter* emitter)
{
	if (emitter == NULL)
	{
		SEthrow(ArgumentNullException());
		return false;
	}
	else
	{
		emitter->SetParticleSystem(NULL);
		_ParticleEmitters.Remove(emitter);
		return true;
	}
}

void ParticleSystem::RemoveAllParticleEmitteres()
{
	_ParticleEmitters.Clear();
}

int ParticleSystem::GetParticleEmitterCount() const
{
	return _ParticleEmitters.Count();
}

int ParticleSystem::GetParticleEmitterIndex(ParticleEmitter* emitter) const
{
	if (emitter == NULL)
	{
		SEthrow(ArgumentNullException());
		return -1;
	}
	else
	{
		return _ParticleEmitters.IndexOf(emitter);
	}
}

ParticleEmitter* ParticleSystem::GetParticleEmitter(int index) const
{
	return _ParticleEmitters[index];
}

bool ParticleSystem::SetParticleEmitter(int index, ParticleEmitter* emitter)
{
	if (emitter == NULL)
	{
		SEthrow(ArgumentNullException());
		return false;
	}
	else
	{
		_ParticleEmitters.SetItem(index, emitter);
		return true;
	}
}

void ParticleSystem::Rewind()
{
	ParticleEmitterList::Iterator it = _ParticleEmitters.GetIterator();
	while (it.Next())
	{
		ParticleEmitter* emitter = it.Current();
		emitter->SetMaxParticles(emitter->GetMaxParticles());
	}
}

}
