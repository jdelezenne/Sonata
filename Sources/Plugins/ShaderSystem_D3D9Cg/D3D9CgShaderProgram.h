/*=============================================================================
D3D9CgShaderProgram.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D9CGSHADERPROGRAM_H_
#define _SE_D3D9CGSHADERPROGRAM_H_

#include "D3D9CgShaderSystem.h"

namespace SE_D3D9Cg
{

/** D3D9Cg shader. */
class D3D9CgShaderProgram : public ShaderProgram
{
protected:
	D3D9CgShaderSystem* _shaderSystem;
	CGprogram _CGprogram;
	bool _IsCompiled;
	String _SourceData;
	String _EntryPoint;
	String _Profile;
	String _ErrorMessages;

public:
	virtual ~D3D9CgShaderProgram();

	virtual ShaderSystem* GetShaderSystem() const { return _shaderSystem; }

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
	D3D9CgShaderProgram(D3D9CgShaderSystem* shaderSystem);
	CGparameter GetCgParameterByIndex(int index) const;
	bool SetCgParameter(CGparameter cgParameter, ShaderParameter* parameter);
};

/** D3D9Cg vertex shader. */
class D3D9CgVertexShaderProgram : public D3D9CgShaderProgram
{
public:
	D3D9CgVertexShaderProgram(D3D9CgShaderSystem* shaderSystem);
	virtual ~D3D9CgVertexShaderProgram();

	virtual ShaderProgramType GetProgramType() const { return ShaderProgramType_Vertex; }

	virtual bool Bind();
	virtual bool Unbind();
};

/** D3D9Cg pixel shader. */
class D3D9CgPixelShaderProgram : public D3D9CgShaderProgram
{
public:
	D3D9CgPixelShaderProgram(D3D9CgShaderSystem* shaderSystem);
	virtual ~D3D9CgPixelShaderProgram();

	virtual ShaderProgramType GetProgramType() const { return ShaderProgramType_Pixel; }

	virtual bool Bind();
	virtual bool Unbind();
};

}

#endif
