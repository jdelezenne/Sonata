/*=============================================================================
FFPEffectShader.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "FFPEffectShader.h"

namespace SonataEngine
{

SE_IMPLEMENT_CLASS(FFPEffectShader);

FFPEffectShader::FFPEffectShader() :
	EffectShader(),
	_activeTechnique(NULL)
{
}

FFPEffectShader::~FFPEffectShader()
{
	TechniqueList::Iterator it = _techniques.GetIterator();
	while (it.Next())
	{
		delete it.Current();
	}
}

int FFPEffectShader::GetParameterCount() const
{
	return 0;
}

EffectParameter* FFPEffectShader::GetParameterByIndex(int index) const
{
	return NULL;
}

EffectParameter* FFPEffectShader::GetParameterByName(const String& name) const
{
	return NULL;
}

EffectParameter* FFPEffectShader::GetParameterBySemantic(const String& semantic) const
{
	return NULL;
}

int FFPEffectShader::GetTechniqueCount() const
{
	return _techniques.Count();
}

EffectTechnique* FFPEffectShader::GetTechniqueByIndex(int index) const
{
	return _techniques[index];
}

EffectTechnique* FFPEffectShader::GetTechniqueByName(const String& name) const
{
	TechniqueList::Iterator it = _techniques.GetIterator();
	while (it.Next())
	{
		if (String::Compare(it.Current()->GetName(), name, true) == 0)
		{
			return it.Current();
		}
	}

	return NULL;
}

int FFPEffectShader::GetFunctionCount() const
{
	return 0;
}

EffectFunction* FFPEffectShader::GetFunctionByIndex(int index) const
{
	return NULL;
}

EffectFunction* FFPEffectShader::GetFunctionByName(const String& name) const
{
	return NULL;
}

void FFPEffectShader::BeginTechnique(EffectTechnique* technique)
{
	if (_techniques.Contains((FFPTechnique*)technique))
	{
		_activeTechnique = (FFPTechnique*)technique;
	}
}

EffectTechnique* FFPEffectShader::GetActiveTechnique()
{
	return _activeTechnique;
}

void FFPEffectShader::BeginTechniqueFromIndex(int index)
{
	_activeTechnique = _techniques[index];
}

void FFPEffectShader::EndTechnique()
{
	_activeTechnique = NULL;
}

bool FFPEffectShader::AddTechnique(FFPTechnique* value)
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return false;
	}

	_techniques.Add(value);
	return true;
}

bool FFPEffectShader::RemoveTechnique(FFPTechnique* value)
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return false;
	}

	if (_activeTechnique == value)
	{
		_activeTechnique  = NULL;
	}
	_techniques.Remove(value);
	return true;
}

void FFPEffectShader::RemoveAllTechniques()
{
	_activeTechnique = NULL;
	_techniques.Clear();
}

}
