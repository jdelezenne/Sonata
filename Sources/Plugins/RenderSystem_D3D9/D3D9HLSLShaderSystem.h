/*=============================================================================
D3D9HLSLShaderSystem.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D9HLSLSHADERSYSTEM_H_
#define _SE_D3D9HLSLSHADERSYSTEM_H_

#include "D3D9RenderSystem.h"

namespace SE_D3D9
{

class D3D9HLSLShaderSystem : public ShaderSystem
{
protected:
	IDirect3DDevice9* _D3DDevice;

public:
	D3D9HLSLShaderSystem(IDirect3DDevice9* pD3DDevice);
	virtual ~D3D9HLSLShaderSystem();

	virtual ShaderProgram* CreateShaderProgram(ShaderProgramType type);

	virtual void DestroyShaderProgram(ShaderProgram* program);

	virtual bool SetShaderProgram(ShaderProgram* program);

	virtual bool DisableShaderProgram(ShaderProgram* program);

	virtual EffectShader* CreateEffectShader(const String& source);

	virtual void DestroyEffectShader(EffectShader* shader);

	IDirect3DDevice9* GetD3DDevice() const { return _D3DDevice; }
};

}

#endif
