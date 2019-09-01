/*=============================================================================
GLSLShaderSystem.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_GLSLSHADERSYSTEM_H_
#define _SE_GLSLSHADERSYSTEM_H_

#include "GLCommon.h"

namespace SE_GL
{

/**
	OpenGL SL Shader System.

	@todo Handle GL context change.
*/
class GLSLShaderSystem : public ShaderSystem
{
public:
	GLSLShaderSystem();
	virtual ~GLSLShaderSystem();

	virtual	bool Create();

	virtual	void Destroy();

	virtual void Update(real64 elapsed);

	virtual ShaderProgram* CreateShaderProgram(ShaderProgramType type);

	virtual void DestroyShaderProgram(ShaderProgram* program);

	virtual bool SetShaderProgram(ShaderProgram* program);

	virtual bool DisableShaderProgram(ShaderProgram* program);

protected:
};

}

#endif
