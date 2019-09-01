/*=============================================================================
D3D8ShaderSystem.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D8ShaderSystem.h"
#include "D3D8ShaderProgram.h"
#include "D3D8Shader.h"

namespace SE_D3D8
{

D3D8ShaderSystem::D3D8ShaderSystem(IDirect3DDevice8* pD3DDevice) :
	ShaderSystem(),
	_D3DDevice(pD3DDevice)
{
}

D3D8ShaderSystem::~D3D8ShaderSystem()
{
}

bool D3D8ShaderSystem::Create()
{
	return true;
}

void D3D8ShaderSystem::Destroy()
{
}

void D3D8ShaderSystem::Update(real64 elapsed)
{
}

ShaderProgram* D3D8ShaderSystem::CreateShaderProgram(ShaderProgramType type)
{
	ShaderProgram* program;

	if (type == ShaderProgramType_Vertex)
	{
		program = new D3D8VertexShaderProgram(_D3DDevice);
	}
	else if (type == ShaderProgramType_Pixel)
	{
		program = new D3D8PixelShaderProgram(_D3DDevice);
	}
	else
	{
		return NULL;
	}

	return program;
}

void D3D8ShaderSystem::DestroyShaderProgram(ShaderProgram* program)
{
	if (program == NULL)
		return;

	delete program;
}

bool D3D8ShaderSystem::SetShaderProgram(ShaderProgram* program)
{
	if (program == NULL)
		return false;

	return program->Bind();
}

bool D3D8ShaderSystem::DisableShaderProgram(ShaderProgram* program)
{
	return program->Unbind();
}

ShaderMaterial* D3D8ShaderSystem::CreateShader(Stream& stream)
{
	HRESULT hr;

	int32 length = stream.GetLength();
	SEbyte* data = new SEbyte[length];
	if (stream.Read(data, length) == 0)
		return NULL;

	ID3DXEffect* pEffect;
	ID3DXBuffer* pCompilationErrors;

	hr = D3DXCreateEffect(_D3DDevice, data, length, &pEffect, &pCompilationErrors);
	delete[] data;

	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D8ShaderSystem.CreateShader (D3DXCreateEffect)"),
			_T("Failed to create a shader effect."));

		if (pCompilationErrors != NULL)
		{
			Logger::Current()->Log(LogLevel::Error, _T("D3D8ShaderSystem.CreateShader"),
				(SEchar*)pCompilationErrors->GetBufferPointer());
		}
		return NULL;
	}

	D3D8ShaderMaterial* shader = new D3D8Shader(pEffect);

	return shader;
}

}
