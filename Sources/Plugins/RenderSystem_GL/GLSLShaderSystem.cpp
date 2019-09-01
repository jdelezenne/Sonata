/*=============================================================================
GLSLShaderSystem.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "GLSLShaderSystem.h"
#include "GLSLShaderProgram.h"

namespace SE_GL
{

GLSLShaderSystem::GLSLShaderSystem() :
	ShaderSystem()
{
}

GLSLShaderSystem::~GLSLShaderSystem()
{
}

bool GLSLShaderSystem::Create()
{
	return true;
}

void GLSLShaderSystem::Destroy()
{
}

void GLSLShaderSystem::Update(real64 elapsed)
{
}

ShaderProgram* GLSLShaderSystem::CreateShaderProgram(ShaderProgramType type)
{
	ShaderProgram* program;

	if (type == ShaderProgramType_Vertex)
	{
		program = new GLSLVertexShaderProgram(this);
	}
	else if (type == ShaderProgramType_Pixel)
	{
		program = new GLSLPixelShaderProgram(this);
	}
	else
	{
		return NULL;
	}

	return program;
}

void GLSLShaderSystem::DestroyShaderProgram(ShaderProgram* program)
{
	if (program == NULL)
		return;

	delete program;
}

bool GLSLShaderSystem::SetShaderProgram(ShaderProgram* program)
{
	if (program == NULL)
		return false;

	return program->Bind();
}

bool GLSLShaderSystem::DisableShaderProgram(ShaderProgram* program)
{
	return program->Unbind();
}

}
