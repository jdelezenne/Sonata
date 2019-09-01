/*=============================================================================
D3D9HLSLShaderParameter.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D9HLSLShaderParameter.h"
#include "D3D9HLSLShaderProgram.h"
#include "D3D9Helper.h"

namespace SE_D3D9
{

D3D9HLSLShaderParameter::D3D9HLSLShaderParameter(D3D9HLSLShaderProgram* shaderProgram, D3DXHANDLE handle) :
	ShaderParameter(),
	_shaderProgram(shaderProgram),
	_handle(handle)
{
	Initialize();
}

D3D9HLSLShaderParameter::~D3D9HLSLShaderParameter()
{
	ParameterList::Iterator it = _members.GetIterator();
	while (it.Next())
	{
		delete it.Current();
	}
}

void D3D9HLSLShaderParameter::Initialize()
{
	ID3DXConstantTable* d3dxConstantTable = _shaderProgram->GetD3DXConstantTable();
	if (d3dxConstantTable == NULL)
		return;

	D3DXCONSTANT_DESC desc;
	UINT count;
	if (FAILED(d3dxConstantTable->GetConstantDesc(_handle, &desc, &count)))
		return;

	_parameterDesc.Bytes = desc.Bytes;
	_parameterDesc.StructMembers = desc.StructMembers;
	_parameterDesc.Elements = desc.Elements;
	_parameterDesc.Columns = desc.Columns;
	_parameterDesc.Rows = desc.Rows;
	_parameterDesc.ParameterType = D3D9Helper::GetParameterType(desc.Type);
	_parameterDesc.ParameterClass = D3D9Helper::GetParameterClass(desc.Class);

	UINT index;
	for (index = 0; index < desc.StructMembers; ++index)
	{
		D3DXHANDLE hParameter = d3dxConstantTable->GetConstant(_handle, index);
		D3D9HLSLShaderParameter* parameter = new D3D9HLSLShaderParameter(_shaderProgram, hParameter);
		_members.Add(parameter);
	}
}

ShaderParameter* D3D9HLSLShaderParameter::GetMemberByIndex(int index) const
{
	return _members[index];
}

ShaderParameter* D3D9HLSLShaderParameter::GetMemberByName(const String& name) const
{
	ParameterList::Iterator it = _members.GetIterator();
	while (it.Next())
	{
		D3D9HLSLShaderParameter* member = it.Current();
		if (String::Compare(member->GetName(), name) == 0)
		{
			return member;
		}
	}

	return NULL;
}

bool D3D9HLSLShaderParameter::SetValue(void* value)
{
	ID3DXConstantTable* d3dxConstantTable = _shaderProgram->GetD3DXConstantTable();
	if (d3dxConstantTable == NULL)
		return false;

	// Set the constant value
	HRESULT hr;
	hr = d3dxConstantTable->SetValue(_shaderProgram->GetD3DDevice(), _handle, value, _parameterDesc.Bytes);

	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D9HLSLShaderParameter.SetValue"),
			_T("Failed to set the value of the parameter."));
		return false;
	}

	return true;
}

}
