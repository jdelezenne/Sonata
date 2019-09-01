/*=============================================================================
D3D8CgShaderProgram.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D8CGSHADERPROGRAM_H_
#define _SE_D3D8CGSHADERPROGRAM_H_

#include "D3D8CgShaderSystem.h"

namespace SE_D3D8Cg
{

/** D3D8Cg shader. */
class D3D8CgShaderProgram : public ShaderProgram
{
public:
	virtual ~D3D8CgShaderProgram();

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
	D3D8CgShaderProgram(D3D8CgShaderSystem* shaderSystem);
	CGparameter GetCgParameterByIndex(int index) const;
	bool SetCgParameter(CGparameter cgParameter, ShaderParameter* parameter);

protected:
	D3D8CgShaderSystem* _shaderSystem;
	CGprogram _CGprogram;

	bool _IsCompiled;
	String _SourceData;
	String _EntryPoint;
	String _Profile;
	String _ErrorMessages;
};

/** D3D8Cg vertex shader. */
class D3D8CgVertexShaderProgram : public D3D8CgShaderProgram
{
public:
	D3D8CgVertexShaderProgram(D3D8CgShaderSystem* shaderSystem);
	virtual ~D3D8CgVertexShaderProgram();

	virtual ShaderProgramType GetProgramType() const { return ShaderProgramType_Vertex; }

	virtual bool Bind();
	virtual bool Unbind();
};

/** D3D8Cg pixel shader. */
class D3D8CgPixelShaderProgram : public D3D8CgShaderProgram
{
public:
	D3D8CgPixelShaderProgram(D3D8CgShaderSystem* shaderSystem);
	virtual ~D3D8CgPixelShaderProgram();

	virtual ShaderProgramType GetProgramType() const { return ShaderProgramType_Pixel; }

	virtual bool Bind();
	virtual bool Unbind();
};

}

#endif
