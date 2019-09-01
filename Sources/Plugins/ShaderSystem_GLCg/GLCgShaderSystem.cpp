/*=============================================================================
GLCgShaderSystem.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "GLCgShaderSystem.h"
#include "GLCgShaderProgram.h"

namespace SE_GLCg
{

bool cgCheckError()
{
    CGerror err = cgGetError();
	if (err != CG_NO_ERROR)
	{
		const char* str = cgGetErrorString(err);
		Logger::Current()->Log(LogLevel::Error, _T("cgGL"), str);
		return false;
    }

	return true;
}

void __cdecl cgErrorCallback()
{
    CGerror err = cgGetError();
	if (err != CG_NO_ERROR)
	{
		const char* str = cgGetErrorString(err);
		Logger::Current()->Log(LogLevel::Error, _T("cgGL"), str);
    }
}

GLCgShaderSystem::GLCgShaderSystem() :
	ShaderSystem(),
	_CGContext(NULL),
	_CGVertexProfile(CG_PROFILE_UNKNOWN),
	_CGPixelProfile(CG_PROFILE_UNKNOWN)
{
	//cgGLEnableDebugTracing(CG_TRUE);
	cgSetErrorCallback(&cgErrorCallback);

	_CGContext = cgCreateContext();

	_CGVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
	_CGPixelProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
}

GLCgShaderSystem::~GLCgShaderSystem()
{
	if (_CGContext != NULL)
	{
		cgDestroyContext(_CGContext);
	}
}

bool GLCgShaderSystem::Create()
{
	return true;
}

void GLCgShaderSystem::Destroy()
{
}

void GLCgShaderSystem::Update(real64 elapsed)
{
}

ShaderProgram* GLCgShaderSystem::CreateShaderProgram(ShaderProgramType type)
{
	ShaderProgram* program;

	if (type == ShaderProgramType_Vertex)
	{
		program = new GLCgVertexShaderProgram(this);
	}
	else if (type == ShaderProgramType_Pixel)
	{
		program = new GLCgPixelShaderProgram(this);
	}
	else
	{
		return NULL;
	}

	return program;
}

void GLCgShaderSystem::DestroyShaderProgram(ShaderProgram* program)
{
	if (program == NULL)
		return;

	delete program;
}

bool GLCgShaderSystem::SetShaderProgram(ShaderProgram* program)
{
	if (program == NULL)
		return false;

	return program->Bind();
}

bool GLCgShaderSystem::DisableShaderProgram(ShaderProgram* program)
{
	return program->Unbind();
}

}
