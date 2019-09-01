/*=============================================================================
GLSLShaderProgram.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_GLSLSHADERPROGRAM_H_
#define _SE_GLSLSHADERPROGRAM_H_

#include "GLSLShaderSystem.h"

namespace SE_GL
{

/**
	GLSL shader program.

	@todo
		fix link error
		fix common program object
		glUniform1iARB to set the texture parameter
*/
class GLSLShaderProgram : public ShaderProgram
{
public:
	virtual ~GLSLShaderProgram();

	virtual bool IsCompiled() const { return _IsCompiled; }

	virtual String GetSourceData() const { return _SourceData; }
	virtual void SetSourceData(String value) { _SourceData = value; }

	virtual String GetEntryPoint() const { return _EntryPoint; }
	virtual void SetEntryPoint(String value) { _EntryPoint = value; }

	virtual String GetProfile() const { return _Profile; }
	virtual void SetProfile(String value) { _Profile = value; }

	virtual String GetErrorMessages() const { return _ErrorMessages; }

	virtual int GetParameterCount() const;
	virtual ShaderParameter* GetParameterByIndex(int index) const;
	virtual ShaderParameter* GetParameterByName(const String& name) const;
	virtual bool SetParameter(int index, ShaderParameter* parameter);
	virtual bool SetParameter(const String& name, ShaderParameter* parameter);

	virtual bool Compile();
	virtual bool Create();
	virtual void Destroy();

protected:
	GLSLShaderProgram(GLSLShaderSystem* shaderSystem);
	bool SetGLParameter(GLint glParameter, ShaderParameter* parameter);

protected:
	GLSLShaderSystem* _shaderSystem;
	static GLhandleARB _GLProgram;//fix
	GLhandleARB _GLShader;

	bool _IsCompiled;
	String _SourceData;
	String _EntryPoint;
	String _Profile;
	String _ErrorMessages;
};

/** GLSL vertex shader. */
class GLSLVertexShaderProgram : public GLSLShaderProgram
{
public:
	GLSLVertexShaderProgram(GLSLShaderSystem* shaderSystem);
	virtual ~GLSLVertexShaderProgram();

	virtual ShaderProgramType GetProgramType() const { return ShaderProgramType_Vertex; }

	virtual bool Bind();
	virtual bool Unbind();
};

/** GLSL pixel shader. */
class GLSLPixelShaderProgram : public GLSLShaderProgram
{
public:
	GLSLPixelShaderProgram(GLSLShaderSystem* shaderSystem);
	virtual ~GLSLPixelShaderProgram();

	virtual ShaderProgramType GetProgramType() const { return ShaderProgramType_Pixel; }

	virtual bool Bind();
	virtual bool Unbind();
};

}

#endif
