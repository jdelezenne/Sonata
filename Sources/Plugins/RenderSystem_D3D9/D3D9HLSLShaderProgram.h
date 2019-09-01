/*=============================================================================
D3D9HLSLShaderProgram.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D9HLSLSHADERPROGRAM_H_
#define _SE_D3D9HLSLSHADERPROGRAM_H_

#include "D3D9RenderSystem.h"
#include "D3D9HLSLShaderSystem.h"
#include "D3D9HLSLShaderParameter.h"

namespace SE_D3D9
{

/** Direct3D9 HLSL Shader Program. */
class D3D9HLSLShaderProgram
{
public:
	typedef Array<D3D9HLSLShaderParameter*> ParameterList;

protected:
	D3D9HLSLShaderSystem* _shaderSystem;
	IDirect3DDevice9* _D3DDevice;
	ID3DXBuffer* _D3DShaderBuffer;
	ID3DXConstantTable* _D3DConstantTable;
	bool _isCompiled;
	String _sourceData;
	String _entryPoint;
	String _profile;
	String _errorMessages;
	ParameterList _parameters;

public:
	virtual ~D3D9HLSLShaderProgram();

	ShaderSystem* GetShaderSystem() const { return _shaderSystem; }

	String GetSourceData() const { return _sourceData; }
	void SetSourceData(String value) { _sourceData = value; }

	String GetEntryPoint() const { return _entryPoint; }
	void SetEntryPoint(String value) { _entryPoint = value; }

	String GetProfile() const { return _profile; }
	void SetProfile(String value) { _profile = value; }

	bool IsCompiled() const { return _isCompiled; }
	String GetErrorMessages() const { return _errorMessages; }
	bool Compile();

	int GetParameterCount() const;
	ShaderParameter* GetParameterByIndex(int index) const;
	ShaderParameter* GetParameterByName(const String& name) const;

	IDirect3DDevice9* GetD3DDevice() const { return _D3DDevice; }
	ID3DXConstantTable* GetD3DXConstantTable() const { return _D3DConstantTable; }
	virtual String GetHighestProfile() = 0;

protected:
	D3D9HLSLShaderProgram(D3D9HLSLShaderSystem* shaderSystem);
	void Initialize();
};


/** D3D9 HLSL Vertex Shader. */
class D3D9HLSLVertexShaderProgram : public D3D9HLSLShaderProgram, public VertexShaderProgram
{
protected:
	IDirect3DVertexShader9* _D3DVertexShader;

public:
	D3D9HLSLVertexShaderProgram(D3D9HLSLShaderSystem* shaderSystem);
	virtual ~D3D9HLSLVertexShaderProgram();

	virtual ShaderSystem* GetShaderSystem() const { return D3D9HLSLShaderProgram::GetShaderSystem(); }
	virtual String GetSourceData() const { return D3D9HLSLShaderProgram::GetSourceData(); }
	virtual void SetSourceData(String value) { D3D9HLSLShaderProgram::SetSourceData(value); }
	virtual String GetEntryPoint() const { return D3D9HLSLShaderProgram::GetEntryPoint(); }
	virtual void SetEntryPoint(String value) { D3D9HLSLShaderProgram::SetEntryPoint(value); }
	virtual String GetProfile() const { return D3D9HLSLShaderProgram::GetProfile(); }
	virtual void SetProfile(String value) { D3D9HLSLShaderProgram::SetProfile(value); }
	virtual bool IsCompiled() const { return D3D9HLSLShaderProgram::IsCompiled(); }
	virtual String GetErrorMessages() const { return D3D9HLSLShaderProgram::GetErrorMessages(); }
	virtual bool Compile() { return D3D9HLSLShaderProgram::Compile(); }
	virtual int GetParameterCount() const { return D3D9HLSLShaderProgram::GetParameterCount(); }
	virtual ShaderParameter* GetParameterByIndex(int index) const { return D3D9HLSLShaderProgram::GetParameterByIndex(index); }
	virtual ShaderParameter* GetParameterByName(const String& name) const { return D3D9HLSLShaderProgram::GetParameterByName(name); }

	virtual bool Create();
	virtual void Destroy();
	virtual bool Bind();
	virtual bool Unbind();

	bool Initialize(CONST DWORD* pVertexShaderFunction);
	virtual String GetHighestProfile();
};


/** D3D9 HLSL Pixel Shader. */
class D3D9HLSLPixelShaderProgram : public D3D9HLSLShaderProgram, public PixelShaderProgram
{
protected:
	IDirect3DPixelShader9* _D3DPixelShader;

public:
	D3D9HLSLPixelShaderProgram(D3D9HLSLShaderSystem* shaderSystem);
	virtual ~D3D9HLSLPixelShaderProgram();

	virtual ShaderSystem* GetShaderSystem() const { return D3D9HLSLShaderProgram::GetShaderSystem(); }
	virtual String GetSourceData() const { return D3D9HLSLShaderProgram::GetSourceData(); }
	virtual void SetSourceData(String value) { D3D9HLSLShaderProgram::SetSourceData(value); }
	virtual String GetEntryPoint() const { return D3D9HLSLShaderProgram::GetEntryPoint(); }
	virtual void SetEntryPoint(String value) { D3D9HLSLShaderProgram::SetEntryPoint(value); }
	virtual String GetProfile() const { return D3D9HLSLShaderProgram::GetProfile(); }
	virtual void SetProfile(String value) { D3D9HLSLShaderProgram::SetProfile(value); }
	virtual bool IsCompiled() const { return D3D9HLSLShaderProgram::IsCompiled(); }
	virtual String GetErrorMessages() const { return D3D9HLSLShaderProgram::GetErrorMessages(); }
	virtual bool Compile() { return D3D9HLSLShaderProgram::Compile(); }
	virtual int GetParameterCount() const { return D3D9HLSLShaderProgram::GetParameterCount(); }
	virtual ShaderParameter* GetParameterByIndex(int index) const { return D3D9HLSLShaderProgram::GetParameterByIndex(index); }
	virtual ShaderParameter* GetParameterByName(const String& name) const { return D3D9HLSLShaderProgram::GetParameterByName(name); }

	virtual bool Create();
	virtual void Destroy();
	virtual bool Bind();
	virtual bool Unbind();

	bool Initialize(CONST DWORD* pPixelShaderFunction);
	virtual String GetHighestProfile();
};

}

#endif
