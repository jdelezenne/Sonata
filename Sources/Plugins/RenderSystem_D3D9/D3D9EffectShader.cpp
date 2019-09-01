/*=============================================================================
D3D9EffectShader.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D9EffectShader.h"
#include "D3D9Helper.h"
#include "D3D9Texture.h"
#include "D3D9EffectParameter.h"
#include "D3D9EffectTechnique.h"
#include "D3D9EffectFunction.h"

namespace SE_D3D9
{

D3D9EffectVariable::D3D9EffectVariable(D3D9EffectShader* effect, D3DXHANDLE handle) :
	_effect(effect),
	_handle(handle)
{
}

D3D9EffectVariable::~D3D9EffectVariable()
{
}


D3D9EffectShader::D3D9EffectShader(D3D9HLSLShaderSystem* shaderSystem, ID3DXEffect* pD3DEffect) :
	EffectShader(),
	_shaderSystem(shaderSystem),
	_D3DXEffect(pD3DEffect)
{
	Initialize();
}

D3D9EffectShader::~D3D9EffectShader()
{
	{
		ParameterList::Iterator it = _parameters.GetIterator();
		while (it.Next())
		{
			delete it.Current();
		}
	}
	{
		TechniqueList::Iterator it = _techniques.GetIterator();
		while (it.Next())
		{
			delete it.Current();
		}
	}
	{
		FunctionList::Iterator it = _functions.GetIterator();
		while (it.Next())
		{
			delete it.Current();
		}
	}
}

void D3D9EffectShader::Initialize()
{
	if (_D3DXEffect == NULL)
		return;

	D3DXEFFECT_DESC desc;
	_D3DXEffect->GetDesc(&desc);

	UINT index;
	for (index = 0; index < desc.Parameters; ++index)
	{
		D3DXHANDLE hParameter = _D3DXEffect->GetParameter(NULL, index);
		D3D9EffectParameter* parameter = new D3D9EffectParameter(this, hParameter);
		_parameters.Add(parameter);
	}

	for (index = 0; index < desc.Techniques; ++index)
	{
		D3DXHANDLE hTechnique = _D3DXEffect->GetTechnique(index);
		D3D9EffectTechnique* technique = new D3D9EffectTechnique(this, hTechnique);
		_techniques.Add(technique);
	}

	for (index = 0; index < desc.Functions; ++index)
	{
		D3DXHANDLE hFunction = _D3DXEffect->GetFunction(index);
		D3D9EffectFunction* function = new D3D9EffectFunction(this, hFunction);
		_functions.Add(function);
	}
}

int D3D9EffectShader::GetParameterCount() const
{
	return _parameters.Count();
}

EffectParameter* D3D9EffectShader::GetParameterByIndex(int index) const
{
	return _parameters[index];
}

EffectParameter* D3D9EffectShader::GetParameterByName(const String& name) const
{
	ParameterList::Iterator it = _parameters.GetIterator();
	while (it.Next())
	{
		D3D9EffectParameter* parameter = it.Current();
		if (String::Compare(parameter->GetName(), name) == 0)
		{
			return parameter;
		}
	}

	return NULL;
}

EffectParameter* D3D9EffectShader::GetParameterBySemantic(const String& semantic) const
{
	ParameterList::Iterator it = _parameters.GetIterator();
	while (it.Next())
	{
		D3D9EffectParameter* parameter = it.Current();
		if (String::Compare(parameter->GetParameterDesc().Semantic, semantic) == 0)
		{
			return parameter;
		}
	}

	return NULL;
}

int D3D9EffectShader::GetTechniqueCount() const
{
	return _techniques.Count();
}

EffectTechnique* D3D9EffectShader::GetTechniqueByIndex(int index) const
{
	return _techniques[index];
}

EffectTechnique* D3D9EffectShader::GetTechniqueByName(const String& name) const
{
	TechniqueList::Iterator it = _techniques.GetIterator();
	while (it.Next())
	{
		D3D9EffectTechnique* technique = it.Current();
		if (String::Compare(technique->GetName(), name) == 0)
		{
			return technique;
		}
	}

	return NULL;
}

int D3D9EffectShader::GetFunctionCount() const
{
	return _functions.Count();
}

EffectFunction* D3D9EffectShader::GetFunctionByIndex(int index) const
{
	return _functions[index];
}

EffectFunction* D3D9EffectShader::GetFunctionByName(const String& name) const
{
	FunctionList::Iterator it = _functions.GetIterator();
	while (it.Next())
	{
		D3D9EffectFunction* function = it.Current();
		if (String::Compare(function->GetName(), name) == 0)
		{
			return function;
		}
	}

	return NULL;
}

void D3D9EffectShader::BeginTechnique(EffectTechnique* technique)
{
	if (_techniques.Contains((D3D9EffectTechnique*)technique))
	{
		_D3DXEffect->SetTechnique(((D3D9EffectTechnique*)technique)->GetHandle());

		UINT passes;
		_D3DXEffect->Begin(&passes, 0);
	}
}

void D3D9EffectShader::BeginTechniqueFromIndex(int index)
{
	BeginTechnique(_techniques[index]);
}

EffectTechnique* D3D9EffectShader::GetActiveTechnique()
{
	D3DXHANDLE hTechnique = _D3DXEffect->GetCurrentTechnique();
	D3DXTECHNIQUE_DESC desc;
	_D3DXEffect->GetTechniqueDesc(hTechnique, &desc);

	TechniqueList::Iterator it = _techniques.GetIterator();
	while (it.Next())
	{
		D3D9EffectTechnique* technique = it.Current();
		if (technique->GetHandle() == hTechnique)
		{
			return technique;
		}
	}

	return NULL;
}

void D3D9EffectShader::EndTechnique()
{
	_D3DXEffect->End();
}

}
