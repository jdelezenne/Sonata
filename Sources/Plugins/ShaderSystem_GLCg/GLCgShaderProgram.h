/*=============================================================================
GLCgShaderProgram.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_GLCGSHADERPROGRAM_H_
#define _SE_GLCGSHADERPROGRAM_H_

#include "GLCgShaderSystem.h"

namespace SE_GLCg
{

/** GLCg shader. */
class GLCgShaderProgram : public ShaderProgram
{
public:
	virtual ~GLCgShaderProgram();

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
	GLCgShaderProgram(GLCgShaderSystem* shaderSystem);
	CGparameter GetCgParameterByIndex(int index) const;
	bool SetCgParameter(CGparameter cgParameter, ShaderParameter* parameter);

protected:
	GLCgShaderSystem* _shaderSystem;
	CGprogram _CGprogram;
	CGprofile _CGprofile;

	bool _IsCompiled;
	String _SourceData;
	String _EntryPoint;
	String _Profile;
	String _ErrorMessages;
};

/** GLCg vertex shader. */
class GLCgVertexShaderProgram : public GLCgShaderProgram
{
public:
	GLCgVertexShaderProgram(GLCgShaderSystem* shaderSystem);
	virtual ~GLCgVertexShaderProgram();

	virtual ShaderProgramType GetProgramType() const { return ShaderProgramType_Vertex; }

	virtual bool Bind();
	virtual bool Unbind();
};

/** GLCg pixel shader. */
class GLCgPixelShaderProgram : public GLCgShaderProgram
{
public:
	GLCgPixelShaderProgram(GLCgShaderSystem* shaderSystem);
	virtual ~GLCgPixelShaderProgram();

	virtual ShaderProgramType GetProgramType() const { return ShaderProgramType_Pixel; }

	virtual bool Bind();
	virtual bool Unbind();
};

}

#endif
