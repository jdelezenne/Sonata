/*=============================================================================
EffectMaterial.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "EffectMaterial.h"

namespace SonataEngine
{

EffectMaterial::EffectMaterial() :
	ShaderMaterial(),
	_effectShader(NULL)
{
}

EffectMaterial::~EffectMaterial()
{
}

void EffectMaterial::SetEffectShader(EffectShader* value)
{
	_effectShader = value;
	SetTechnique(_techniqueName);
	Initialize();
}

void EffectMaterial::SetTechnique(const String& value)
{
	_techniqueName = value;
	_activeTechnique = NULL;
	if (_effectShader != NULL)
	{
		if (_techniqueName.IsEmpty())
		{
			_activeTechnique = _effectShader->GetTechniqueByIndex(0);
		}
		else
		{
			_activeTechnique = _effectShader->GetTechniqueByName(_techniqueName);
		}
	}
}

void EffectMaterial::Initialize()
{
}

void EffectMaterial::SetupMaterial(SceneState* sceneState)
{
}

void EffectMaterial::SetupGeometry(MeshPart* meshPart)
{
}

void EffectMaterial::SetupPass(SceneState* sceneState, MeshPart* meshPart)
{
}

void EffectMaterial::BeginMaterial() const
{
	if (_effectShader != NULL && _activeTechnique != NULL)
	{
		_effectShader->BeginTechnique(_activeTechnique);
	}
}

void EffectMaterial::EndMaterial() const
{
	if (_effectShader != NULL && _activeTechnique != NULL)
	{
		_effectShader->EndTechnique();
	}
}

int EffectMaterial::GetPassCount() const
{
	if (_effectShader != NULL && _activeTechnique != NULL)
	{
		return _activeTechnique->GetPassCount();
	}
	else
	{
		return 0;
	}
}

void EffectMaterial::BeginPass(int index) const
{
	if (_effectShader != NULL && _activeTechnique != NULL)
	{
		_activeTechnique->BeginPassFromIndex(index);
	}
}

void EffectMaterial::EndPass() const
{
	if (_effectShader != NULL && _activeTechnique != NULL)
	{
		_activeTechnique->EndPass();
	}
}

}
