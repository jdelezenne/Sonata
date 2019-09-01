/*=============================================================================
D3D9EffectPass.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D9EffectPass.h"
#include "D3D9Helper.h"

namespace SE_D3D9
{

D3D9EffectPass::D3D9EffectPass(D3D9EffectShader* effect, D3DXHANDLE handle) :
	D3D9EffectVariable(effect, handle),
	_vertexProgram(NULL),
	_pixelProgram(NULL)
{
	Initialize();
}

D3D9EffectPass::~D3D9EffectPass()
{
	AnnotationList::Iterator it = _annotations.GetIterator();
	while (it.Next())
	{
		delete it.Current();
	}
}

void D3D9EffectPass::Initialize()
{
	ID3DXEffect* effect = _effect->GetD3DXEffect();
	if (effect == NULL)
		return;

	D3DXPASS_DESC desc;
	effect->GetPassDesc(_handle, &desc);
	_name = desc.Name;

	if (desc.pVertexShaderFunction != NULL)
	{
		_vertexProgram = new D3D9HLSLVertexShaderProgram((D3D9HLSLShaderSystem*)_effect->GetShaderSystem());
		_vertexProgram->Initialize(desc.pVertexShaderFunction);
	}

	if (desc.pPixelShaderFunction != NULL)
	{
		_pixelProgram = new D3D9HLSLPixelShaderProgram((D3D9HLSLShaderSystem*)_effect->GetShaderSystem());
		_pixelProgram->Initialize(desc.pPixelShaderFunction);
	}

	UINT index;
	for (index = 0; index < desc.Annotations; ++index)
	{
		D3DXHANDLE hAnnotation = effect->GetAnnotation(_handle, index);
		D3D9EffectAnnotation* annotation = new D3D9EffectAnnotation(_effect, hAnnotation);
		_annotations.Add(annotation);
	}
}

ShaderProgram* D3D9EffectPass::GetVertexProgram() const
{
	return _vertexProgram;
}

void D3D9EffectPass::SetVertexProgram(ShaderProgram* value)
{
}

ShaderProgram* D3D9EffectPass::GetPixelProgram() const
{
	return _pixelProgram;
}

void D3D9EffectPass::SetPixelProgram(ShaderProgram* value)
{
}

ShaderProgram* D3D9EffectPass::GetGeometryProgram() const
{
	return NULL;
}

void D3D9EffectPass::SetGeometryProgram(ShaderProgram* value)
{
}

int D3D9EffectPass::GetAnnotationCount() const
{
	return _annotations.Count();
}

EffectAnnotation* D3D9EffectPass::GetAnnotationByIndex(int index) const
{
	return _annotations[index];
}

EffectAnnotation* D3D9EffectPass::GetAnnotationByName(const String& name) const
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

}
