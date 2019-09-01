/*=============================================================================
D3D9HLSLShaderSystem.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D9HLSLShaderSystem.h"
#include "D3D9HLSLShaderProgram.h"
#include "D3D9EffectShader.h"

namespace SE_D3D9
{

D3D9HLSLShaderSystem::D3D9HLSLShaderSystem(IDirect3DDevice9* pD3DDevice) :
	ShaderSystem(),
	_D3DDevice(pD3DDevice)
{
}

D3D9HLSLShaderSystem::~D3D9HLSLShaderSystem()
{
}

ShaderProgram* D3D9HLSLShaderSystem::CreateShaderProgram(ShaderProgramType type)
{
	ShaderProgram* program;

	if (type == ShaderProgramType_Vertex)
	{
		program = new D3D9HLSLVertexShaderProgram(this);
	}
	else if (type == ShaderProgramType_Pixel)
	{
		program = new D3D9HLSLPixelShaderProgram(this);
	}
	else
	{
		return NULL;
	}

	return program;
}

void D3D9HLSLShaderSystem::DestroyShaderProgram(ShaderProgram* program)
{
	if (program == NULL || program->GetShaderSystem() != this)
	{
		return;
	}

	delete program;
}

bool D3D9HLSLShaderSystem::SetShaderProgram(ShaderProgram* program)
{
	if (program == NULL || program->GetShaderSystem() != this)
	{
		return false;
	}

	return program->Bind();
}

bool D3D9HLSLShaderSystem::DisableShaderProgram(ShaderProgram* program)
{
	if (program == NULL || program->GetShaderSystem() != this)
	{
		return false;
	}

	return program->Unbind();
}

EffectShader* D3D9HLSLShaderSystem::CreateEffectShader(const String& source)
{
	HRESULT hr;

	if (source.IsEmpty())
		return NULL;

	File sourceFile(source);
	FileStreamPtr stream = sourceFile.Open(FileMode_Open, FileAccess_Read);
	if (stream == NULL)
		return NULL;

	int32 length = stream->GetLength();
	SEbyte* data = new SEbyte[length];
	if (stream->Read(data, length) == 0)
	{
		delete[] data;
		return NULL;
	}

	ID3DXEffect* pEffect;
	ID3DXBuffer* pCompilationErrors;

	hr = D3DXCreateEffect(_D3DDevice, data, length, NULL, NULL,
		D3DXSHADER_PACKMATRIX_ROWMAJOR, NULL, &pEffect, &pCompilationErrors);
	delete[] data;

	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D9HLSLShaderSystem.CreateShader (D3DXCreateEffect)"),
			_T("Failed to create a shader effect."));

		if (pCompilationErrors != NULL)
		{
			Logger::Current()->Log(LogLevel::Error, _T("D3D9HLSLShaderSystem.CreateShader"),
				(SEchar*)pCompilationErrors->GetBufferPointer());
		}
		return NULL;
	}

	D3D9EffectShader* shader = new D3D9EffectShader(this, pEffect);

	return shader;
}

void D3D9HLSLShaderSystem::DestroyEffectShader(EffectShader* shader)
{
	if (shader == NULL || shader->GetShaderSystem() != this)
	{
		return;
	}

	delete shader;
}

}
