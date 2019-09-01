/*=============================================================================
D3D9EffectFunction.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D9EFFECTFUNCTION_H_
#define _SE_D3D9EFFECTFUNCTION_H_

#include "D3D9HLSLShaderSystem.h"
#include "D3D9EffectShader.h"

namespace SE_D3D9
{

/** Direct3D9 Effect Function. */
class D3D9EffectFunction : public D3D9EffectVariable, public EffectFunction
{
public:
	D3D9EffectFunction(D3D9EffectShader* shader, D3DXHANDLE handle);
	virtual ~D3D9EffectFunction();

protected:
	void Initialize();
};

}

#endif
