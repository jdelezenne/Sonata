/*=============================================================================
D3D9HLSLShaderProgram.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D9HLSLShaderProgram.h"
#include "D3D9Helper.h"

namespace SE_D3D9
{

D3D9HLSLShaderProgram::D3D9HLSLShaderProgram(D3D9HLSLShaderSystem* shaderSystem) :
	_shaderSystem(shaderSystem),
	_D3DDevice(NULL),
	_D3DShaderBuffer(NULL),
	_D3DConstantTable(NULL),
	_isCompiled(false)
{
	_D3DDevice = _shaderSystem->GetD3DDevice();
}

D3D9HLSLShaderProgram::~D3D9HLSLShaderProgram()
{
	if (_D3DShaderBuffer != NULL)
	{
		_D3DShaderBuffer->Release();
		_D3DShaderBuffer = NULL;
	}

	if (_D3DConstantTable != NULL)
	{
		_D3DConstantTable->Release();
		_D3DConstantTable = NULL;
	}

	ParameterList::Iterator it = _parameters.GetIterator();
	while (it.Next())
	{
		delete it.Current();
	}
}

void D3D9HLSLShaderProgram::Initialize()
{
	ParameterList::Iterator it = _parameters.GetIterator();
	while (it.Next())
	{
		delete it.Current();
	}
	_parameters.Clear();

	D3DXCONSTANTTABLE_DESC desc;
	if (FAILED(_D3DConstantTable->GetDesc(&desc)))
		return;

	UINT index;
	for (index = 0; index < desc.Constants; ++index)
	{
		D3DXHANDLE hConstant = _D3DConstantTable->GetConstant(NULL, index);
		D3D9HLSLShaderParameter* parameter = new D3D9HLSLShaderParameter(this, hConstant);
		_parameters.Add(parameter);
	}
}

int D3D9HLSLShaderProgram::GetParameterCount() const
{
	return _parameters.Count();
}

ShaderParameter* D3D9HLSLShaderProgram::GetParameterByIndex(int index) const
{
	return _parameters[index];
}

ShaderParameter* D3D9HLSLShaderProgram::GetParameterByName(const String& name) const
{
	ParameterList::Iterator it = _parameters.GetIterator();
	while (it.Next())
	{
		D3D9HLSLShaderParameter* parameter = it.Current();
		if (String::Compare(parameter->GetName(), name) == 0)
		{
			return parameter;
		}
	}

	return NULL;
}

bool D3D9HLSLShaderProgram::Compile()
{
	HRESULT hr;

	ID3DXBuffer* pErrorMsgs;

	if (_D3DShaderBuffer != NULL)
	{
		_D3DShaderBuffer->Release();
		_D3DShaderBuffer = NULL;
	}

	if (_D3DConstantTable != NULL)
	{
		_D3DConstantTable->Release();
		_D3DConstantTable = NULL;
	}

	if (_entryPoint.IsEmpty())
	{
		_entryPoint = _T("main");
	}

	// if no profile is specified, get the highest HLSL profile supported
	if (_profile.IsEmpty())
	{
		_profile = GetHighestProfile();
	}

	hr = D3DXCompileShader(_sourceData.Data(), _sourceData.Length(), NULL, NULL,
		_entryPoint.Data(), _profile.Data(), D3DXSHADER_PACKMATRIX_ROWMAJOR,
		&_D3DShaderBuffer, &pErrorMsgs, &_D3DConstantTable);

	if (pErrorMsgs != NULL)
		_errorMessages = (SEchar*)pErrorMsgs->GetBufferPointer();
	else
		_errorMessages = String::Empty;

	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D9HLSLShaderProgram.Compile (D3DXCompileShader)"),
			_T("Failed to compile the shader program: ") + _errorMessages);
		_isCompiled = false;
		return false;
	}

	_isCompiled = true;
	Initialize();

	return true;
}


D3D9HLSLVertexShaderProgram::D3D9HLSLVertexShaderProgram(D3D9HLSLShaderSystem* shaderSystem) :
	D3D9HLSLShaderProgram(shaderSystem),
	_D3DVertexShader(NULL)
{
}

D3D9HLSLVertexShaderProgram::~D3D9HLSLVertexShaderProgram()
{
	Destroy();
}

bool D3D9HLSLVertexShaderProgram::Create()
{
	if (_D3DShaderBuffer == NULL)
		return false;

	Destroy();

	return Initialize((DWORD*)_D3DShaderBuffer->GetBufferPointer());
}

void D3D9HLSLVertexShaderProgram::Destroy()
{
	if (_D3DVertexShader != NULL)
	{
		_D3DVertexShader->Release();
		_D3DVertexShader = NULL;
	}
}

bool D3D9HLSLVertexShaderProgram::Bind()
{
	if (_D3DVertexShader == NULL)
		return false;

	_D3DDevice->SetVertexShader(_D3DVertexShader);

	return true;
}

bool D3D9HLSLVertexShaderProgram::Unbind()
{
	if (_D3DVertexShader == NULL)
		return false;

	_D3DDevice->SetVertexShader(NULL);

	return true;
}

bool D3D9HLSLVertexShaderProgram::Initialize(CONST DWORD* pVertexShaderFunction)
{
	if (pVertexShaderFunction == NULL)
	{
		return false;
	}

	HRESULT hr = _D3DDevice->CreateVertexShader(pVertexShaderFunction, &_D3DVertexShader);
	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D9HLSLVertexShaderProgram.CreateVertexShader"),
			_T("Failed to create the vertex shader program."));
		return false;
	}

	if (_D3DConstantTable == NULL)
	{
		D3DXGetShaderConstantTable(pVertexShaderFunction, &_D3DConstantTable);
	}

	return true;
}

String D3D9HLSLVertexShaderProgram::GetHighestProfile()
{
	return D3DXGetVertexShaderProfile(_D3DDevice);
}


D3D9HLSLPixelShaderProgram::D3D9HLSLPixelShaderProgram(D3D9HLSLShaderSystem* shaderSystem) :
	D3D9HLSLShaderProgram(shaderSystem),
	_D3DPixelShader(NULL)
{
}

D3D9HLSLPixelShaderProgram::~D3D9HLSLPixelShaderProgram()
{
	Destroy();
}

bool D3D9HLSLPixelShaderProgram::Create()
{
	if (_D3DShaderBuffer == NULL)
		return false;

	Destroy();

	return Initialize((DWORD*)_D3DShaderBuffer->GetBufferPointer());
}

void D3D9HLSLPixelShaderProgram::Destroy()
{
	if (_D3DPixelShader != NULL)
	{
		_D3DPixelShader->Release();
		_D3DPixelShader = NULL;
	}
}

bool D3D9HLSLPixelShaderProgram::Bind()
{
	if (_D3DPixelShader == NULL)
		return false;

	_D3DDevice->SetPixelShader(_D3DPixelShader);

	return true;
}

bool D3D9HLSLPixelShaderProgram::Unbind()
{
	if (_D3DPixelShader == NULL)
		return false;

	_D3DDevice->SetPixelShader(NULL);

	return true;
}

bool D3D9HLSLPixelShaderProgram::Initialize(CONST DWORD* pPixelShaderFunction)
{
	if (pPixelShaderFunction == NULL)
	{
		return false;
	}

	HRESULT hr = _D3DDevice->CreatePixelShader(pPixelShaderFunction, &_D3DPixelShader);
	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D9HLSLPixelShaderProgram.CreatePixelShader"),
			_T("Failed to create the pixel shader program."));
		return false;
	}

	if (_D3DConstantTable == NULL)
	{
		D3DXGetShaderConstantTable(pPixelShaderFunction, &_D3DConstantTable);
	}

	return true;
}

String D3D9HLSLPixelShaderProgram::GetHighestProfile()
{
	return D3DXGetPixelShaderProfile(_D3DDevice);
}

}
