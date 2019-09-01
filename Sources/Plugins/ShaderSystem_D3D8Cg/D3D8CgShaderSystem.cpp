/*=============================================================================
D3D8CgShaderSystem.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D8CgShaderSystem.h"
#include "D3D8CgShaderProgram.h"

namespace SE_D3D8Cg
{

void __cdecl cgErrorCallback()
{
    CGerror err = cgGetError();
    const char* str = cgD3D8TranslateCGerror(err);
    if (err == cgD3D8Failed)
    {
        HRESULT hr = cgD3D8GetLastError();
        const char* error = cgD3D8TranslateHRESULT(hr);

		Logger::Current()->Log(LogLevel::Error, _T("cgD3D8"), error);
    }
	else if (err == cgD3D8DebugTrace)
    { 
        bool debugTrace = true;
    }
	else
    {
        bool otherError = true;
    }
}

D3D8CgShaderSystem::D3D8CgShaderSystem(IDirect3DDevice8* pD3DDevice) :
	ShaderSystem(),
	_D3DDevice(pD3DDevice),
	_CGContext(NULL),
	_CGVertexProfile(CG_PROFILE_UNKNOWN),
	_CGPixelProfile(CG_PROFILE_UNKNOWN)
{
	//cgD3D8EnableDebugTracing(CG_TRUE);
	cgSetErrorCallback(&cgErrorCallback);

	_CGContext = cgCreateContext();
	cgD3D8SetDevice(_D3DDevice);

	_CGVertexProfile = cgD3D8GetLatestVertexProfile();
	_CGPixelProfile = cgD3D8GetLatestPixelProfile();
}

D3D8CgShaderSystem::~D3D8CgShaderSystem()
{
	if (_CGContext != NULL)
	{
		cgDestroyContext(_CGContext);
		cgD3D8SetDevice(NULL);
	}
}

bool D3D8CgShaderSystem::Create()
{
	return true;
}

void D3D8CgShaderSystem::Destroy()
{
}

void D3D8CgShaderSystem::Update(real64 elapsed)
{
}

ShaderProgram* D3D8CgShaderSystem::CreateShaderProgram(ShaderProgramType type)
{
	ShaderProgram* program;

	if (type == ShaderProgramType_Vertex)
	{
		program = new D3D8CgVertexShaderProgram(this);
	}
	else if (type == ShaderProgramType_Pixel)
	{
		program = new D3D8CgPixelShaderProgram(this);
	}
	else
	{
		return NULL;
	}

	return program;
}

void D3D8CgShaderSystem::DestroyShaderProgram(ShaderProgram* program)
{
	if (program == NULL)
		return;

	delete program;
}

bool D3D8CgShaderSystem::SetShaderProgram(ShaderProgram* program)
{
	if (program == NULL)
		return false;

	return program->Bind();
}

bool D3D8CgShaderSystem::DisableShaderProgram(ShaderProgram* program)
{
	return program->Unbind();
}

}
