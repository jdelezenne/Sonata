/*=============================================================================
D3D9EffectParameter.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D9EffectParameter.h"
#include "D3D9Helper.h"
#include "D3D9Texture.h"

namespace SE_D3D9
{

D3D9EffectParameter::D3D9EffectParameter(D3D9EffectShader* effect, D3DXHANDLE handle) :
	D3D9EffectVariable(effect, handle)
{
	Initialize();
}

D3D9EffectParameter::~D3D9EffectParameter()
{
	{
		ParameterList::Iterator it = _members.GetIterator();
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

void D3D9EffectParameter::Initialize()
{
	ID3DXEffect* d3dxEffect = _effect->GetD3DXEffect();
	if (d3dxEffect == NULL)
		return;

	D3DXPARAMETER_DESC desc;
	if (FAILED(d3dxEffect->GetParameterDesc(_handle, &desc)))
		return;

	_name = desc.Name;

	_parameterDesc.Bytes = desc.Bytes;
	_parameterDesc.StructMembers = desc.StructMembers;
	_parameterDesc.Elements = desc.Elements;
	_parameterDesc.Columns = desc.Columns;
	_parameterDesc.Rows = desc.Rows;
	_parameterDesc.ParameterType = D3D9Helper::GetParameterType(desc.Type);
	_parameterDesc.ParameterClass = D3D9Helper::GetParameterClass(desc.Class);
	_parameterDesc.Semantic = desc.Semantic;

	UINT index;
	for (index = 0; index < desc.StructMembers; ++index)
	{
		D3DXHANDLE hParameter = d3dxEffect->GetParameter(_handle, index);
		D3D9EffectParameter* parameter = new D3D9EffectParameter(_effect, hParameter);
		_members.Add(parameter);
	}

	for (index = 0; index < desc.Annotations; ++index)
	{
		D3DXHANDLE hAnnotation = d3dxEffect->GetAnnotation(_handle, index);
		D3D9EffectAnnotation* annotation = new D3D9EffectAnnotation(_effect, hAnnotation);
		_annotations.Add(annotation);
	}
}

EffectParameter* D3D9EffectParameter::GetMemberByIndex(int index) const
{
	return _members[index];
}

EffectParameter* D3D9EffectParameter::GetMemberByName(const String& name) const
{
	ParameterList::Iterator it = _members.GetIterator();
	while (it.Next())
	{
		D3D9EffectParameter* member = it.Current();
		if (String::Compare(member->GetName(), name) == 0)
		{
			return member;
		}
	}

	return NULL;
}

EffectParameter* D3D9EffectParameter::GetMemberBySemantic(const String& semantic) const
{
	ParameterList::Iterator it = _members.GetIterator();
	while (it.Next())
	{
		D3D9EffectParameter* member = it.Current();
		if (String::Compare(member->GetParameterDesc().Semantic, semantic) == 0)
		{
			return member;
		}
	}

	return NULL;
}

bool D3D9EffectParameter::GetValue(void** value)
{
	ID3DXEffect* d3dxEffect = _effect->GetD3DXEffect();
	if (d3dxEffect == NULL)
		return false;

	HRESULT hr;
	hr = d3dxEffect->GetValue(_handle, value, _parameterDesc.Bytes);

	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D9EffectParameter.GetValue"),
			_T("Failed to get the value of the parameter."));
		return false;
	}

	return true;
}

bool D3D9EffectParameter::SetValue(void* value)
{
	ID3DXEffect* d3dxEffect = _effect->GetD3DXEffect();
	if (d3dxEffect == NULL)
		return false;

	// Set the parameter value
	HRESULT hr;
	bool useDefault = true;
	switch (_parameterDesc.ParameterType)
	{
	case ShaderParameterType_Texture:
		hr = d3dxEffect->SetTexture(_handle, ((D3D9Texture*)value)->GetD3DBaseTexture());
		useDefault = false;
		break;

	case ShaderParameterType_Float:
		if (_parameterDesc.ParameterClass == ShaderParameterClass_Matrix_Rows ||
			_parameterDesc.ParameterClass == ShaderParameterClass_Matrix_Columns)
		{
			if (_parameterDesc.Elements == 0)
			{
				D3DXMATRIX matrix = D3D9Helper::MakeD3DXMatrix(*(Matrix4*)value);
				hr = d3dxEffect->SetMatrix(_handle, &matrix);
			}
			else
			{
				D3DXMATRIX* matrices = new D3DXMATRIX[_parameterDesc.Elements];
				for (int i = 0; i < _parameterDesc.Elements; ++i)
					matrices[i] = D3D9Helper::MakeD3DXMatrix(*(((Matrix4*)value)+i));
				hr = d3dxEffect->SetMatrixArray(_handle, matrices, _parameterDesc.Elements);
				delete[] matrices;
			}
			useDefault = false;
		}
		break;
	}

	if (useDefault)
	{
		hr = d3dxEffect->SetValue(_handle, value, _parameterDesc.Bytes);
	}

	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D9EffectParameter.SetValue"),
			_T("Failed to set the value of the parameter."));
		return false;
	}

	d3dxEffect->CommitChanges();
	return true;
}

int D3D9EffectParameter::GetAnnotationCount() const
{
	return _annotations.Count();
}

EffectAnnotation* D3D9EffectParameter::GetAnnotationByIndex(int index) const
{
	return _annotations[index];
}

EffectAnnotation* D3D9EffectParameter::GetAnnotationByName(const String& name) const
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
