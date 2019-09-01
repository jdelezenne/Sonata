/*=============================================================================
D3D9EffectShader.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D9EFFECTSHADER_H_
#define _SE_D3D9EFFECTSHADER_H_

#include "D3D9HLSLShaderSystem.h"

namespace SE_D3D9
{

class D3D9EffectShader;
class D3D9EffectParameter;
class D3D9EffectTechnique;
class D3D9EffectFunction;

/** Direct3D9 Effect Variable. */
class D3D9EffectVariable
{
protected:
	D3D9EffectShader* _effect;
	D3DXHANDLE _handle;

public:
	D3D9EffectVariable(D3D9EffectShader* effect, D3DXHANDLE handle);
	virtual ~D3D9EffectVariable();

	D3DXHANDLE GetHandle() const { return _handle; }
};


/** Direct3D9 Effect Shader. */
class D3D9EffectShader : public EffectShader
{
public:
	typedef Array<D3D9EffectParameter*> ParameterList;
	typedef Array<D3D9EffectTechnique*> TechniqueList;
	typedef Array<D3D9EffectFunction*> FunctionList;

protected:
	D3D9HLSLShaderSystem* _shaderSystem;
	ID3DXEffect* _D3DXEffect;
	ParameterList _parameters;
	TechniqueList _techniques;
	FunctionList _functions;

public:
	D3D9EffectShader(D3D9HLSLShaderSystem* shaderSystem, ID3DXEffect* pD3DEffect);
	virtual ~D3D9EffectShader();

	virtual ShaderSystem* GetShaderSystem() const { return _shaderSystem; }

	virtual int GetParameterCount() const;
	virtual EffectParameter* GetParameterByIndex(int index) const;
	virtual EffectParameter* GetParameterByName(const String& name) const;
	virtual EffectParameter* GetParameterBySemantic(const String& semantic) const;

	virtual int GetTechniqueCount() const;
	virtual EffectTechnique* GetTechniqueByIndex(int index) const;
	virtual EffectTechnique* GetTechniqueByName(const String& name) const;

	virtual int GetFunctionCount() const;
	virtual EffectFunction* GetFunctionByIndex(int index) const;
	virtual EffectFunction* GetFunctionByName(const String& name) const;

	virtual void BeginTechnique(EffectTechnique* technique);
	virtual void BeginTechniqueFromIndex(int index);
	virtual EffectTechnique* GetActiveTechnique();
	virtual void EndTechnique();

	ID3DXEffect* GetD3DXEffect() const { return _D3DXEffect; }

protected:
	void Initialize();
};

}

#endif
