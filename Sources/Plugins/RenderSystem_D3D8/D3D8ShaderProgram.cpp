/*=============================================================================
D3D8ShaderProgram.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D8ShaderProgram.h"

namespace SE_D3D8
{

D3DXPARAMETERTYPE GetParameterType(ShaderVariableType value)
{
	switch (value)
	{
	case ShaderVariableType_Void: return D3DXPT_DWORD;
	case ShaderVariableType_Boolean: return D3DXPT_DWORD;
	case ShaderVariableType_Integer: return D3DXPT_DWORD;
	case ShaderVariableType_Float: return D3DXPT_FLOAT;
	case ShaderVariableType_String: return D3DXPT_STRING;
	case ShaderVariableType_Texture: return D3DXPT_TEXTURE;
	case ShaderVariableType_Texture1D: return D3DXPT_TEXTURE;
	case ShaderVariableType_Texture2D: return D3DXPT_TEXTURE;
	case ShaderVariableType_Texture3D: return D3DXPT_TEXTURE;
	case ShaderVariableType_TextureCube: return D3DXPT_TEXTURE;
	case ShaderVariableType_Sampler: return D3DXPT_DWORD;
	case ShaderVariableType_Sampler1D: return D3DXPT_DWORD;
	case ShaderVariableType_Sampler2D: return D3DXPT_DWORD;
	case ShaderVariableType_Sampler3D: return D3DXPT_DWORD;
	case ShaderVariableType_SamplerCube: return D3DXPT_DWORD;
	case ShaderVariableType_PixelShader: return D3DXPT_PIXELSHADER;
	case ShaderVariableType_VertexShader: return D3DXPT_VERTEXSHADER;
	case ShaderVariableType_GeometryShader: return D3DXPT_DWORD;
	default: return D3DXPT_DWORD;
	}
}

D3DXPARAMETERTYPE GetParameterClass(ShaderVariableClass value)
{
	switch (value)
	{
	case ShaderVariableClass_Scalar: return D3DXPT_FLOAT;
	case ShaderVariableClass_Vector: return D3DXPT_VECTOR;
	case ShaderVariableClass_Matrix_Rows: return D3DXPT_MATRIX;
	case ShaderVariableClass_Matrix_Columns: return D3DXPT_MATRIX;
	case ShaderVariableClass_Object: return D3DXPT_DWORD;
	case ShaderVariableClass_Struct: return D3DXPT_DWORD;
	default: return D3DXPT_DWORD;
	}
}

D3D8ShaderProgram::D3D8ShaderProgram(IDirect3DDevice8* pD3DDevice) :
	ShaderProgram(),
	_D3DDevice(pD3DDevice),
	_D3DShaderBuffer(NULL),
	_IsCompiled(false)
{
}

D3D8ShaderProgram::~D3D8ShaderProgram()
{
	if (_D3DShaderBuffer != NULL)
		_D3DShaderBuffer->Release();
}

int D3D8ShaderProgram::GetParameterCount() const
{
	return 0;
}

ShaderParameter* D3D8ShaderProgram::GetParameterByIndex(int index) const
{
	return NULL;
}

ShaderParameter* D3D8ShaderProgram::GetParameterByName(const String& name) const
{
	return NULL;
}

bool D3D8ShaderProgram::SetParameter(int index, ShaderParameter* parameter)
{
	return false;
}

bool D3D8ShaderProgram::SetParameter(const String& name, ShaderParameter* parameter)
{
	return false;
}

bool D3D8ShaderProgram::Compile()
{
	HRESULT hr;

	ID3DXBuffer* pErrorMsgs;

	if (_D3DShaderBuffer != NULL)
		_D3DShaderBuffer->Release();

	if (_EntryPoint.IsEmpty())
	{
		_EntryPoint = _T("main");
	}

	hr = D3DXAssembleShader(_SourceData.Data(), _SourceData.Length(), 0, NULL,
		&_D3DShaderBuffer, &pErrorMsgs);

	if (pErrorMsgs != NULL)
		_ErrorMessages = (SEchar*)pErrorMsgs->GetBufferPointer();
	else
		_ErrorMessages = String::Empty;

	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D8ShaderProgram.Compile (D3DXAssembleShader)"),
			_T("Failed to assemble the shader program: ") + _ErrorMessages);

		_IsCompiled = false;
		return false;
	}

	_IsCompiled = true;

	return true;
}

D3D8VertexShaderProgram::D3D8VertexShaderProgram(IDirect3DDevice8* pD3DDevice) :
	D3D8ShaderProgram(pD3DDevice),
	_D3DVertexShader(0)
{
}

D3D8VertexShaderProgram::~D3D8VertexShaderProgram()
{
	Destroy();
}

bool D3D8VertexShaderProgram::Create()
{
	HRESULT hr;

	if (_D3DShaderBuffer == NULL)
		return false;

	Destroy();

	//TODO
	DWORD* d3dDeclaration;
	hr = _D3DDevice->CreateVertexShader(d3dDeclaration, (DWORD*)_D3DShaderBuffer->GetBufferPointer(), &_D3DVertexShader, 0);

	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D8VertexShaderProgram.Create (CreateVertexShader)"),
			_T("Failed to create the shader program."));

		return false;
	}

	return true;
}

void D3D8VertexShaderProgram::Destroy()
{
	if (_D3DVertexShader != 0)
	{
		_D3DDevice->DeleteVertexShader(_D3DVertexShader);
		_D3DVertexShader = 0;
	}
}

bool D3D8VertexShaderProgram::Bind()
{
	if (_D3DVertexShader == 0)
		return false;

	_D3DDevice->SetVertexShader(_D3DVertexShader);

	return true;
}

bool D3D8VertexShaderProgram::Unbind()
{
	if (_D3DVertexShader == 0)
		return false;

	_D3DDevice->SetVertexShader(0);

	return true;
}

D3D8PixelShaderProgram::D3D8PixelShaderProgram(IDirect3DDevice8* pD3DDevice) :
	D3D8ShaderProgram(pD3DDevice),
	_D3DPixelShader(0)
{
}

D3D8PixelShaderProgram::~D3D8PixelShaderProgram()
{
	Destroy();
}

bool D3D8PixelShaderProgram::Create()
{
	HRESULT hr;

	if (_D3DShaderBuffer == NULL)
		return false;

	Destroy();

	hr = _D3DDevice->CreatePixelShader((DWORD*)_D3DShaderBuffer->GetBufferPointer(), &_D3DPixelShader);

	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D8PixelShaderProgram.Create (CreatePixelShader)"),
			_T("Failed to create the shader program."));

		return false;
	}

	return true;
}

void D3D8PixelShaderProgram::Destroy()
{
	if (_D3DPixelShader != 0)
	{
		_D3DDevice->DeletePixelShader(_D3DPixelShader);
		_D3DPixelShader = 0;
	}
}

bool D3D8PixelShaderProgram::Bind()
{
	if (_D3DPixelShader == 0)
		return false;

	_D3DDevice->SetPixelShader(_D3DPixelShader);

	return true;
}

bool D3D8PixelShaderProgram::Unbind()
{
	if (_D3DPixelShader == 0)
		return false;

	_D3DDevice->SetPixelShader(0);

	return true;
}

}
