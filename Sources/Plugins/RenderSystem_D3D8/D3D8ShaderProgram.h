/*=============================================================================
D3D8ShaderProgram.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D8SHADERPROGRAM_H_
#define _SE_D3D8SHADERPROGRAM_H_

#include "D3D8RenderSystem.h"
#include "D3D8ShaderSystem.h"

namespace SE_D3D8
{

/** Direct3D8 shader program. */
class D3D8ShaderProgram : public ShaderProgram
{
public:
	virtual ~D3D8ShaderProgram();

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

protected:
	D3D8ShaderProgram(IDirect3DDevice8* pD3DDevice);

protected:
	IDirect3DDevice8* _D3DDevice;
	ID3DXBuffer* _D3DShaderBuffer;

	bool _IsCompiled;
	String _SourceData;
	String _EntryPoint;
	String _Profile;
	String _ErrorMessages;
};

/** D3D8  vertex shader. */
class D3D8VertexShaderProgram : public D3D8ShaderProgram
{
public:
	D3D8VertexShaderProgram(IDirect3DDevice8* pD3DDevice);
	virtual ~D3D8VertexShaderProgram();

	virtual ShaderProgramType GetProgramType() const { return ShaderProgramType_Vertex; }

	virtual bool Create();
	virtual void Destroy();
	virtual bool Bind();
	virtual bool Unbind();

protected:
	DWORD _D3DVertexShader;
};

/** D3D8  pixel shader. */
class D3D8PixelShaderProgram : public D3D8ShaderProgram
{
public:
	D3D8PixelShaderProgram(IDirect3DDevice8* pD3DDevice);
	virtual ~D3D8PixelShaderProgram();

	virtual ShaderProgramType GetProgramType() const { return ShaderProgramType_Pixel; }

	virtual bool Create();
	virtual void Destroy();
	virtual bool Bind();
	virtual bool Unbind();

protected:
	DWORD _D3DPixelShader;
};

}

#endif
