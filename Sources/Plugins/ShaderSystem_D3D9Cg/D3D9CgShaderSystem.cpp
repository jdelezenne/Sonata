/*=============================================================================
D3D9CgShaderSystem.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D9CgShaderSystem.h"
#include "D3D9CgShaderProgram.h"

namespace SE_D3D9Cg
{

void __cdecl cgErrorCallback()
{
    CGerror err = cgGetError();
    const char* str = cgD3D9TranslateCGerror(err);
    if (err == cgD3D9Failed)
    {
        HRESULT hr = cgD3D9GetLastError();
        const char* error = cgD3D9TranslateHRESULT(hr);

		Logger::Current()->Log(LogLevel::Error, _T("cgD3D9"), error);
    }
	else if (err == cgD3D9DebugTrace)
    { 
        bool debugTrace = true;
    }
	else
    {
        bool otherError = true;
    }
}

D3D9CgShaderSystem::D3D9CgShaderSystem(IDirect3DDevice9* pD3DDevice) :
	ShaderSystem(),
	_D3DDevice(pD3DDevice),
	_CGContext(NULL),
	_CGVertexProfile(CG_PROFILE_UNKNOWN),
	_CGPixelProfile(CG_PROFILE_UNKNOWN)
{
	//cgD3D9EnableDebugTracing(CG_TRUE);
	cgSetErrorCallback(&cgErrorCallback);

	_CGContext = cgCreateContext();
	cgD3D9SetDevice(_D3DDevice);

	_CGVertexProfile = cgD3D9GetLatestVertexProfile();
	_CGPixelProfile = cgD3D9GetLatestPixelProfile();
}

D3D9CgShaderSystem::~D3D9CgShaderSystem()
{
	if (_CGContext != NULL)
	{
		cgDestroyContext(_CGContext);
		cgD3D9SetDevice(NULL);
	}
}

bool D3D9CgShaderSystem::Create()
{
	return true;
}

void D3D9CgShaderSystem::Destroy()
{
}

void D3D9CgShaderSystem::Update(real64 elapsed)
{
}

ShaderProgram* D3D9CgShaderSystem::CreateShaderProgram(ShaderProgramType type)
{
	ShaderProgram* program;

	if (type == ShaderProgramType_Vertex)
	{
		program = new D3D9CgVertexShaderProgram(this);
	}
	else if (type == ShaderProgramType_Pixel)
	{
		program = new D3D9CgPixelShaderProgram(this);
	}
	else
	{
		return NULL;
	}

	return program;
}

void D3D9CgShaderSystem::DestroyShaderProgram(ShaderProgram* program)
{
	if (program == NULL || program->GetShaderSystem() != this)
	{
		return;
	}

	delete program;
}

bool D3D9CgShaderSystem::SetShaderProgram(ShaderProgram* program)
{
	if (program == NULL || program->GetShaderSystem() != this)
	{
		return false;
	}

	return program->Bind();
}

bool D3D9CgShaderSystem::DisableShaderProgram(ShaderProgram* program)
{
	if (program == NULL || program->GetShaderSystem() != this)
	{
		return false;
	}

	return program->Unbind();
}

ShaderMaterial* D3D9CgShaderSystem::CreateShader(Stream& stream)
{
	/*HRESULT hr;

	int32 length = stream.GetLength();
	SEbyte* data = new SEbyte[length];
	if (stream.Read(data, length) == 0)
		return NULL;

	ID3DXEffect* pEffect;
	ID3DXBuffer* pCompilationErrors;

	hr = D3DXCreateEffect(_D3DDevice, data, length, NULL, NULL,
		D3DXSHADER_PACKMATRIX_ROWMAJOR, NULL, &pEffect, &pCompilationErrors);
	delete[] data;

	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D9CgShaderSystem.CreateShader (D3DXCreateEffect)"),
			_T("Failed to create a shader effect."));

		if (pCompilationErrors != NULL)
		{
			Logger::Current()->Log(LogLevel::Error, _T("D3D9CgShaderSystem.CreateShader"),
				(SEchar*)pCompilationErrors->GetBufferPointer());
		}
		return NULL;
	}

	D3D9CgShaderMaterial* shader = new D3D9CgShader(this, pEffect);

	return shader;*/
	return NULL;
}

void D3D9CgShaderSystem::DestroyShader(ShaderMaterial* shader)
{
	if (shader == NULL || shader->GetShaderSystem() != this)
	{
		return;
	}

	delete shader;
}

}
