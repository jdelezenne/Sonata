/*=============================================================================
ParticleManager.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ParticleManager.h"

namespace SonataEngine
{

ParticleManager::ParticleManager()
{
}

ParticleManager::~ParticleManager()
{
	ParticleSystemList::Iterator it = _ParticleSystems.GetIterator();
	while (it.Next())
	{
		delete it.Current();
	}
}

bool ParticleManager::AddParticleSystem(ParticleSystem* ps)
{
	if (ps == NULL)
	{
		SEthrow(ArgumentNullException());
		return false;
	}
	else
	{
		_ParticleSystems.Add(ps);
		return true;
	}
}

bool ParticleManager::InsertParticleSystem(int index, ParticleSystem* ps)
{
	if (ps == NULL)
	{
		SEthrow(ArgumentNullException());
		return false;
	}
	else
	{
		_ParticleSystems.Insert(index, ps);
		return true;
	}
}

bool ParticleManager::ReplaceParticleSystem(ParticleSystem* previous, ParticleSystem* ps)
{
	if (previous == NULL || ps == NULL)
	{
		SEthrow(ArgumentNullException());
		return false;
	}
	else
	{
		int index = _ParticleSystems.IndexOf(previous);
		if (index == -1)
		{
			throw "ArgumentException";
			return false;
		}
		else
		{
			_ParticleSystems.SetItem(index, ps);
			return true;
		}
	}
}

bool ParticleManager::RemoveParticleSystem(ParticleSystem* ps)
{
	if (ps == NULL)
	{
		SEthrow(ArgumentNullException());
		return false;
	}
	else
	{
		_ParticleSystems.Remove(ps);
		return true;
	}
}

void ParticleManager::RemoveAllParticleSystems()
{
	_ParticleSystems.Clear();
}

int ParticleManager::GetParticleSystemCount() const
{
	return _ParticleSystems.Count();
}

int ParticleManager::GetParticleSystemIndex(ParticleSystem* ps) const
{
	if (ps == NULL)
	{
		SEthrow(ArgumentNullException());
		return -1;
	}
	else
	{
		return _ParticleSystems.IndexOf(ps);
	}
}

ParticleSystem* ParticleManager::GetParticleSystem(int index) const
{
	return _ParticleSystems[index];
}

bool ParticleManager::SetParticleSystem(int index, ParticleSystem* ps)
{
	if (ps == NULL)
	{
		SEthrow(ArgumentNullException());
		return false;
	}
	else
	{
		_ParticleSystems.SetItem(index, ps);
		return true;
	}
}

}
