/*=============================================================================
D3D9EffectTechnique.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D9EffectTechnique.h"

namespace SE_D3D9
{

D3D9EffectTechnique::D3D9EffectTechnique(D3D9EffectShader* effect, D3DXHANDLE handle) :
	D3D9EffectVariable(effect, handle),
	_activePass(NULL)
{
	Initialize();
}

D3D9EffectTechnique::~D3D9EffectTechnique()
{
	{
		PassList::Iterator it = _passes.GetIterator();
		while (it.Next())
		{
			delete it.Current();
		}
	}
	{
		AnnotationList::Iterator it = _annotations.GetIterator();
		while (it.Next())
		{
			delete it.Current();
		}
	}
}

void D3D9EffectTechnique::Initialize()
{
	ID3DXEffect* effect = _effect->GetD3DXEffect();
	if (effect == NULL)
		return;

	D3DXTECHNIQUE_DESC desc;
	effect->GetTechniqueDesc(_handle, &desc);
	_name = desc.Name;

	UINT index;
	for (index = 0; index < desc.Passes; ++index)
	{
		D3DXHANDLE hPass = effect->GetPass(_handle, index);
		D3D9EffectPass* pass = new D3D9EffectPass(_effect, hPass);
		_passes.Add(pass);
	}

	for (index = 0; index < desc.Annotations; ++index)
	{
		D3DXHANDLE hAnnotation = effect->GetAnnotation(_handle, index);
		D3D9EffectAnnotation* annotation = new D3D9EffectAnnotation(_effect, hAnnotation);
		_annotations.Add(annotation);
	}
}

int D3D9EffectTechnique::GetPassCount() const
{
	return _passes.Count();
}

EffectPass* D3D9EffectTechnique::GetPassByIndex(int index) const
{
	return _passes[index];
}

EffectPass* D3D9EffectTechnique::GetPassByName(const String& name) const
{
	PassList::Iterator it = _passes.GetIterator();
	while (it.Next())
	{
		D3D9EffectPass* pass = it.Current();
		if (String::Compare(pass->GetName(), name) == 0)
		{
			return pass;
		}
	}

	return NULL;
}

int D3D9EffectTechnique::GetAnnotationCount() const
{
	return _annotations.Count();
}

EffectAnnotation* D3D9EffectTechnique::GetAnnotationByIndex(int index) const
{
	return _annotations[index];
}

EffectAnnotation* D3D9EffectTechnique::GetAnnotationByName(const String& name) const
{
	AnnotationList::Iterator it = _annotations.GetIterator();
	while (it.Next())
	{
		D3D9EffectAnnotation* annotation = it.Current();
		if (String::Compare(annotation->GetName(), name) == 0)
		{
			return annotation;
		}
	}

	return NULL;
}

void D3D9EffectTechnique::BeginPass(EffectPass* pass)
{
	D3D9EffectPass* d3dPass = (D3D9EffectPass*)pass;
	if (_passes.Contains(d3dPass))
	{
		UINT index = _passes.IndexOf(d3dPass);
		_effect->GetD3DXEffect()->BeginPass(index);
		_activePass = d3dPass;
	}
}

void D3D9EffectTechnique::BeginPassFromIndex(int index)
{
	_effect->GetD3DXEffect()->BeginPass(index);
}

EffectPass* D3D9EffectTechnique::GetActivePass()
{
	return _activePass;
}

void D3D9EffectTechnique::EndPass()
{
	_effect->GetD3DXEffect()->EndPass();
	_activePass = NULL;
}

}
