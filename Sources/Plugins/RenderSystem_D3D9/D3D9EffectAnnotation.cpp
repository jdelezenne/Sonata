/*=============================================================================
D3D9EffectAnnotation.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D9EffectAnnotation.h"
#include "D3D9Helper.h"

namespace SE_D3D9
{

D3D9EffectAnnotation::D3D9EffectAnnotation(D3D9EffectShader* effect, D3DXHANDLE handle) :
	D3D9EffectVariable(effect, handle)
{
	Initialize();
}

D3D9EffectAnnotation::~D3D9EffectAnnotation()
{
}

void D3D9EffectAnnotation::Initialize()
{
	ID3DXEffect* effect = _effect->GetD3DXEffect();
	if (effect == NULL)
		return;

	D3DXPARAMETER_DESC desc;
	effect->GetParameterDesc(_handle, &desc);
	_name = desc.Name;

	_parameterDesc.Bytes = desc.Bytes;
	_parameterDesc.StructMembers = desc.StructMembers;
	_parameterDesc.Elements = desc.Elements;
	_parameterDesc.Columns = desc.Columns;
	_parameterDesc.Rows = desc.Rows;
	_parameterDesc.ParameterType = D3D9Helper::GetParameterType(desc.Type);
	_parameterDesc.ParameterClass = D3D9Helper::GetParameterClass(desc.Class);
	_parameterDesc.Semantic = desc.Semantic;
}

bool D3D9EffectAnnotation::GetValueBoolean()
{
	bool value = false;
	if (_parameterDesc.ParameterType == ShaderParameterType_Boolean &&
		_parameterDesc.ParameterClass == ShaderParameterClass_Scalar)
	{
		GetValue(&value);
		return value;
	}
	else
	{
		return false;
	}
}

int32 D3D9EffectAnnotation::GetValueInt32()
{
	int32 value = 0;
	if (_parameterDesc.ParameterType == ShaderParameterType_Integer &&
		_parameterDesc.ParameterClass == ShaderParameterClass_Scalar)
	{
		GetValue(&value);
		return value;
	}
	else
	{
		return 0;
	}
}

Matrix4 D3D9EffectAnnotation::GetValueMatrix()
{
	Matrix4 value = Matrix4::Identity;
	if (_parameterDesc.ParameterType == ShaderParameterType_Float &&
		(_parameterDesc.ParameterClass == ShaderParameterClass_Matrix_Columns ||
		_parameterDesc.ParameterClass == ShaderParameterClass_Matrix_Rows) &&
		_parameterDesc.Elements == 16)
	{
		GetValue(&value);
		return value;
	}
	else
	{
		return Matrix4::Identity;
	}
}

real32 D3D9EffectAnnotation::GetValueSingle()
{
	real32 value = 0.0f;
	if (_parameterDesc.ParameterType == ShaderParameterType_Float &&
		_parameterDesc.ParameterClass == ShaderParameterClass_Scalar)
	{
		GetValue(&value);
		return value;
	}
	else
	{
		return 0.0f;
	}
}

String D3D9EffectAnnotation::GetValueString()
{
	SEbyte* pData[4];
	if (_parameterDesc.ParameterType == ShaderParameterType_String)
	{
		GetValue(pData);
		return String(*(SEchar**)pData);
	}
	else
	{
		return String::Empty;
	}
}

Vector2 D3D9EffectAnnotation::GetValueVector2()
{
	Vector2 value = Vector2::Zero;
	if (_parameterDesc.ParameterType == ShaderParameterType_Float &&
		_parameterDesc.ParameterClass == ShaderParameterClass_Scalar &&
		_parameterDesc.Elements == 2)
	{
		GetValue(&value);
		return value;
	}
	else
	{
		return Vector2::Zero;
	}
}

Vector3 D3D9EffectAnnotation::GetValueVector3()
{
	Vector3 value = Vector3::Zero;
	if (_parameterDesc.ParameterType == ShaderParameterType_Float &&
		_parameterDesc.ParameterClass == ShaderParameterClass_Vector &&
		_parameterDesc.Elements == 3)
	{
		GetValue(&value);
		return value;
	}
	else
	{
		return Vector3::Zero;
	}
}

Vector4 D3D9EffectAnnotation::GetValueVector4()
{
	Vector4 value = Vector4::Zero;
	if (_parameterDesc.ParameterType == ShaderParameterType_Float &&
		_parameterDesc.ParameterClass == ShaderParameterClass_Vector &&
		_parameterDesc.Elements == 4)
	{
		GetValue(&value);
		return value;
	}
	else
	{
		return Vector4::Zero;
	}
}

bool D3D9EffectAnnotation::GetValue(void* value)
{
	ID3DXEffect* d3dxEffect = _effect->GetD3DXEffect();

	HRESULT hr;
	hr = d3dxEffect->GetValue(_handle, value, _parameterDesc.Bytes);

	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D9EffectAnnotation.GetValue"),
			_T("Failed to get the value of the annotation."));
		return false;
	}

	return true;
}

}
