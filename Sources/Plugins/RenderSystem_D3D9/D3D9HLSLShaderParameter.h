/*=============================================================================
D3D9HLSLShaderParameter.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D9HLSLSHADERPARAMETER_H_
#define _SE_D3D9HLSLSHADERPARAMETER_H_

#include "D3D9HLSLShaderSystem.h"

namespace SE_D3D9
{

class D3D9HLSLShaderProgram;

/** Direct3D9 HLSL Shader Parameter. */
class D3D9HLSLShaderParameter : public ShaderParameter
{
public:
	typedef Array<D3D9HLSLShaderParameter*> ParameterList;

protected:
	D3D9HLSLShaderProgram* _shaderProgram;
	D3DXHANDLE _handle;
	ParameterList _members;

public:
	D3D9HLSLShaderParameter(D3D9HLSLShaderProgram* shaderProgram, D3DXHANDLE handle);
	virtual ~D3D9HLSLShaderParameter();

	virtual ShaderParameter* GetMemberByIndex(int index) const;
	virtual ShaderParameter* GetMemberByName(const String& name) const;

	virtual bool SetValue(void* value);

protected:
	void Initialize();
};

}

#endif
